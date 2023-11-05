#include "CTextureResourceLoader.hpp"

#include "CResourceLoaderFactory.hpp"
#include "Math.hpp"
#include "Types.hpp"

#include "CDataManager.hpp"
#include "SGroundTruth.hpp"
#include "SHeightMap.hpp"
#include "STexture.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QPen>

#include <iostream> // TODO: Remove this.
#include <memory>
#include <utility>
namespace
{
QString SAVE_TO_FILE_NAME {"tracksTexture.png"};

[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CTextureResourceLoader>::Register(Types::eResource::Texture)};
}

Types::eLoadResult CTextureResourceLoader::LoadResource()
{
    // TODO: Remove this.
    std::cout << "[TextureResource] Loading texture" << std::endl;

    QImage TextureImage {CreateBackgroundTexture()};
    DrawGroundTruth(TextureImage);

    // TODO: Why are we mirroring this??
    QImage TextureImageMirror = std::move(TextureImage).mirrored(true, true);

    QString SaveToFilePath {GetResourceFilePath(SAVE_TO_FILE_NAME)};
    TextureImageMirror.save(SaveToFilePath);

    std::unique_ptr<STexture> Texture {std::make_unique<STexture>(std::move(TextureImageMirror))};
    mDataManager.SetTexture(std::move(Texture));

    return Types::eLoadResult::Successful;
}

QImage CTextureResourceLoader::CreateBackgroundTexture()
{
    const s32 TextureSizeX {static_cast<s32>(mDataManager.GetHeightMap().oResolution.oX)};
    const s32 TextureSizeY {static_cast<s32>(mDataManager.GetHeightMap().oResolution.oY)};

    QImage TextureImage {TextureSizeX, TextureSizeY, QImage::Format::Format_RGBA8888};
    TextureImage.fill(QColor(255, 0, 0));

    return TextureImage;
}

void CTextureResourceLoader::DrawGroundTruth(QImage& aImage)
{
    QPen     Pen {QColor(255, 255, 255), 10, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin};
    QPainter Painter {&aImage};

    const SGroundTruth& GroundTruth {mDataManager.GetGroundTruth()};

    for (const auto& Track : GroundTruth.oNetwork)
    {
        QPainterPath PathPainter;
        for (u32 PointIndex {0U}; PointIndex < Track.size(); PointIndex++)
        {
            // TODO: Take min and max values from the terrain resolution.
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

Math::Vector2D<s32> CTextureResourceLoader::WorldToTexCoords(const Math::Vector2D<f64>& aWorldPoint, const Math::Box2D& aWorldBounds, const QSize& aTextureSize) const
{
    Math::Vector2D<f64> WorldDisplacement {aWorldPoint - aWorldBounds.oMin};
    Math::Vector2D<f64> WorldSize {aWorldBounds.oMax - aWorldBounds.oMin};

    f64 NormalizedX {WorldDisplacement.oX / WorldSize.oX};
    f64 NormalizedY {WorldDisplacement.oY / WorldSize.oY};

    s32 PixelColumn {static_cast<s32>(NormalizedX * (aTextureSize.width() - 1))};
    s32 PixelRow {static_cast<s32>(NormalizedY * (aTextureSize.height() - 1))};

    return {PixelColumn, PixelRow};
}
