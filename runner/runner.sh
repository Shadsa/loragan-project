#!/bin/bash

if [ $# -ge 1 ]
then
    version=$1
else
    version="v1"
fi

echo "Current version :" $version

runnerpath="./runtime/"
testpath="../test_unit/"
libpath="../libraries/"
libname="ThingSat_"

rm -r $runnerpath*
cp -r "$testpath$scriptname" "$runnerpath"
cp -r "$libpath$libname$version" "$runnerpath"
