#!/bin/bash

runnerpath="./runtime"
testpath="../test_unit"
libpath="../libraries"
libname="ThingSat_"
mainpath="../main.ino"

if [ $# -eq 2 ]
then
    version=$2
else
    version="v1"
fi

echo "Current version :" $version
rm -r $runnerpath/*

if [ $# -eq 1 ]
then 
    scriptname=$1
    echo "Copy $scriptname"
    cp $testpath/$scriptname $runnerpath/
else 
    echo Copy $mainpath
    cp $mainpath $runnerpath/
fi

cp $libpath/$libname$version/* $runnerpath/