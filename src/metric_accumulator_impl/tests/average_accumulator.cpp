#include "metric_accumulator_impl/average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {
class AverageAccumulatorTest : public ::testing::Test {
protected:
    AverageAccumulator accumulator;

    metric::MetricResult CreateIntMetric(int value, const std::string &name = "test_metric") {
        return metric::MetricResult{name, value};
    }
};

TEST_F(AverageAccumulatorTest, SingleValue) {
    accumulator.Accumulate(CreateIntMetric(10));
    accumulator.Finalize();

    EXPECT_DOUBLE_EQ(10.0, accumulator.Get());
}

TEST_F(AverageAccumulatorTest, MultipleValues) {
    accumulator.Accumulate(CreateIntMetric(10));
    accumulator.Accumulate(CreateIntMetric(20));
    accumulator.Accumulate(CreateIntMetric(30));
    accumulator.Finalize();

    EXPECT_DOUBLE_EQ(20.0, accumulator.Get());
}

TEST_F(AverageAccumulatorTest, ZeroValues) {
    accumulator.Accumulate(CreateIntMetric(0));
    accumulator.Accumulate(CreateIntMetric(0));
    accumulator.Finalize();

    EXPECT_DOUBLE_EQ(0.0, accumulator.Get());
}

TEST_F(AverageAccumulatorTest, NegativeValues) {
    accumulator.Accumulate(CreateIntMetric(-10));
    accumulator.Accumulate(CreateIntMetric(10));
    accumulator.Finalize();

    EXPECT_DOUBLE_EQ(0.0, accumulator.Get());
}

TEST_F(AverageAccumulatorTest, GetBeforeFinalizeThrows) {
    accumulator.Accumulate(CreateIntMetric(10));

    EXPECT_THROW(accumulator.Get(), std::runtime_error);
}

TEST_F(AverageAccumulatorTest, ResetClearsData) {
    accumulator.Accumulate(CreateIntMetric(10));
    accumulator.Finalize();
    EXPECT_DOUBLE_EQ(10.0, accumulator.Get());

    accumulator.Reset();
    accumulator.Accumulate(CreateIntMetric(5));
    accumulator.Finalize();

    EXPECT_DOUBLE_EQ(5.0, accumulator.Get());
}

TEST_F(AverageAccumulatorTest, LargeNumbers) {
    accumulator.Accumulate(CreateIntMetric(1000000));
    accumulator.Accumulate(CreateIntMetric(2000000));
    accumulator.Finalize();

    EXPECT_DOUBLE_EQ(1500000.0, accumulator.Get());
}

TEST_F(AverageAccumulatorTest, FractionalAverage) {
    accumulator.Accumulate(CreateIntMetric(1));
    accumulator.Accumulate(CreateIntMetric(2));
    accumulator.Accumulate(CreateIntMetric(3));
    accumulator.Finalize();

    EXPECT_DOUBLE_EQ(2.0, accumulator.Get());
}

TEST_F(AverageAccumulatorTest, DifferentMetricNames) {
    accumulator.Accumulate(CreateIntMetric(10, "cyclomatic_complexity"));
    accumulator.Accumulate(CreateIntMetric(20, "lines_of_code"));
    accumulator.Finalize();

    // Имена метрик не влияют на расчет среднего
    EXPECT_DOUBLE_EQ(15.0, accumulator.Get());
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
