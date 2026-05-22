#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

class CountParametersMetricTest : public ::testing::Test {
protected:
    CountParametersMetric metric;
};

TEST_F(CountParametersMetricTest, NoParameters) {
    std::string ast = R"(
        (function_definition [1,0] [3,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters) [1,10] [1,12]
          body: (block [2,0] [3,0]
            (return_statement [2,4] [2,13]
              (integer) [2,11] [2,13])
          )
        )
    )";

    auto func = CreateTestFunction("no_params", ast);
    auto result = metric.Calculate(func);

    EXPECT_EQ(0, std::get<int>(result));
}

TEST_F(CountParametersMetricTest, SingleParameter) {
    std::string ast = R"(
        (function_definition [1,0] [3,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters [1,10] [1,13]
            (identifier) [1,11] [1,12]
          )
          body: (block [2,0] [3,0]
            (return_statement [2,4] [2,13]
              (identifier) [2,11] [2,12])
          )
        )
    )";

    auto func = CreateTestFunction("single_param", ast);
    auto result = metric.Calculate(func);

    EXPECT_EQ(1, std::get<int>(result));
}

TEST_F(CountParametersMetricTest, MultipleParameters) {
    std::string ast = R"(
        (function_definition [1,0] [3,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters [1,10] [1,20]
            (identifier) [1,11] [1,12]
            (identifier) [1,14] [1,15]
            (identifier) [1,17] [1,18]
          )
          body: (block [2,0] [3,0]
            (return_statement [2,4] [2,13]
              (identifier) [2,11] [2,12])
          )
        )
    )";

    auto func = CreateTestFunction("multi_params", ast);
    auto result = metric.Calculate(func);

    EXPECT_EQ(3, std::get<int>(result));
}

TEST_F(CountParametersMetricTest, ParametersWithDefaults) {
    std::string ast = R"(
        (function_definition [1,0] [3,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters [1,10] [1,25]
            (identifier) [1,11] [1,12]
            (default_parameter [1,14] [1,18]
              (identifier) [1,14] [1,15]
              (integer) [1,17] [1,18]
            )
          )
          body: (block [2,0] [3,0]
            (return_statement [2,4] [2,13]
              (identifier) [2,11] [2,12])
          )
        )
    )";

    auto func = CreateTestFunction("default_params", ast);
    auto result = metric.Calculate(func);

    EXPECT_EQ(2, std::get<int>(result));
}

TEST_F(CountParametersMetricTest, SelfParameter) {
    std::string ast = R"(
        (function_definition [1,0] [3,0]
          name: (identifier) [1,8] [1,14]
          parameters: (parameters [1,14] [1,20]
            (identifier) [1,15] [1,19]
            (identifier) [1,21] [1,22]
          )
          body: (block [2,0] [3,0]
            (return_statement [2,8] [2,17]
              (identifier) [2,15] [2,16])
          )
        )
    )";

    auto func = CreateTestFunction("method", ast, "test.py", "MyClass");
    auto result = metric.Calculate(func);

    EXPECT_EQ(2, std::get<int>(result));
}

TEST_F(CountParametersMetricTest, ManyParameters) {
    std::string ast = R"(
        (function_definition [1,0] [3,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters [1,10] [1,50]
            (identifier) [1,11] [1,12]
            (identifier) [1,14] [1,15]
            (identifier) [1,17] [1,18]
            (identifier) [1,20] [1,21]
            (identifier) [1,23] [1,24]
            (identifier) [1,26] [1,27]
          )
          body: (block [2,0] [3,0]
            (return_statement [2,4] [2,13])
          )
        )
    )";

    auto func = CreateTestFunction("many_params", ast);
    auto result = metric.Calculate(func);

    EXPECT_EQ(6, std::get<int>(result));
}

}  // namespace analyzer::metric::metric_impl
