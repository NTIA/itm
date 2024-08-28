#include "TestUtils.h"

class TestInverseCCDF: public testing::Test {
protected:
    double q;
    double Q_q;
    double epsilon = 4.5e-4;
};

TEST_F(TestInverseCCDF, TestSmallQ) {
    // q < 0.5
    q = 0.1;
    Q_q = InverseComplementaryCumulativeDistributionFunction(q);
    EXPECT_GT(Q_q, 0.0);
    ASSERT_NEAR(Q_q, 1.281729, epsilon);
    q = 0.49;
    Q_q = InverseComplementaryCumulativeDistributionFunction(q);
    EXPECT_GT(Q_q, 0.0);
    ASSERT_NEAR(Q_q, 0.024998, epsilon);
}

TEST_F(TestInverseCCDF, TestCutoffQ) {
    // q == 0.5
    q = 0.5;
    Q_q = InverseComplementaryCumulativeDistributionFunction(q);
    EXPECT_DOUBLE_EQ(Q_q, 2.378045045325905e-07);
    ASSERT_NEAR(Q_q, 0.000000, epsilon);
}

TEST_F(TestInverseCCDF, TestLargeQ) {
    // q > 0.5
    q = 0.51;
    Q_q = InverseComplementaryCumulativeDistributionFunction(q);
    EXPECT_LT(Q_q, 0.0);
    ASSERT_NEAR(Q_q, -0.024998, epsilon);
    q = 0.9;
    Q_q = InverseComplementaryCumulativeDistributionFunction(q);
    EXPECT_LT(Q_q, 0.0);
    ASSERT_NEAR(Q_q, -1.281729, epsilon);
}

TEST_F(TestInverseCCDF, TestSymmetry) {
    // Test some inputs whose outputs should be equal but negated
    double Q_inv;
    Q_q = InverseComplementaryCumulativeDistributionFunction(0.1);
    Q_inv = InverseComplementaryCumulativeDistributionFunction(0.9);
    ASSERT_NEAR(Q_q + Q_inv, 0.000000, 2*epsilon);
    Q_q = InverseComplementaryCumulativeDistributionFunction(0.25);
    Q_inv = InverseComplementaryCumulativeDistributionFunction(0.75);
    ASSERT_NEAR(Q_q + Q_inv, 0.000000, 2 * epsilon);
    Q_q = InverseComplementaryCumulativeDistributionFunction(0.45);
    Q_inv = InverseComplementaryCumulativeDistributionFunction(0.55);
    ASSERT_NEAR(Q_q + Q_inv, 0.000000, 2 * epsilon);
}
