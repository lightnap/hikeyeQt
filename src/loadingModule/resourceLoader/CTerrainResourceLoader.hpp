#ifndef C_TERRAIN_RESOURCE_LOADER_H
#define C_TERRAIN_RESOURCE_LOADER_H

#include "CResourceLoader.hpp"
#include "Types.hpp"

/**
 * @brief Class that loads the terrain 3d model.
 */
class CTerrainResourceLoader : public CResourceLoader
{
    Q_OBJECT

  public:
    /**
     * Inherited constructor.
     */
    using CResourceLoader::CResourceLoader;

  private:
    /**
     * @brief Loads the corresponding resource.
     * @return Code indicating the load result.
     */
    Types::eResourceLoadingError LoadResource() override;
};

#endif // C_TERRAIN_RESOURCE_LOADER_H