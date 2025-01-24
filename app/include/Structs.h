/** @file Structs.h
 * Contains data structures and type macros used by this software
*/
#pragma once

#include "ITM.h"
#include "Enums.h"

#include <string>  // for std::string

/////////////////////////////
// Data Structures

/** Parameters provided to the command line driver */
struct DrvrParams {
        std::string in_file = "";  /**< Input file */
        std::string out_file = ""; /**< Output file */
        ITS::Propagation::ITM::PropagationMode mode = "";     /**< Model prediction mode */
        bool DBG = false;          /**< Dump intermediate values to file? */
};


// clang-format off

/** Input parameters for point-to-point mode */
struct P2PParams {
        ITS::Propagation::ITM::PathParameters path;       /**< PathParameters inputs */
        ITS::Propagation::ITM::EnvironmentParameters env; /**< EnvironmentParameters inputs */
        ITS::Propagation::ITM::RfParameters rf;           /**< RfParameters inputs */
        ITS::Propagation::ITM::VariabilityParameters var; /**< VariabilityParameters */
};


/** Key names for point-to-point mode input file parameters */
struct P2PInputKeys {
        static const std::string h_tx__meter;         /**< Transmitter structural height, in meters */
        static const std::string h_rx__meter;         /**< Receiver structural height, in meters*/
        static const std::string terrain_step__meter; /**< Spacing between terrain data points, in meters */
        static const std::string climate;             /**<  Radio climate of the path (enum value) */
        static const std::string N_0;                 /**< Surface refractivity, in N-units */
        static const std::string f__mhz;              /**< Frequency, in MHz */
        static const std::string pol;                 /**< Polarization (enum value) */
        static const std::string epsilon;             /**< Relative permittivity of the ground (dimensionless)*/
        static const std::string sigma;               /**< Conductivity of the ground, in siemens per meter */
        static const std::string mdvar;               /**< Mode of variability (enum value) */
        static const std::string time;                /**< Time variability (percentage) */
        static const std::string location;            /**< Location variability (percentage) */
        static const std::string situation;           /**< Situation variability (percentage) */
        static const std::string eliminate_location;  /**< Whether to eliminate location variability (boolean) */
        static const std::string eliminate_situation; /**< Whether to eliminate direct situation variability (boolean) */
}; // Constants defined in app/src/PointToPointMode.cpp


// TODO Add area mode structs 

// clang-format on