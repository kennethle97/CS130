if(EXISTS "/root/CS130/Team-404/build/session_lib_test[1]_tests.cmake")
  include("/root/CS130/Team-404/build/session_lib_test[1]_tests.cmake")
else()
  add_test(session_lib_test_NOT_BUILT session_lib_test_NOT_BUILT)
endif()
