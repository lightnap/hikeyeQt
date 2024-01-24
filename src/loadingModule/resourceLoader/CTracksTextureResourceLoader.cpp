#include "CTracksTextureResourceLoader.hpp"

#include "common/Math.hpp"
#include "common/Types.hpp"
#include "loadingModule/resourceLoaderFactory/CResourceLoaderFactory.hpp"

#include "dataStructures/CDataManager.hpp"
#include "dataStructures/SGroundTruth.hpp"
#include "dataStructures/SHeightMap.hpp"
#include "dataStructures/STexture.hpp"
#include "graphics/CCustomColourSpectrum.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QPen>

#include <iostream>
#include <memory>
#include <utility>

namespace
{
    QString SAVE_TO_FILE_NAME {"tracksTexture.png"};

    [[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTracksTextureResourceLoader>::Register(Types::eResource::TracksTexture)};
}

Types::eLoadResult CTracksTextureResourceLoader::LoadResource()
{
    std::cout << "[TracksTextureResource] Loading tracks texture" << std::endl;

    // TODO: HK-54 Why are we mirroring this??
    QImage TextureImage {mDataManager.GetTexture().oTexture.mirrored(true, true)};

    DrawGroundTruth(TextureImage);

    QString SaveToFilePath {GetResourceFilePath(SAVE_TO_FILE_NAME)};

    // TODO: HK-54 Why are we mirroring this??
    TextureImage.mirrored(false, true).save(SaveToFilePath);

    // TODO: HK-54 Why are we mirroring this??
    QImage TextureImageMirror = std::move(TextureImage).mirrored(true, true);

    std::unique_ptr<STexture> Texture {std::make_unique<STexture>(std::move(TextureImageMirror))};
    mDataManager.SetTexture(std::move(Texture));

    return Types::eLoadResult::Successful;
}

void CTracksTextureResourceLoader::DrawGroundTruth(QImage& aImage)
{
    QPainter Painter {&aImage};

    const SGroundTruth& GroundTruth {mDataManager.GetGroundTruth()};

    for (u32 TrackIndex {0U}; TrackIndex < GroundTruth.oNetwork.size(); TrackIndex++)
    {
        const auto&  Track {GroundTruth.oNetwork.at(TrackIndex)};
        QPen         Pen {GetPen(TrackIndex)};
        QPainterPath PathPainter;
        for (u32 PointIndex {0U}; PointIndex < Track.size(); PointIndex++)
        {
            // TODO: HK-52 Take min and max values from the terrain resolution.
            // So avoid being harcoded.
            Math::Vector2D<f64> Min(444825.0, 4633335.0 - 2 * aImage.height());
            Math::Vector2D<f64> Max(444825.0 + 2 * aImage.width(), 4633335.0);
            Math::Box2D         DomainBox {Min, Max};

            Math::Vector2D<s32> TextureCoordinates {WorldToTexCoords(Track.at(PointIndex), DomainBox, aImage.size())};

            if (PointIndex == 0U)
            {
                PathPainter.moveTo(TextureCoordinates.oX, TextureCoordinates.oY);
            }
            else
            {
                PathPainter.lineTo(TextureCoordinates.oX, TextureCoordinates.oY);
            }
        }
        Painter.setPen(Pen);
        Painter.drawPath(PathPainter);
    }
}

QPen CTracksTextureResourceLoader::GetPen(u32 aTrackIndex)
{

    const auto& Queries {mDataManager.GetQueries()};
    const s32   CrossingCount {static_cast<s32>(Queries.oCrossingCount.at(aTrackIndex))};

    static constexpr s32 MAX_CROSSINGS {15};
    const f32            CrossingsPercentage {std::min(1.0f, static_cast<f32>(CrossingCount) / static_cast<f32>(MAX_CROSSINGS))};

    auto Color {CCustomColourSpectrum::CoolWarm().GetColor(CrossingsPercentage)};

    static constexpr f32 MAX_BRUSH_WIDTH {30.0f};
    static constexpr f32 MIN_BRUSH_WIDTH {3.0f};
    const f64            BrushWidth {Math::Lerp(MIN_BRUSH_WIDTH, MAX_BRUSH_WIDTH, CrossingsPercentage)};

    QPen Pen {Color.ToQColor(), BrushWidth, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin};
    return Pen;
}

Math::Vector2D<s32> CTracksTextureResourceLoader::WorldToTexCoords(const Math::Vector2D<f64>& aWorldPoint, const Math::Box2D& aWorldBounds, const QSize& aTextureSize) const
{
    Math::Vector2D<f64> WorldDisplacement {aWorldPoint - aWorldBounds.oMin};
    Math::Vector2D<f64> WorldSize {aWorldBounds.oMax - aWorldBounds.oMin};

    f64 NormalizedX {WorldDisplacement.oX / WorldSize.oX};
    f64 NormalizedY {WorldDisplacement.oY / WorldSize.oY};

    s32 PixelColumn {static_cast<s32>(NormalizedX * (aTextureSize.width() - 1))};
    s32 PixelRow {static_cast<s32>(NormalizedY * (aTextureSize.height() - 1))};

    return {PixelColumn, PixelRow};
}
