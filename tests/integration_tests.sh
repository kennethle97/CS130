#!/bin/bash

#kills all server
killall server 

# Defining paths to server binary and config
PATH_SERVER_BIN=../build/bin/server
PATH_RESPONSE=./tests

echo "
server {
  listen   8080;
  server_name foo.com;
  root /home/ubuntu/sites/foo/;
}" > config_test

$PATH_SERVER_BIN config_test &
PID_SERVER=$!
echo "Server process ID: $PID_SERVER"
sleep 1
#Test 1 should pass as it has correct return format
echo -e "Test 1:should succeed \n "
echo -e "GET /test HTTP/1.1\nHost: www.integration_test.com\nAccept: */*\r\n\r\n" | nc 127.0.0.1 8080 > test_response2

diff -q expected_response2 test_response2

DIFF_EXIT_CODE=$?
#expected and test response should be both blank
if [ $DIFF_EXIT_CODE -eq 0 ]; then
    echo -e "Success! Expected Response = Test Response\n"
else
    echo -e "Failure. Expected Reponse != Test Response\n"
    kill -9 $PID_SERVER
    exit 1
fi
#Test 2 should fail given that the return type is \n
echo -e "Test 3:should return nothing with wrong message format\n "
#Times out after 1 second with nc -w flag
echo -e "GET /test HTTP/1.1\nHost: www.integration_test.com\nAccept: */*\n" | nc -w 1 127.0.0.1 8080 > test_response3

sleep 1

diff -q expected_response3 test_response3

DIFF_EXIT_CODE=$?

if [ $DIFF_EXIT_CODE -eq 0 ]; then
    echo -e "Success! Expected Response = Test Response\n"
else
    echo -e "Failure. Expected Reponse != Test Response\n"
    kill -9 $PID_SERVER
    exit 1
fi

echo -e "Killing Server $PID_SERVER\n"
kill -9 $PID_SERVER
echo -e "Tests Completed!\n"
exit 0






