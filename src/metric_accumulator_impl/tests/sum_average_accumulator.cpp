#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

class SumAverageAccumulatorTest : public ::testing::Test {
protected:
    SumAverageAccumulator accumulator;

    metric::MetricResult CreateIntMetric(int value, const std::string &name = "test_metric") {
        return metric::MetricResult{name, value};
    }
};

TEST_F(SumAverageAccumulatorTest, SingleValue) {
    accumulator.Accumulate(CreateIntMetric(10));
    accumulator.Finalize();

    auto result = accumulator.Get();
    EXPECT_EQ(10, result.sum);
    EXPECT_DOUBLE_EQ(10.0, result.average);
}

TEST_F(SumAverageAccumulatorTest, MultipleValues) {
    accumulator.Accumulate(CreateIntMetric(10));
    accumulator.Accumulate(CreateIntMetric(20));
    accumulator.Accumulate(CreateIntMetric(30));
    accumulator.Finalize();

    auto result = accumulator.Get();
    EXPECT_EQ(60, result.sum);
    EXPECT_DOUBLE_EQ(20.0, result.average);
}

TEST_F(SumAverageAccumulatorTest, ZeroValues) {
    accumulator.Accumulate(CreateIntMetric(0));
    accumulator.Accumulate(CreateIntMetric(0));
    accumulator.Finalize();

    auto result = accumulator.Get();
    EXPECT_EQ(0, result.sum);
    EXPECT_DOUBLE_EQ(0.0, result.average);
}

TEST_F(SumAverageAccumulatorTest, NegativeValues) {
    accumulator.Accumulate(CreateIntMetric(-10));
    accumulator.Accumulate(CreateIntMetric(20));
    accumulator.Accumulate(CreateIntMetric(-5));
    accumulator.Finalize();

    auto result = accumulator.Get();
    EXPECT_EQ(5, result.sum);
    EXPECT_DOUBLE_EQ(5.0 / 3.0, result.average);
}

TEST_F(SumAverageAccumulatorTest, GetBeforeFinalizeThrows) {
    accumulator.Accumulate(CreateIntMetric(10));

    EXPECT_THROW(accumulator.Get(), std::runtime_error);
}

TEST_F(SumAverageAccumulatorTest, ResetClearsData) {
    accumulator.Accumulate(CreateIntMetric(100));
    accumulator.Finalize();
    auto result1 = accumulator.Get();
    EXPECT_EQ(100, result1.sum);

    accumulator.Reset();
    accumulator.Accumulate(CreateIntMetric(50));
    accumulator.Finalize();
    auto result2 = accumulator.Get();

    EXPECT_EQ(50, result2.sum);
    EXPECT_DOUBLE_EQ(50.0, result2.average);
}

TEST_F(SumAverageAccumulatorTest, LargeNumbers) {
    accumulator.Accumulate(CreateIntMetric(1000000));
    accumulator.Accumulate(CreateIntMetric(2000000));
    accumulator.Accumulate(CreateIntMetric(3000000));
    accumulator.Finalize();

    auto result = accumulator.Get();
    EXPECT_EQ(6000000, result.sum);
    EXPECT_DOUBLE_EQ(2000000.0, result.average);
}

TEST_F(SumAverageAccumulatorTest, FractionalAverage) {
    accumulator.Accumulate(CreateIntMetric(1));
    accumulator.Accumulate(CreateIntMetric(2));
    accumulator.Accumulate(CreateIntMetric(3));
    accumulator.Finalize();

    auto result = accumulator.Get();
    EXPECT_EQ(6, result.sum);
    EXPECT_DOUBLE_EQ(2.0, result.average);
}

TEST_F(SumAverageAccumulatorTest, MultipleFinalizeCalls) {
    accumulator.Accumulate(CreateIntMetric(10));
    accumulator.Finalize();
    auto result1 = accumulator.Get();

    // Второй вызов Finalize() не должен менять результат
    accumulator.Finalize();
    auto result2 = accumulator.Get();

    EXPECT_EQ(result1.sum, result2.sum);
    EXPECT_DOUBLE_EQ(result1.average, result2.average);
}

TEST_F(SumAverageAccumulatorTest, DifferentMetricNames) {
    accumulator.Accumulate(CreateIntMetric(10, "cyclomatic_complexity"));
    accumulator.Accumulate(CreateIntMetric(20, "lines_of_code"));
    accumulator.Accumulate(CreateIntMetric(30, "parameters_count"));
    accumulator.Finalize();

    auto result = accumulator.Get();
    EXPECT_EQ(60, result.sum);
    EXPECT_DOUBLE_EQ(20.0, result.average);
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
