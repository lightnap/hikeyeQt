#include "CHeightMapResourceLoader.hpp"

#include "CConfigs.hpp"
#include "CResourceLoaderFactory.hpp"
#include "SHeightMap.hpp"
#include "Types.hpp"

#include <QDir>
#include <QFile>
#include <QImage>
#include <QString>

#include <iostream> // TODO: Remove this.
#include <memory>
#include <utility>

namespace
{
QString   FILE_NAME {"dem.png"};
const s32 MIN_HEIGHT {608};  // TODO HK-49: Make this into config files or something.
const s32 MAX_HEIGHT {1692}; // TODO HK-49: Make this into config files or something.

[[maybe_unused]] const bool FactoryRegistered {CConcreteResourceLoaderFactory<CHeightMapResourceLoader>::Register(Types::eResource::HeightMap)};
}

Types::eLoadResult CHeightMapResourceLoader::LoadResource()
{
    QString ResourceFilePath {GetResourceFilePath(FILE_NAME)};

    // TODO: Remove this.
    std::cout << "[Height] LoadingFile: " << ResourceFilePath.toStdString() << std::endl;

    if (!QFile::exists(ResourceFilePath))
    {
        mLoadErrorMessage = "Height map file not found.";
        return Types::eLoadResult::Error;
    }

    QImage HeightMapTexture;
    HeightMapTexture.load(ResourceFilePath);

    SHeightMapConfig Config(MIN_HEIGHT, MAX_HEIGHT);

    auto HeightMap {std::make_unique<SHeightMap>(HeightMapTexture, Config)};
    mDataManager.SetHeightMap(std::move(HeightMap));

    if (isInterruptionRequested())
    {
        mLoadErrorMessage = "Interrupted by user";
        return Types::eLoadResult::Interrupted;
    }

    return Types::eLoadResult::Successful;
}
