cmake_minimum_required(VERSION 2.6)

project(decaf-mips)

# Setup testing
enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall --pedantic -std=c++0x")

find_package(FLEX REQUIRED)

find_package(Boost REQUIRED)

include_directories(${FLEX_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_SOURCE_DIR})

# Build preprocessor
flex_target(dpp_flex dpp.l ${CMAKE_CURRENT_BINARY_DIR}/dpp.yy.cpp)
add_executable(dpp
               dppmain.cc
	       errors.cc
               utility.cc
               ${FLEX_dpp_flex_OUTPUTS})
add_dependencies(dpp dpp_flex)
target_link_libraries(dpp ${FLEX_LIBRARIES}
                          ${Boost_LIBRARIES})

# Build compiler
flex_target(dcc_flex scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cpp)
add_executable(dcc
               main.cc
               errors.cc
               utility.cc
               ${FLEX_dcc_flex_OUTPUTS})
add_dependencies(dcc dcc_flex dpp)
target_link_libraries(dcc ${FLEX_LIBRARIES}
                          ${Boost_LIBRARIES})

# Copy solution binaries to output directory
add_subdirectory(solution)

# Copy test fragments to output directory
add_subdirectory(samples)
