#include "TestUtils.h"

TEST(TestComputeDeltaH, InterpolateTerrainExactIndices) {
    // Specify d_start and d_stop at exact indices of the terrain profile
    // Use a small terrain profile (terrain_sample interpolates rather than samples)
    double pfl[] = {9.0, 10.0, 100.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 100.0};
    const double d_start__meter = 10.0;
    const double d_stop__meter = 90.0;
    double result = ComputeDeltaH(pfl, d_start__meter, d_stop__meter);
    EXPECT_DOUBLE_EQ(result, 130.80086741994160);
}

TEST(TestComputeDeltaH, InterpolateTerrainInexactIndices) {
    // Specify d_start and d_stop between indices of the terrain profile
    // Use a small terrain profile (terrain_sample interpolates rather than samples)
    double pfl[] = {9.0, 10.0, 100.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 100.0};
    const double d_start__meter = 15.0;
    const double d_stop__meter = 85.0;
    double result = ComputeDeltaH(pfl, d_start__meter, d_stop__meter);
    EXPECT_DOUBLE_EQ(result, 41.639683828192240);
}

TEST(TestComputeDeltaH, SampleTerrainExactIndices) {
    // Specify d_start and d_stop at exact indices of the terrain profile
    // Use a large terrain profile (terrain_sample samples rather than interpolates)
    double pfl[]
        = {33.0, 10.0, 100.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0,  2.0, 2.0,
           1.0,   1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 1.0, 1.0, 1.0,  2.0, 2.0,
           2.0,   1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 100.0};
    const double d_start__meter = 10.0;
    const double d_stop__meter = 320.0;
    double result = ComputeDeltaH(pfl, d_start__meter, d_stop__meter);
    EXPECT_DOUBLE_EQ(result, 5.5481498660992807);
}

TEST(TestComputeDeltaH, SampleTerrainInexactIndices) {
    // Specify d_start and d_stop between indices of the terrain profile
    // Use a large terrain profile (terrain_sample samples rather than interpolates)
    double pfl[]
        = {33.0, 10.0, 100.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0,  2.0, 2.0,
           1.0,   1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 1.0, 1.0, 1.0,  2.0, 2.0,
           2.0,   1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 100.0};
    const double d_start__meter = 15.0;
    const double d_stop__meter = 315.0;
    double result = ComputeDeltaH(pfl, d_start__meter, d_stop__meter);
    EXPECT_DOUBLE_EQ(result, 5.4395216034312366);
}

TEST(TestComputeDeltaH, ShortCircuit) {
    // Function should return zero if less than 2 terrain points are provided
    double pfl[] = {4.0, 10.0, 1.0, 2.0, 3.0, 4.0, 5.0};  // quite a linear hill
    const double d_start__meter = 40.0;
    const double d_stop__meter = 50.0;
    double result = ComputeDeltaH(pfl, d_start__meter, d_stop__meter);
    EXPECT_DOUBLE_EQ(result, 0.0);
}