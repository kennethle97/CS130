# CMake generated Testfile for 
# Source directory: /root/CS130/Team-404
# Build directory: /root/CS130/Team-404/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
include("/root/CS130/Team-404/build/server_lib_test[1]_include.cmake")
include("/root/CS130/Team-404/build/session_lib_test[1]_include.cmake")
include("/root/CS130/Team-404/build/config_parser_lib_test[1]_include.cmake")
include("/root/CS130/Team-404/build/logger_test[1]_include.cmake")
add_test(integration_tests "bash" "/root/CS130/Team-404/tests/integration_tests.sh")
set_tests_properties(integration_tests PROPERTIES  WORKING_DIRECTORY "/root/CS130/Team-404/tests" _BACKTRACE_TRIPLES "/root/CS130/Team-404/CMakeLists.txt;100;add_test;/root/CS130/Team-404/CMakeLists.txt;0;")
subdirs("googletest")
