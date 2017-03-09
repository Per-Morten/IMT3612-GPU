cmake_minimum_required (VERSION 2.6)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
	"${CMAKE_SOURCE_DIR}/common"
)

add_library(pms_puzzle_lib
    pms_string_twist.c
)

target_link_libraries(pms_puzzle_lib
	pms_common
)
