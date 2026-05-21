add_library(function
    function.cpp
)

target_include_directories(function
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

add_library(file
    file.cpp
)

target_include_directories(file
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

add_library(metric_accumulator
    metric_accumulator.cpp
    metric_accumulator_impl/average_accumulator.cpp
    metric_accumulator_impl/sum_average_accumulator.cpp
    metric_accumulator_impl/categorical_accumulator.cpp
)

target_include_directories(metric_accumulator
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

target_link_libraries(metric_accumulator
    PUBLIC
        function
        file
)

add_library(metric
    metric.cpp
    metric_impl/naming_style.cpp
    metric_impl/code_lines_count.cpp
    metric_impl/cyclomatic_complexity.cpp
    metric_impl/parameters_count.cpp
)

target_include_directories(metric
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
)

target_link_libraries(metric
    PUBLIC
        function
        metric_accumulator
)

add_library(cmd_options
    cmd_options.cpp
)

target_include_directories(cmd_options
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries(cmd_options
    PUBLIC
        Boost::program_options
)

add_subdirectory(metric_accumulator_impl)
add_subdirectory(metric_impl)