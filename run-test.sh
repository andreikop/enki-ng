#!/bin/sh

TEST_NAME=$1

make $TEST_NAME && (cd ../tests/test-project && ../../build/$TEST_NAME)
