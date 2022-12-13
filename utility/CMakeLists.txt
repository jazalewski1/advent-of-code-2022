set(
    HEADERS
    include/utility/Arguments.hpp
    include/utility/Dbg.hpp
    include/utility/Stream.hpp
)

add_library(
    utility
    ${HEADERS}
)

target_include_directories(
    utility
    PUBLIC include
)

set_target_properties(
	utility
	PROPERTIES
	LINKER_LANGUAGE CXX
)
