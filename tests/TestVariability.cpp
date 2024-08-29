#include "ITS.Propagation.ITM/Enums.h"
#include "TestUtils.h"

class TestVariability: public testing::Test {
protected:
    double time;
    double location;
    double situation;
    IntermediateValues iv;
    double f__mhz;
    double d__meter;
    int climate;
    int mdvar;
    long warnings;
    double result;

    void SetUp() override {
        time = 0.3;
        location = 0.6;
        situation = 0.9;
        mdvar = BROADCAST_MODE;
        iv.h_e__meter[0] = 100.0;
        iv.h_e__meter[1] = 100.0;
        iv.delta_h__meter = 10.0;
        d__meter = 10000.0;
        iv.A_ref__db = 50.0;
        f__mhz = 3500.0;
        climate = CLIMATE__EQUATORIAL;
        warnings = 0;
    }
};

TEST_F(TestVariability, EquatorialClimate) {
    climate = CLIMATE__EQUATORIAL;
    result = Variability(
        time,
        location,
        situation,
        iv.h_e__meter,
        iv.delta_h__meter,
        f__mhz,
        d__meter,
        iv.A_ref__db,
        climate,
        mdvar,
        &warnings
    );
    EXPECT_DOUBLE_EQ(result, 62.258514685008194);
    EXPECT_EQ(warnings, 0);
}

TEST_F(TestVariability, ContinentalSubtropicalClimate) {
    climate = CLIMATE__CONTINENTAL_SUBTROPICAL;
    result = Variability(
        time,
        location,
        situation,
        iv.h_e__meter,
        iv.delta_h__meter,
        f__mhz,
        d__meter,
        iv.A_ref__db,
        climate,
        mdvar,
        &warnings
    );
    EXPECT_DOUBLE_EQ(result, 62.172817403455056);
    EXPECT_EQ(warnings, 0);
}

TEST_F(TestVariability, MaritimeSubtropicalClimate) {
    climate = CLIMATE__MARITIME_SUBTROPICAL;
    result = Variability(
        time,
        location,
        situation,
        iv.h_e__meter,
        iv.delta_h__meter,
        f__mhz,
        d__meter,
        iv.A_ref__db,
        climate,
        mdvar,
        &warnings
    );
    EXPECT_DOUBLE_EQ(result, 62.180947861894204);
    EXPECT_EQ(warnings, 0);
}

TEST_F(TestVariability, DesertClimate) {
    climate = CLIMATE__DESERT;
    result = Variability(
        time,
        location,
        situation,
        iv.h_e__meter,
        iv.delta_h__meter,
        f__mhz,
        d__meter,
        iv.A_ref__db,
        climate,
        mdvar,
        &warnings
    );
    EXPECT_DOUBLE_EQ(result, 62.285710999957999);
    EXPECT_EQ(warnings, 0);
}

TEST_F(TestVariability, ContinentalTemperateClimate) {
    climate = CLIMATE__CONTINENTAL_TEMPERATE;
    result = Variability(
        time,
        location,
        situation,
        iv.h_e__meter,
        iv.delta_h__meter,
        f__mhz,
        d__meter,
        iv.A_ref__db,
        climate,
        mdvar,
        &warnings
    );
    EXPECT_DOUBLE_EQ(result, 62.148868476830557);
    EXPECT_EQ(warnings, 0);
}

TEST_F(TestVariability, MaritimeTemperateOverLandClimate) {
    climate = CLIMATE__MARITIME_TEMPERATE_OVER_LAND;
    result = Variability(
        time,
        location,
        situation,
        iv.h_e__meter,
        iv.delta_h__meter,
        f__mhz,
        d__meter,
        iv.A_ref__db,
        climate,
        mdvar,
        &warnings
    );
    EXPECT_DOUBLE_EQ(result, 62.202217322742925);
    EXPECT_EQ(warnings, 0);
}

TEST_F(TestVariability, MaritimeTemperateOverSeaClimate) {
    climate = CLIMATE__MARITIME_TEMPERATE_OVER_SEA;
    result = Variability(
        time,
        location,
        situation,
        iv.h_e__meter,
        iv.delta_h__meter,
        f__mhz,
        d__meter,
        iv.A_ref__db,
        climate,
        mdvar,
        &warnings
    );
    EXPECT_DOUBLE_EQ(result, 62.171720116596696);
    EXPECT_EQ(warnings, 0);
}

TEST_F(TestVariability, SingleMessageMode) {
    mdvar = MDVAR__SINGLE_MESSAGE_MODE;
    result = Variability(
        time,
        location,
        situation,
        iv.h_e__meter,
        iv.delta_h__meter,
        f__mhz,
        d__meter,
        iv.A_ref__db,
        climate,
        mdvar,
        &warnings
    );
    EXPECT_DOUBLE_EQ(result, 65.983366483245064);
    EXPECT_EQ(warnings, 0);
}

TEST_F(TestVariability, AccidentalMode) {
    mdvar = MDVAR__ACCIDENTAL_MODE;
    result = Variability(
        time,
        location,
        situation,
        iv.h_e__meter,
        iv.delta_h__meter,
        f__mhz,
        d__meter,
        iv.A_ref__db,
        climate,
        mdvar,
        &warnings
    );
    EXPECT_DOUBLE_EQ(result, 65.975212768420008);
    EXPECT_EQ(warnings, 0);
}

TEST_F(TestVariability, MobileMode) {
    mdvar = MDVAR__MOBILE_MODE;
    result = Variability(
        time,
        location,
        situation,
        iv.h_e__meter,
        iv.delta_h__meter,
        f__mhz,
        d__meter,
        iv.A_ref__db,
        climate,
        mdvar,
        &warnings
    );
    EXPECT_DOUBLE_EQ(result, 54.938699908329184);
    EXPECT_EQ(warnings, 0);
}

TEST_F(TestVariability, BroadcastMode) {
    mdvar = MDVAR__BROADCAST_MODE;
    result = Variability(
        time,
        location,
        situation,
        iv.h_e__meter,
        iv.delta_h__meter,
        f__mhz,
        d__meter,
        iv.A_ref__db,
        climate,
        mdvar,
        &warnings
    );
    EXPECT_DOUBLE_EQ(result, 62.258514685008194);
    EXPECT_EQ(warnings, 0);
}

TEST_F(TestVariability, EliminateLocationVariability) {
    mdvar += 10;
    result = Variability(
        time,
        location,
        situation,
        iv.h_e__meter,
        iv.delta_h__meter,
        f__mhz,
        d__meter,
        iv.A_ref__db,
        climate,
        mdvar,
        &warnings
    );
    EXPECT_DOUBLE_EQ(result, 59.833829899214862);
    EXPECT_EQ(warnings, 0);
}

TEST_F(TestVariability, EliminateSituationVariability) {
    mdvar += 20;
    result = Variability(
        time,
        location,
        situation,
        iv.h_e__meter,
        iv.delta_h__meter,
        f__mhz,
        d__meter,
        iv.A_ref__db,
        climate,
        mdvar,
        &warnings
    );
    EXPECT_DOUBLE_EQ(result, 53.043626273213889);
    EXPECT_EQ(warnings, 0);
}
