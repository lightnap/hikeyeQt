#ifndef C_QUERIES_RESOURCE_LOADER_H
#define C_QUERIES_RESOURCE_LOADER_H

#include "common/Types.hpp"
#include "loadingModule/resourceLoader/CResourceLoader.hpp"

/**
 * @brief Class that enriches the ground truth with data through queries.
 */
class CQueriesResourceLoader : public CResourceLoader
{
    Q_OBJECT

  public:
    /**
     * @brief Inherited constructor.
     */
    using CResourceLoader::CResourceLoader;

  private:
    /**
     * @brief Loads the corresponding resource.
     * @return Code indicating the load result.
     */
    Types::eLoadResult LoadResource() override;
};

#endif // C_QUERIES_RESOURCE_LOADER_H
