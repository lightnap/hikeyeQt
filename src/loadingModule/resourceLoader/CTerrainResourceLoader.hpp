#ifndef C_TERRAIN_RESOURCE_LOADER_H
#define C_TERRAIN_RESOURCE_LOADER_H

#include "CResourceLoader.hpp"

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
     */
    virtual void LoadResource();
};

#endif // C_TERRAIN_RESOURCE_LOADER_H
