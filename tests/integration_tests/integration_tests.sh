#!/bin/bash

# NOTE: NAME ALL TEMPORARY TEST FILES AS "test_*" SO THAT THEY WILL BE DELETED AFTER THE INTEGRATION TEST ENDS

#kills all server
killall server 

# Defining paths to server binary and config
PATH_SERVER_BIN=../../build/bin/server
PATH_RESPONSE=../tests

echo "
port 8080;

location /echo EchoHandler {

}

location /static1 StaticHandler {    
    root ./public/folder1;
}

location /static2/ StaticHandler {
    root ./public/folder2;
}

location / ErrorHandler {

}
" > test_config

$PATH_SERVER_BIN test_config &
PID_SERVER=$!
echo "Server process ID: $PID_SERVER"
sleep 1
#Test 1 should pass as it has correct return format
printf "Test 1:should succeed \n "
printf "GET /echo HTTP/1.1\r\nHost: www.integration_test.com\r\nAccept: */*\r\n\r\n" | nc 127.0.0.1 8080 > test_response1

diff -q expected_response1 test_response1

DIFF_EXIT_CODE=$?
#expected and test response should be both blank
if [ $DIFF_EXIT_CODE -eq 0 ]; then
    printf "Success! Expected Response = Test Response\n"
else
    printf "Failure. Expected Reponse != Test Response\n"
    kill -9 $PID_SERVER
    exit 1
fi
#Test 2 should fail given that the return type is \n
printf "Test 2:should return bad request\n"
#Times out after 1 second with nc -w flag
printf "GET /this_does_not_exist HTTP/1.1\nHost: www.integration_test.com\nAccept: */*\r\n\r\n" | nc -w 1 127.0.0.1 8080 > test_response2

#sleep 1

diff -q expected_response2 test_response2

DIFF_EXIT_CODE=$?

if [ $DIFF_EXIT_CODE -eq 0 ]; then
    printf "Success! Expected Response = Test Response\n"
else
    printf "Failure. Expected Reponse != Test Response\n"
    kill -9 $PID_SERVER
    exit 1
fi

# Test 3 should pass as we are sending the correct request for a static file
printf "Test 3: should return response containing contents of static file"

printf "GET /static1/random.txt HTTP/1.1\r\n\r\n" | nc 127.0.0.1 8080 > test_response3

diff -q expected_response3 test_response3

DIFF_EXIT_CODE=$?
#expected and test response should be both blank
if [ $DIFF_EXIT_CODE -eq 0 ]; then
    printf "Success! Expected Response = Test Response\n"
else
    printf "Failure. Expected Reponse != Test Response\n"
    kill -9 $PID_SERVER
    exit 1
fi

printf "Killing Server $PID_SERVER\n"
kill -9 $PID_SERVER
printf "Tests Completed!\n"

# remove no longer needed test files
rm test_*
exit 0

