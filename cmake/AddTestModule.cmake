function(add_test_module)
	cmake_parse_arguments(
		"ARG"
		""
		"NAME;SOURCES;OUTPUT_DIR"
		"DEPENDS;INCLUDES"
		${ARGN}
	)

	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}/${ARG_OUTPUT_DIR})

	add_executable(
		${ARG_NAME}
		${ARG_SOURCES}
	)

	target_link_libraries(
		${ARG_NAME}
		gtest
		gtest_main
		utility
		${ARG_DEPENDS}
	)

	target_include_directories(
		${ARG_NAME}
		PRIVATE ${ARG_INCLUDES}
	)

	add_test(NAME ${ARG_NAME} COMMAND ${ARG_NAME})
endfunction(add_test_module)