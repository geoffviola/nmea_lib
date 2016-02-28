cmake_minimum_required(VERSION 2.8.3)
project(nmea_lib)

find_package(catkin REQUIRED COMPONENTS roslint)

catkin_package()

add_definitions("-std=c++11 -Wall -Werror")

add_library(nmea_lib
	src/nmea_parser.hpp
	src/nmea_parser.cpp
	)

include_directories(src)
catkin_add_gtest(nmea_parser_utest test/nmea_parser_utest.cpp)
target_link_libraries(nmea_parser_utest nmea_lib)

roslint_cpp(src/nmea_parser.cpp test/nmea_parser_utest.cpp)

