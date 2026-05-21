#include "metric_impl/code_lines_count.hpp"

#include <gtest/gtest.h>
#include <variant>

namespace analyzer::metric::metric_impl {

analyzer::function::Function CreateTestFunction(const std::string &name, const std::string &ast,
                                                const std::string &filename = "test.py",
                                                const std::optional<std::string> &class_name = std::nullopt) {
    return analyzer::function::Function{.filename = filename, .class_name = class_name, .name = name, .ast = ast};
}

class CodeLinesCountMetricTest : public ::testing::Test {
protected:
    CodeLinesCountMetric metric;
};

TEST_F(CodeLinesCountMetricTest, SimpleFunction) {
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

    EXPECT_GE(std::get<int>(result.value), 1);
}

TEST_F(CodeLinesCountMetricTest, FunctionWithComments) {
    std::string ast = R"(
        (function_definition [1,0] [4,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters) [1,10] [1,12]
          body: (block [2,0] [4,0]
            (comment) [2,0] [2,20]
            (return_statement [3,4] [3,13]
              (integer) [3,11] [3,13])
          )
        )
    )";

    auto func = CreateTestFunction("with_comments", ast);
    auto result = metric.Calculate(func);

    EXPECT_GE(std::get<int>(result.value), 1);
}

TEST_F(CodeLinesCountMetricTest, EmptyFunction) {
    std::string ast = R"(
        (function_definition [1,0] [3,0]
          name: (identifier) [1,4] [1,9]
          parameters: (parameters) [1,9] [1,11]
          body: (block [2,0] [3,0]
            (pass_statement) [2,4] [2,8]
          )
        )
    )";

    auto func = CreateTestFunction("empty", ast);
    auto result = metric.Calculate(func);

    EXPECT_GE(std::get<int>(result.value), 1);
}

TEST_F(CodeLinesCountMetricTest, MultilineFunction) {
    std::string ast = R"(
        (function_definition [1,0] [5,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters) [1,10] [1,12]
          body: (block [2,0] [5,0]
            (assignment [2,4] [2,12]
              left: (identifier) [2,4] [2,5]
              right: (integer) [2,8] [2,10])
            (assignment [3,4] [3,12]
              left: (identifier) [3,4] [3,5]
              right: (integer) [3,8] [3,10])
            (return_statement [4,4] [4,13]
              (identifier) [4,11] [4,12])
          )
        )
    )";

    auto func = CreateTestFunction("multiline", ast);
    auto result = metric.Calculate(func);

    EXPECT_GE(std::get<int>(result.value), 3);
}

TEST_F(CodeLinesCountMetricTest, FunctionWithOnlyComments) {
    std::string ast = R"(
        (function_definition [1,0] [4,0]
          name: (identifier) [1,4] [1,10]
          parameters: (parameters) [1,10] [1,12]
          body: (block [2,0] [4,0]
            (comment) [2,0] [2,20]
            (comment) [3,0] [3,20]
            (pass_statement) [4,4] [4,8]
          )
        )
    )";

    auto func = CreateTestFunction("comments_only", ast);
    auto result = metric.Calculate(func);

    EXPECT_GE(std::get<int>(result.value), 1);
}

}  // namespace analyzer::metric::metric_impl
