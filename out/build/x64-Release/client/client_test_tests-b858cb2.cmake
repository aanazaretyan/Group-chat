add_test( [==[Something works]==] C:/Programming/Group-chat/out/build/x64-Release/bin/client_test.exe [==[--test-case=Something works]==])
set_tests_properties( [==[Something works]==] PROPERTIES WORKING_DIRECTORY C:/Programming/Group-chat/out/build/x64-Release/client)
set( client_test_TESTS [==[Something works]==])
