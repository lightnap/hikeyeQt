#ifndef TYPES_H
#define TYPES_H

#include <array>
#include <cstdint>
#include <string>

using u8 = uint8_t;   //!< Type for unsigned integer of 1 byte.
using u16 = uint16_t; //!< Type for unsigned integer of 2 bytes.
using u32 = uint32_t; //!< Type for unsigned integer of 4 bytes.
using u64 = uint64_t; //!< Type for unsigned integer of 8 bytes.

using s8 = int8_t;   //!< Type for signed integer of 1 byte.
using s16 = int16_t; //!< Type for signed integer of 2 bytes.
using s32 = int32_t; //!< Type for signed integer of 4 bytes.
using s64 = int64_t; //!< Type for signed integer of 8 bytes.

using f32 = float;  //!< Type for floating point number of 4 bytes.
using f64 = double; //!< Type for floating point number of 8 bytes.

namespace Types
{
    /**
     * @brief Enum describing possible Loading Module types.
     */
    enum class eLoadingModule
    {
        Terrain = 0, //!< Loading module that loads terrain info.
        Tracks,      //!< Loading module that loads tack info.
        SaveImage,   //!< Loading module that saves an image into file.
        Size         //!< Size of this enum.
    };

    /**
     * @brief Enum describing the several resources to be loaded.
     */
    enum class eResource
    {
        None = 0,       //!< No resource to be loaded, or empty resource.
        HeightMap,      //!< Image file with terrain height data.
        Terrain,        //!< Terrain 3d model.
        TerrainTexture, //!< Shading texture for the terrain.
        GroundTruth,    //!< Graph of tracks we consider as the ground truth.
        Matches,        //!< Recorded tracks we want to analyze.
        Queries,        //!< Ground Truth enriched with info from tracks.
        TracksTexture,  //!< Image containing the queries info, to be displyed on map.
        SaveImage,      //!< Image to be saved on filesystem.
        Size,           //!< Size of this enum.
    };

    /**
     * @brief Enum describing possible errors while loading resource.
     */
    enum class eLoadResult
    {
        Successful = 0, //!< No error while loading.
        Interrupted,    //!< Load interrupted by user.
        Error,          //!< Load stopped due to error.
        Size            //!< Size of this enum.
    };

    /**
     * @brief Enum describing the direction of the crossing.
     */
    enum class eDirection
    {
        None = 0, //!< Unknown direction.
        Positive, //!< Destination node had higher osmId than source node.
        Negative, //!< Destination node had lower osmId than source node.
    };

    /**
     * @brief Enum describing the possible strategies we can use to paint info from matchings.
     */
    enum class ePaintStrategy
    {
        None,                   //!< No strategy, all painting is done the same.
        CountCrossings,         //!< Heatmap depending on how many times a track was crossed.
        CountCrossingsPerMatch, //!< Heatmap depending on how many times a track was crossed. Multiple corssings on one matching count as one.
        Speed,                  //!< Headmap based on speed.
        Directions,             //!< Heatmap based on whether tracks crossing in each direction is balanced.
    };

    /**
     * @brief Struct for describing a filter of dates.
     */
    struct sDateFilter
    {
        std::array<bool, 8>  FilteredDays {false}; //!< In the days of week qt enum monday starts with 1.
        std::array<bool, 13> FilteredMonths {false};
    };

    /**
     * @brief Reflection for the eResource Enum.
     * @param aModuleType Moduletype enum we want to turn into string.
     * @retrun String version of the Moduletype enum.
     */
    std::string ResourceToString(Types::eResource aResource);

    /**
     * @brief Reflection for the eModuleType Enum.
     * @param aModuleType eModuletype enum we want to turn into string.
     * @retrun String version of the Moduletype enum.
     */
    std::string LoadingModuleToString(Types::eLoadingModule aModule);
}

#endif // TYPES_H
