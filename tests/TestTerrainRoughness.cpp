#include "TestUtils.h"

TEST(TestTerrainRoughness, ZeroTest) {
    // Result goes to zero when delta_h input is zero
    EXPECT_DOUBLE_EQ(TerrainRoughness(1.0, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(TerrainRoughness(999.0, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(TerrainRoughness(-999.0, 0.0), 0.0);
}

TEST(TestTerrainRoughness, SimpleTest) {
    // Test against simplified expression
    EXPECT_DOUBLE_EQ(TerrainRoughness(-50e3, 1.0), 1.0 - 0.8 * exp(1.0));
    EXPECT_DOUBLE_EQ(
        TerrainRoughness(-50e3, 2.0), 2.0 * (1.0 - 0.8 * exp(1.0))
    );
}