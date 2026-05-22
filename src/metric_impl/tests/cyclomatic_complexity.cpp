#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

class CyclomaticComplexityMetricTest : public ::testing::Test {
protected:
    CyclomaticComplexityMetric metric;
};

TEST_F(CyclomaticComplexityMetricTest, SimpleFunction) {
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

    auto func = CreateTestFunction("simple", ast);
    auto result = metric.Calculate(func);

    EXPECT_EQ(1, std::get<int>(result));
}

TEST_F(CyclomaticComplexityMetricTest, FunctionWithIf) {
    std::string ast = R"(
        (function_definition [1,0] [5,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters) [1,10] [1,12]
          body: (block [2,0] [5,0]
            (if_statement [2,4] [4,15]
              condition: (comparison) [2,7] [2,12]
              consequence: (block [3,8] [3,16]
                (return_statement [3,8] [3,16]))
              alternative: (block [4,8] [4,15]
                (return_statement [4,8] [4,15]))
            )
          )
        )
    )";

    auto func = CreateTestFunction("with_if", ast);
    auto result = metric.Calculate(func);

    EXPECT_EQ(2, std::get<int>(result));
}

TEST_F(CyclomaticComplexityMetricTest, FunctionWithIfElif) {
    std::string ast = R"(
        (function_definition [1,0] [6,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters) [1,10] [1,12]
          body: (block [2,0] [6,0]
            (if_statement [2,4] [5,15]
              condition: (comparison) [2,7] [2,12]
              consequence: (block [3,8] [3,16]
                (return_statement [3,8] [3,16]))
            )
            (elif_statement [4,4] [5,15]
              condition: (comparison) [4,9] [4,14]
              consequence: (block [5,8] [5,15]
                (return_statement [5,8] [5,15]))
            )
          )
        )
    )";

    auto func = CreateTestFunction("with_if_elif", ast);
    auto result = metric.Calculate(func);

    EXPECT_EQ(3, std::get<int>(result));
}

TEST_F(CyclomaticComplexityMetricTest, FunctionWithForLoop) {
    std::string ast = R"(
        (function_definition [1,0] [4,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters) [1,10] [1,12]
          body: (block [2,0] [4,0]
            (for_statement [2,4] [3,20]
              left: (identifier) [2,8] [2,9]
              right: (identifier) [2,13] [2,19]
              body: (block [3,8] [3,20]
                (print_statement [3,8] [3,20]))
            )
          )
        )
    )";

    auto func = CreateTestFunction("with_for", ast);
    auto result = metric.Calculate(func);

    EXPECT_EQ(2, std::get<int>(result));
}

TEST_F(CyclomaticComplexityMetricTest, FunctionWithWhileLoop) {
    std::string ast = R"(
        (function_definition [1,0] [4,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters) [1,10] [1,12]
          body: (block [2,0] [4,0]
            (while_statement [2,4] [3,20]
              condition: (comparison) [2,10] [2,15]
              body: (block [3,8] [3,20]
                (print_statement [3,8] [3,20]))
            )
          )
        )
    )";

    auto func = CreateTestFunction("with_while", ast);
    auto result = metric.Calculate(func);

    // Сложность = 1 (базовая) + 1 (while) = 2
    EXPECT_EQ(2, std::get<int>(result));
}

TEST_F(CyclomaticComplexityMetricTest, ComplexFunction) {
    std::string ast = R"(
        (function_definition [1,0] [6,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters) [1,10] [1,12]
          body: (block [2,0] [6,0]
            (if_statement [2,4] [4,15]
              condition: (comparison) [2,7] [2,12]
              consequence: (block [3,8] [3,16]
                (for_statement [3,8] [3,16]
                  left: (identifier) [3,12] [3,13]
                  right: (identifier) [3,17] [3,23]
                  body: (block [4,12] [4,20]
                    (while_statement [4,12] [4,20]
                      condition: (comparison) [4,18] [4,23]
                      body: (block [5,16] [5,24]
                        (pass_statement) [5,16] [5,20])
                    )
                  )
                )
              )
            )
          )
        )
    )";

    auto func = CreateTestFunction("complex", ast);
    auto result = metric.Calculate(func);

    // Сложность = 1 (базовая) + 1 (if) + 1 (for) + 1 (while) = 4
    EXPECT_EQ(4, std::get<int>(result));
}

TEST_F(CyclomaticComplexityMetricTest, FunctionWithTryExcept) {
    std::string ast = R"(
        (function_definition [1,0] [5,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters) [1,10] [1,12]
          body: (block [2,0] [5,0]
            (try_statement [2,4] [5,15]
              body: (block [3,8] [3,20]
                (pass_statement) [3,8] [3,12])
              handlers: (except_clause [4,4] [4,20]
                (pass_statement) [4,8] [4,12])
            )
          )
        )
    )";

    auto func = CreateTestFunction("with_try", ast);
    auto result = metric.Calculate(func);

    EXPECT_EQ(2, std::get<int>(result));
}

}  // namespace analyzer::metric::metric_impl
