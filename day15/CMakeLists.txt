parse_day_name(${CMAKE_CURRENT_SOURCE_DIR} DAY_NAME)

set(PART1_EXE ${DAY_NAME}p1)
add_task(${PART1_EXE} src/Part1.cpp)

set(PART2_EXE ${DAY_NAME}p2)
add_task(${PART2_EXE} src/Part2.cpp)

string(REGEX MATCH "day[0-9]+" CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_test_module(
    NAME AddRangeTest
    SOURCES ut/AddRangeTest.cpp
    OUTPUT_DIR ${CURRENT_DIR}
    INCLUDES include
)

add_test_module(
    NAME CountElementsTest
    SOURCES ut/CountElementsTest.cpp
    OUTPUT_DIR ${CURRENT_DIR}
    INCLUDES include
)

add_test_module(
    NAME GetRangeTest
    SOURCES ut/GetRangeTest.cpp
    OUTPUT_DIR ${CURRENT_DIR}
    INCLUDES include
)
