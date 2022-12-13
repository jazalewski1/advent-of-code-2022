function(parse_day_name INPUT_STR RETVALUE)
    string(REGEX MATCH "day[0-9]+" DAY_NAME ${INPUT_STR})
    string(REGEX MATCH "[0-9]+" ID ${DAY_NAME})
    set(${RETVALUE} d${ID} PARENT_SCOPE)
endfunction(parse_day_name)

function(add_task EXE_NAME SOURCE_PATH)
    add_executable(${EXE_NAME} ${SOURCE_PATH})

    target_include_directories(
        ${EXE_NAME}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_link_libraries(
        ${EXE_NAME}
        utility
    )
endfunction(add_task)