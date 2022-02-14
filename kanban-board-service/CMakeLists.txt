cmake_minimum_required(VERSION 3.16.0)
project(Reminder VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
include(CMakePrintHelpers)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.55 COMPONENTS system thread REQUIRED)

add_subdirectory(extern/crowcpp)
add_subdirectory(extern/rapidjson)
add_subdirectory(extern/sqlite)

add_subdirectory(src)

target_link_libraries(Service crow rapidjson sqlite3)
target_compile_definitions(Service PUBLIC "$<$<CONFIG:RELEASE>:RELEASE_SERVICE>")

if(WIN32)
  target_compile_options(Service PRIVATE -DBOOST_ERROR_CODE_HEADER_ONLY)
endif()


