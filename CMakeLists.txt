cmake_minimum_required(VERSION 2.8.3)
project(nmea_lib)

find_package(catkin REQUIRED COMPONENTS roslint)

catkin_package(
	INCLUDE_DIRS include
	LIBRARIES nmea_lib
)

set(COVERAGE "OFF" CACHE STRING "Enable coverage generation.")
if("${COVERAGE}" STREQUAL "ON")
	message("coverage enabled")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
endif()

add_definitions("-std=c++11 -Wall -Werror")

include_directories(include/nmea_lib src)

add_library(nmea_lib
	src/nmea_builder.cpp
	src/nmea_parser.cpp
)

catkin_add_gtest(nmea_parser_utest test/nmea_parser_utest.cpp)
target_link_libraries(nmea_parser_utest nmea_lib)
catkin_add_gtest(nmea_builder_utest test/nmea_builder_utest.cpp)
target_link_libraries(nmea_builder_utest nmea_lib)

roslint_cpp()

install(TARGETS nmea_lib
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

