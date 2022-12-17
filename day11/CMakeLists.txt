parse_day_name(${CMAKE_CURRENT_SOURCE_DIR} DAY_NAME)

set(PART1_EXE ${DAY_NAME}p1)
add_task(${PART1_EXE} src/Part1.cpp)

set(PART2_EXE ${DAY_NAME}p2)
add_task(${PART2_EXE} src/Part2.cpp)
