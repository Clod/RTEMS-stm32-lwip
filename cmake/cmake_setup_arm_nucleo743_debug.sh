#!/bin/sh
source_dir=$(pwd)

counter=0
if [ -z "$RTEMS_PREFIX" ]; then
	echo "RTEMS_PREFIX environmental variable (RTEMS prefix) has to be set!"
	exit 1
fi

if [ -z "$RTEMS_VERSION" ]; then
	echo "RTEMS_VERSION environmental variable not set. Will be autodetermined from tools path"
fi

if [ ! -d "rtems-cmake" ];then
	echo "rtems-cmake directory not found! Please run this script from the project root."
	exit 1
fi

builddir="build-Debug"
build_generator=""
if [ "${OS}" = "Windows_NT" ]; then
	build_generator="MinGW Makefiles"
# Could be other OS but this works for now.
else
	build_generator="Unix Makefiles"
fi

echo "Running command (without the leading +):"
set -x # Print command 
python3 rtems-cmake/cmake_build_config.py -p "${RTEMS_PREFIX}" -t "arm/nucleo-h743zi" --rtems_version "${RTEMS_VERSION}" -g "${build_generator}" \
        -b "debug" -s "${source_dir}" -l "${builddir}" --force
# Use this if commands are added which should not be printed
# set +x
