

#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

class CategoricalAccumulatorTest : public ::testing::Test {
protected:
    CategoricalAccumulator accumulator;

    metric::MetricResult CreateStringMetric(const std::string &value, const std::string &name = "naming_style") {
        return metric::MetricResult{name, value};
    }
};

TEST_F(CategoricalAccumulatorTest, SingleCategory) {
    accumulator.Accumulate(CreateStringMetric("snake_case"));
    accumulator.Finalize();

    const auto &result = accumulator.Get();
    EXPECT_EQ(1, result.size());
    EXPECT_EQ(1, result.at("snake_case"));
}

TEST_F(CategoricalAccumulatorTest, MultipleSameCategories) {
    accumulator.Accumulate(CreateStringMetric("snake_case"));
    accumulator.Accumulate(CreateStringMetric("snake_case"));
    accumulator.Accumulate(CreateStringMetric("snake_case"));
    accumulator.Finalize();

    const auto &result = accumulator.Get();
    EXPECT_EQ(1, result.size());
    EXPECT_EQ(3, result.at("snake_case"));
}

TEST_F(CategoricalAccumulatorTest, MultipleDifferentCategories) {
    accumulator.Accumulate(CreateStringMetric("snake_case"));
    accumulator.Accumulate(CreateStringMetric("CamelCase"));
    accumulator.Accumulate(CreateStringMetric("UPPER_CASE"));
    accumulator.Finalize();

    const auto &result = accumulator.Get();
    EXPECT_EQ(3, result.size());
    EXPECT_EQ(1, result.at("snake_case"));
    EXPECT_EQ(1, result.at("CamelCase"));
    EXPECT_EQ(1, result.at("UPPER_CASE"));
}

TEST_F(CategoricalAccumulatorTest, MixedFrequencies) {
    accumulator.Accumulate(CreateStringMetric("A"));
    accumulator.Accumulate(CreateStringMetric("A"));
    accumulator.Accumulate(CreateStringMetric("B"));
    accumulator.Accumulate(CreateStringMetric("A"));
    accumulator.Accumulate(CreateStringMetric("C"));
    accumulator.Finalize();

    const auto &result = accumulator.Get();
    EXPECT_EQ(3, result.size());
    EXPECT_EQ(3, result.at("A"));
    EXPECT_EQ(1, result.at("B"));
    EXPECT_EQ(1, result.at("C"));
}

TEST_F(CategoricalAccumulatorTest, GetBeforeFinalizeThrows) {
    accumulator.Accumulate(CreateStringMetric("test"));

    EXPECT_THROW(accumulator.Get(), std::runtime_error);
}

TEST_F(CategoricalAccumulatorTest, ResetClearsData) {
    accumulator.Accumulate(CreateStringMetric("A"));
    accumulator.Finalize();
    EXPECT_EQ(1, accumulator.Get().size());

    accumulator.Reset();
    accumulator.Accumulate(CreateStringMetric("B"));
    accumulator.Finalize();

    const auto &result = accumulator.Get();
    EXPECT_EQ(1, result.size());
    EXPECT_EQ(1, result.at("B"));
    EXPECT_EQ(0, result.count("A"));
}

TEST_F(CategoricalAccumulatorTest, EmptyAfterReset) {
    accumulator.Accumulate(CreateStringMetric("A"));
    accumulator.Reset();
    accumulator.Finalize();

    EXPECT_EQ(0, accumulator.Get().size());
}

TEST_F(CategoricalAccumulatorTest, LongCategoryNames) {
    std::string long_name = "very_long_category_name_with_many_underscores";
    accumulator.Accumulate(CreateStringMetric(long_name));
    accumulator.Finalize();

    const auto &result = accumulator.Get();
    EXPECT_EQ(1, result.at(long_name));
}

TEST_F(CategoricalAccumulatorTest, NamingStyleCategories) {
    accumulator.Accumulate(CreateStringMetric("Snake Case", "naming_style"));
    accumulator.Accumulate(CreateStringMetric("Lower Case", "naming_style"));
    accumulator.Accumulate(CreateStringMetric("Snake Case", "naming_style"));
    accumulator.Finalize();

    const auto &result = accumulator.Get();
    EXPECT_EQ(2, result.size());
    EXPECT_EQ(2, result.at("Snake Case"));
    EXPECT_EQ(1, result.at("Lower Case"));
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
