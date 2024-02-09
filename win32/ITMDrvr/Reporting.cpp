#include "Reporting.h"
#include "Common.h"
#include "Labels.h"
#include "Enums.h"
#include "Errors.h"
#include "Warnings.h"

bool need_buffer = false;

/*=============================================================================
 |
 |  Description:  Print text messages corresponding to any warning flags
 |
 |        Input:  fp        - File pointer
 |                warnings  - Warning flags
 |
 |       Output:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void PrintWarningMessages(FILE* fp, long warnings) {
    if (warnings == 0) {
        PrintLabel(fp, LBL__NO_WARNINGS);
    }
    else {
        if (warnings & WARN__TX_TERMINAL_HEIGHT) PrintWarningHelper(fp, LBL__WARN_TX_TERMINAL_HEIGHT);
        if (warnings & WARN__RX_TERMINAL_HEIGHT) PrintWarningHelper(fp, LBL__WARN_RX_TERMINAL_HEIGHT);
        if (warnings & WARN__FREQUENCY) PrintWarningHelper(fp, LBL__WARN_FREQUENCY);
        if (warnings & WARN__PATH_DISTANCE_TOO_BIG_1) PrintWarningHelper(fp, LBL__WARN_PATH_DISTANCE_TOO_BIG_1);
        if (warnings & WARN__PATH_DISTANCE_TOO_BIG_2) PrintWarningHelper(fp, LBL__WARN_PATH_DISTANCE_TOO_BIG_2);
        if (warnings & WARN__PATH_DISTANCE_TOO_SMALL_1) PrintWarningHelper(fp, LBL__WARN_PATH_DISTANCE_TOO_SMALL_1);
        if (warnings & WARN__PATH_DISTANCE_TOO_SMALL_2) PrintWarningHelper(fp, LBL__WARN_PATH_DISTANCE_TOO_SMALL_2);
        if (warnings & WARN__TX_HORIZON_ANGLE) PrintWarningHelper(fp, LBL__WARN_TX_HORIZON_ANGLE);
        if (warnings & WARN__RX_HORIZON_ANGLE) PrintWarningHelper(fp, LBL__WARN_RX_HORIZON_ANGLE);
        if (warnings & WARN__TX_HORIZON_DISTANCE_1) PrintWarningHelper(fp, LBL__WARN_TX_HORIZON_DISTANCE_1);
        if (warnings & WARN__RX_HORIZON_DISTANCE_1) PrintWarningHelper(fp, LBL__WARN_RX_HORIZON_DISTANCE_1);
        if (warnings & WARN__TX_HORIZON_DISTANCE_2) PrintWarningHelper(fp, LBL__WARN_TX_HORIZON_DISTANCE_2);
        if (warnings & WARN__RX_HORIZON_DISTANCE_2) PrintWarningHelper(fp, LBL__WARN_RX_HORIZON_DISTANCE_2);
        if (warnings & WARN__EXTREME_VARIABILITIES) PrintWarningHelper(fp, LBL__WARN_EXTREME_VARIABILITIES);
        if (warnings & WARN__SURFACE_REFRACTIVITY) PrintWarningHelper(fp, LBL__WARN_SURFACE_REFRACTIVITY);
    }
}

/*=============================================================================
 |
 |  Description:  Formatting helper function for the printing text messages 
 |                corresponding to any warning flags
 |
 |        Input:  fp        - File pointer
 |                lbl       - Warning flag text message
 |
 |       Output:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void PrintWarningHelper(FILE* fp, const char* lbl) {
    if (need_buffer) {
        fprintf_s(fp, "                                      ");
    }
    need_buffer = true;

    PrintLabel(fp, lbl);
}

/*=============================================================================
 |
 |  Description:  Print text message corresponding to radio climate value
 |
 |        Input:  fp        - File pointer
 |                climate   - Radio climate
 |
 |       Output:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void PrintClimateLabel(FILE* fp, int climate) {
    switch (climate) {
    case CLIMATE__EQUATORIAL:
        PrintLabel(fp, LBL__CLIMATE_EQUATORIAL);
        break;
    case CLIMATE__CONTINENTAL_SUBTROPICAL:
        PrintLabel(fp, LBL__CLIMATE_CONTINENTAL_SUBTROPICAL);
        break;
    case CLIMATE__MARITIME_SUBTROPICAL:
        PrintLabel(fp, LBL__CLIMATE_MARITIME_SUBTROPICAL);
        break;
    case CLIMATE__DESERT:
        PrintLabel(fp, LBL__CLIMATE_DESERT);
        break;
    case CLIMATE__CONTINENTAL_TEMPERATE:
        PrintLabel(fp, LBL__CLIMATE_CONTINENTAL_TEMPERATE);
        break;
    case CLIMATE__MARITIME_TEMPERATE_OVER_LAND:
        PrintLabel(fp, LBL__CLIMATE_MARITIME_TEMPERATE_OVER_LAND);
        break;
    case CLIMATE__MARITIME_TEMPERATE_OVER_SEA:
        PrintLabel(fp, LBL__CLIMATE_MARITIME_TEMPERATE_OVER_SEA);
        break;
    default:
        PrintLabel(fp, LBL__ERROR_INVALID_VALUE);
        break;
    }
}

/*=============================================================================
 |
 |  Description:  Print text message corresponding to TX siting criteria value
 |
 |        Input:  fp                    - File pointer
 |                tx_siting_criteria    - TX siting criteria
 |
 |       Output:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void PrintSitingCriteriaLabel(FILE* fp, int tx_siting_criteria) {
    switch (tx_siting_criteria)
    {
    case SITING_CRITERIA__RANDOM:
        PrintLabel(fp, LBL__SITING_CRITERIA_RANDOM);
        break;
    case SITING_CRITERIA__CAREFUL:
        PrintLabel(fp, LBL__SITING_CRITERIA_CAREFUL);
        break;
    case SITING_CRITERIA__VERY_CAREFUL:
        PrintLabel(fp, LBL__SITING_CRITERIA_VERY_CAREFUL);
        break;
    default:
        PrintLabel(fp, LBL__ERROR_INVALID_VALUE);
        break;
    }
}

/*=============================================================================
 |
 |  Description:  Print text message corresponding to polarization value
 |
 |        Input:  fp        - File pointer
 |                pol       - Polarization
 |
 |       Output:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void PrintPolarizationLabel(FILE* fp, int pol) {
    switch (pol) {
    case POLARIZATION__HORIZONTAL:
        PrintLabel(fp, LBL__POLARIZATION_HORIZONTAL);
        break;
    case POLARIZATION__VERTICAL:
        PrintLabel(fp, LBL__POLARIZATION_VERTICAL);
        break;
    default:
        PrintLabel(fp, LBL__ERROR_INVALID_VALUE);
        break;
    }
}

/*=============================================================================
 |
 |  Description:  Print text message corresponding to mdvar value
 |
 |        Input:  fp        - File pointer
 |                mdvar     - Mode of variability
 |
 |       Output:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void PrintMdvarLabel(FILE* fp, int mdvar) {
    if (mdvar >= 20) {
        mdvar -= 20;
    }

    if (mdvar >= 10) {
        mdvar -= 10;
    }

    switch (mdvar) {
    case MDVAR__SINGLE_MESSAGE_MODE:
        PrintLabel(fp, LBL__MDVAR_SINGLE_MESSAGE_MODE);
        break;
    case MDVAR__ACCIDENTAL_MODE:
        PrintLabel(fp, LBL__MDVAR_ACCIDENTAL_MODE);
        break;
    case MDVAR__MOBILE_MODE:
        PrintLabel(fp, LBL__MDVAR_MOBILE_MODE);
        break;
    case MDVAR__BROADCAST_MODE:
        PrintLabel(fp, LBL__MDVAR_BROADCAST_MODE);
        break;
    default:
        PrintLabel(fp, LBL__ERROR_INVALID_VALUE);
        break;
    }
}

/*=============================================================================
 |
 |  Description:  Print text message corresponding error codes
 |
 |        Input:  fp        - File pointer
 |                err       - Error code
 |
 |       Output:  [void]
 |
 |      Returns:  [void]
 |
 *===========================================================================*/
void PrintErrorMsgLabel(FILE* fp, int err) {
    switch (err)
    {
    case SUCCESS:
        PrintLabel(fp, LBL__SUCCESS);
        break;
    case SUCCESS_WITH_WARNINGS:
        PrintLabel(fp, LBL__SUCCESS_WITH_WARNINGS);
        break;
    case ERROR__TX_TERMINAL_HEIGHT:
        PrintLabel(fp, LBL__ERROR_TX_TERMINAL_HEIGHT);
        break;
    case ERROR__RX_TERMINAL_HEIGHT:
        PrintLabel(fp, LBL__ERROR_RX_TERMINAL_HEIGHT);
        break;
    case ERROR__INVALID_RADIO_CLIMATE:
        PrintLabel(fp, LBL__ERROR_INVALID_RADIO_CLIMATE);
        break;
    case ERROR__INVALID_TIME:
        PrintLabel(fp, LBL__ERROR_INVALID_TIME);
        break;
    case ERROR__INVALID_LOCATION:
        PrintLabel(fp, LBL__ERROR_INVALID_LOCATION);
        break;
    case ERROR__INVALID_SITUATION:
        PrintLabel(fp, LBL__ERROR_INVALID_SITUATION);
        break;
    case ERROR__INVALID_CONFIDENCE:
        PrintLabel(fp, LBL__ERROR_INVALID_CONFIDENCE);
        break;
    case ERROR__INVALID_RELIABILITY:
        PrintLabel(fp, LBL__ERROR_INVALID_RELIABILITY);
        break;
    case ERROR__REFRACTIVITY:
        PrintLabel(fp, LBL__ERROR_REFRACTIVITY);
        break;
    case ERROR__FREQUENCY:
        PrintLabel(fp, LBL__ERROR_FREQUENCY);
        break;
    case ERROR__POLARIZATION:
        PrintLabel(fp, LBL__ERROR_POLARIZATION);
        break;
    case ERROR__EPSILON:
        PrintLabel(fp, LBL__ERROR_EPSILON);
        break;
    case ERROR__SIGMA:
        PrintLabel(fp, LBL__ERROR_SIGMA);
        break;
    case ERROR__GROUND_IMPEDANCE:
        PrintLabel(fp, LBL__ERROR_GROUND_IMPEDANCE);
        break;
    case ERROR__MDVAR:
        PrintLabel(fp, LBL__ERROR_MDVAR);
        break;
    case ERROR__EFFECTIVE_EARTH:
        PrintLabel(fp, LBL__ERROR_EFFECTIVE_EARTH);
        break;
    case ERROR__PATH_DISTANCE:
        PrintLabel(fp, LBL__ERROR_PATH_DISTANCE);
        break;
    case ERROR__DELTA_H:
        PrintLabel(fp, LBL__ERROR_DELTA_H);
        break;
    case ERROR__TX_SITING_CRITERIA:
        PrintLabel(fp, LBL__ERROR_TX_SITING_CRITERIA);
        break;
    case ERROR__RX_SITING_CRITERIA:
        PrintLabel(fp, LBL__ERROR_RX_SITING_CRITERIA);
        break;
    case ERROR__SURFACE_REFRACTIVITY_SMALL:
        PrintLabel(fp, LBL__ERROR_SURFACE_REFRACTIVITY_SMALL);
        break;
    case ERROR__SURFACE_REFRACTIVITY_LARGE:
        PrintLabel(fp, LBL__ERROR_SURFACE_REFRACTIVITY_LARGE);
        break;
    }
}

/*=============================================================================
|
|  Description:  Helper function to standardize printing of text labels to file
|
|        Input:  fp        - File pointer
|                lbl       - Text message
|
|       Output:  [void]
|
|      Returns:  [void]
|
*===========================================================================*/
void PrintLabel(FILE* fp, const char* lbl) {
    fprintf_s(fp, "[%s]\n", lbl);
}