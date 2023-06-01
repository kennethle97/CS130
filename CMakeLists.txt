cmake_minimum_required(VERSION 3.10.0)
project(projects)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory) and run CMake from there.
    You may need to remove CMakeCache.txt." )
endif()

# Turn on debug builds if we are building in a devel shell
if (CMAKE_BUILD_TYPE STREQUAL "" AND "$ENV{DEVEL_SHELL}" STREQUAL "1")
    message(STATUS "Setting debug build type by default in devel shell")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Output binaries to a sub directory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)

# Enable Boost
# Use static libraries so binaries can be deployed without a full boost install
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS system regex log_setup log)
message(STATUS "Boost version: ${Boost_VERSION}")


include_directories(include)

# Update name and srcs
# add_library(example_lib src/example_lib.cc)
add_library(server_lib src/server.cc)
add_library(session_lib src/session.cc)
add_library(config_parser_lib src/config_parser.cc)
add_library(server_main_lib src/server_main.cc)
add_library(logger_lib src/logger.cc)
add_library(http_lib
                src/http/mime_types.cpp
                )

add_library(request_handler_dispatcher_obj OBJECT src/request_handler_dispatcher.cc)
add_library(request_404_handler_factory_obj OBJECT src/request_handler/request_404_handler_factory.cc)
add_library(static_handler_factory_obj OBJECT src/request_handler/static_handler_factory.cc)
add_library(echo_handler_factory_obj OBJECT src/request_handler/echo_handler_factory.cc)
add_library(crud_handler_factory_obj OBJECT src/request_handler/crud_handler_factory.cc)
add_library(sleep_handler_factory_obj OBJECT src/request_handler/sleep_handler_factory.cc)
add_library(health_handler_factory_obj OBJECT src/request_handler/health_handler_factory.cc)
add_library(request_handler_echo_obj OBJECT src/request_handler/request_handler_echo.cc)
add_library(request_handler_static_obj OBJECT src/request_handler/request_handler_static.cc)
add_library(request_handler_404_obj OBJECT src/request_handler/request_handler_404.cc)
add_library(request_handler_crud_obj OBJECT src/request_handler/request_handler_crud.cc)
add_library(request_handler_health_obj OBJECT src/request_handler/request_handler_health.cc)
add_library(request_handler_sleep_obj OBJECT src/request_handler/request_handler_sleep.cc)

# Create request_handler_lib using object files
add_library(request_handler_lib STATIC
    $<TARGET_OBJECTS:request_handler_dispatcher_obj>
    $<TARGET_OBJECTS:request_404_handler_factory_obj>
    $<TARGET_OBJECTS:static_handler_factory_obj>
    $<TARGET_OBJECTS:echo_handler_factory_obj>
    $<TARGET_OBJECTS:crud_handler_factory_obj>
    $<TARGET_OBJECTS:sleep_handler_factory_obj>
    $<TARGET_OBJECTS:health_handler_factory_obj>
    $<TARGET_OBJECTS:sleep_handler_factory_obj>
    $<TARGET_OBJECTS:request_handler_echo_obj>
    $<TARGET_OBJECTS:request_handler_static_obj>
    $<TARGET_OBJECTS:request_handler_404_obj>
    $<TARGET_OBJECTS:request_handler_crud_obj>
    $<TARGET_OBJECTS:request_handler_health_obj>
    $<TARGET_OBJECTS:request_handler_sleep_obj>
)


# Update executable name, srcs, and deps
# add_executable(example_server src/example_server_main.cc)
# target_link_libraries(example_server example_lib Boost::system)
add_executable(server src/server_main.cc)

target_link_libraries(server server_main_lib server_lib session_lib 
                      config_parser_lib logger_lib request_handler_lib http_lib Boost::system pthread
                      Boost::regex Boost::log_setup Boost::log Boost::thread)


# Update test executable name, srcs, and deps
# add_executable(example_lib_test tests/example_lib_test.cc)
# target_link_libraries(example_lib_test example_lib gtest_main)

add_executable(sleep_handler_factory_test tests/sleep_handler_factory_test.cc)
target_link_libraries(sleep_handler_factory_test request_handler_lib http_lib config_parser_lib logger_lib gtest_main Boost::log Boost::log_setup)

add_executable(health_handler_factory_test tests/health_handler_factory_test.cc)
target_link_libraries(health_handler_factory_test request_handler_lib http_lib config_parser_lib logger_lib gtest_main Boost::log Boost::log_setup)

add_executable(request_404_handler_factory_test tests/request_404_handler_factory_test.cc)
target_link_libraries(request_404_handler_factory_test request_handler_lib http_lib config_parser_lib logger_lib gtest_main Boost::log Boost::log_setup)

add_executable(echo_handler_factory_test tests/echo_handler_factory_test.cc)
target_link_libraries(echo_handler_factory_test request_handler_lib http_lib config_parser_lib logger_lib gtest_main Boost::log Boost::log_setup)

add_executable(static_handler_factory_test tests/static_handler_factory_test.cc)
target_link_libraries(static_handler_factory_test request_handler_lib http_lib config_parser_lib logger_lib gtest_main Boost::log Boost::log_setup)

add_executable(request_handler_sleep_test tests/request_handler_sleep_test.cc)
target_link_libraries(request_handler_sleep_test request_handler_lib http_lib logger_lib gtest_main Boost::log Boost::log_setup)

add_executable(request_handler_404_test tests/request_handler_404_test.cc)
target_link_libraries(request_handler_404_test request_handler_lib http_lib logger_lib gtest_main Boost::log Boost::log_setup)

add_executable(request_handler_echo_test tests/request_handler_echo_test.cc)
target_link_libraries(request_handler_echo_test http_lib request_handler_lib logger_lib gtest_main Boost::log Boost::log_setup)

add_executable(request_handler_static_test tests/request_handler_static_test.cc)
target_link_libraries(request_handler_static_test gtest_main request_handler_lib http_lib logger_lib Boost::filesystem Boost::log Boost::log_setup )

add_executable(request_handler_dispatcher_test tests/request_handler_dispatcher_test.cc)
target_link_libraries(request_handler_dispatcher_test gtest_main request_handler_lib http_lib config_parser_lib logger_lib Boost::filesystem Boost::log Boost::log_setup )

add_executable(request_handler_crud_test tests/request_handler_crud_test.cc)
target_link_libraries(request_handler_crud_test gtest_main request_handler_lib http_lib config_parser_lib logger_lib Boost::filesystem Boost::log Boost::log_setup )

add_executable(crud_handler_factory_test tests/crud_handler_factory_test.cc)
target_link_libraries(crud_handler_factory_test gtest_main request_handler_lib http_lib config_parser_lib logger_lib Boost::filesystem Boost::log Boost::log_setup )

add_executable(request_handler_health_test tests/request_handler_health_test.cc)
target_link_libraries(request_handler_health_test gtest_main request_handler_lib http_lib config_parser_lib logger_lib Boost::filesystem Boost::log Boost::log_setup )

add_executable(reply_lib_test tests/reply_test.cc)
target_link_libraries(reply_lib_test http_lib gtest_main)

add_executable(server_lib_test tests/server_test.cc)
target_link_libraries(server_lib_test server_lib session_lib request_handler_lib http_lib  config_parser_lib logger_lib gtest_main
                      Boost::log_setup Boost::log)

add_executable(session_lib_test tests/session_test.cc)
target_link_libraries(session_lib_test server_lib session_lib request_handler_lib http_lib  config_parser_lib logger_lib gtest_main
                      Boost::log_setup Boost::log)

add_executable(config_parser_lib_test tests/config_parser_test.cc)
target_link_libraries(config_parser_lib_test config_parser_lib gtest_main)

add_executable(logger_test tests/logger_test.cc)
target_link_libraries(logger_test logger_lib gtest_main Boost::log_setup Boost::log)

# Update with test binary
# gtest_discover_tests(example_lib_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(server_lib_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(session_lib_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(config_parser_lib_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(logger_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(reply_lib_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_handler_echo_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_handler_static_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_handler_dispatcher_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_handler_404_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_handler_health_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_404_handler_factory_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(sleep_handler_factory_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(static_handler_factory_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(echo_handler_factory_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(health_handler_factory_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_handler_crud_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_handler_sleep_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(crud_handler_factory_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_handler_sleep_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)



# Update with target/test targets
# include(cmake/CodeCoverageReportConfig.cmake)
# generate_coverage_report(TARGETS example_server example_lib TESTS example_lib_test)
include(cmake/CodeCoverageReportConfig.cmake)
generate_coverage_report(TARGETS server server_lib session_lib config_parser_lib logger_lib
                         TESTS server_lib_test 
                         session_lib_test
                         config_parser_lib_test 
                         logger_test 
                         reply_lib_test 
                         request_handler_echo_test 
                         request_handler_static_test 
                         request_handler_dispatcher_test 
                         request_handler_404_test
                         request_handler_health_test 
                         health_handler_factory_test
                         request_404_handler_factory_test 
                         static_handler_factory_test 
                         echo_handler_factory_test 
                         request_handler_crud_test
                         request_handler_sleep_test 
                         sleep_handler_factory_test
                         crud_handler_factory_test
                         )


# paste local path
#          ${REPO}/build_coverage/report/index.html
# to see coverage report
add_test(NAME integration_tests COMMAND bash ${CMAKE_SOURCE_DIR}/tests/integration_tests/integration_tests.sh 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/integration_tests)