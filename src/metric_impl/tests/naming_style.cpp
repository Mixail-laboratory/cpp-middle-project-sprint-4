#include "metric_impl/naming_style.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

class NamingStyleMetricTest : public ::testing::Test {
protected:
    NamingStyleMetric metric;
};

TEST_F(NamingStyleMetricTest, SnakeCaseStyle) {
    auto func = CreateTestFunction("my_function", "");
    auto result = metric.Calculate(func);

    EXPECT_EQ("Snake Case", std::get<std::string>(result));
}

TEST_F(NamingStyleMetricTest, CamelCaseStyle) {
    auto func = CreateTestFunction("myFunction", "");
    auto result = metric.Calculate(func);

    EXPECT_EQ("Camel Case", std::get<std::string>(result));
}

TEST_F(NamingStyleMetricTest, PascalCaseStyle) {
    auto func = CreateTestFunction("MyFunction", "");
    auto result = metric.Calculate(func);

    EXPECT_EQ("Pascal Case", std::get<std::string>(result));
}

TEST_F(NamingStyleMetricTest, LowerCaseStyle) {
    auto func = CreateTestFunction("myfunction", "");
    auto result = metric.Calculate(func);

    EXPECT_EQ("Lower Case", std::get<std::string>(result));
}

TEST_F(NamingStyleMetricTest, SnakeCaseWithNumbers) {
    auto func = CreateTestFunction("my_function_2", "");
    auto result = metric.Calculate(func);

    EXPECT_EQ("Snake Case", std::get<std::string>(result));
}

TEST_F(NamingStyleMetricTest, UnknownStyleWithHyphen) {
    auto func = CreateTestFunction("my-function", "");
    auto result = metric.Calculate(func);

    EXPECT_EQ("Unknown", std::get<std::string>(result));
}

TEST_F(NamingStyleMetricTest, DunderMethod) {
    auto func = CreateTestFunction("__init__", "");
    auto result = metric.Calculate(func);

    EXPECT_EQ("Snake Case", std::get<std::string>(result));
}

TEST_F(NamingStyleMetricTest, SingleLetter) {
    auto func = CreateTestFunction("f", "");
    auto result = metric.Calculate(func);

    EXPECT_EQ("Lower Case", std::get<std::string>(result));
}

TEST_F(NamingStyleMetricTest, SnakeCaseWithCapitalLetters) {
    auto func = CreateTestFunction("my_FUNCTION", "");
    auto result = metric.Calculate(func);

    EXPECT_EQ("Unknown", std::get<std::string>(result));
}

TEST_F(NamingStyleMetricTest, CamelCaseMultipleWords) {
    auto func = CreateTestFunction("myFunctionName", "");
    auto result = metric.Calculate(func);

    EXPECT_EQ("Camel Case", std::get<std::string>(result));
}

TEST_F(NamingStyleMetricTest, PascalCaseMultipleWords) {
    auto func = CreateTestFunction("MyFunctionName", "");
    auto result = metric.Calculate(func);

    EXPECT_EQ("Pascal Case", std::get<std::string>(result));
}

TEST_F(NamingStyleMetricTest, MethodInClass) {
    auto func = CreateTestFunction("my_method", "", "test.py", "MyClass");
    auto result = metric.Calculate(func);

    EXPECT_EQ("Snake Case", std::get<std::string>(result));
}

}  // namespace analyzer::metric::metric_impl
