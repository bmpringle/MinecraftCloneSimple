#! /usr/bin/env python3
import os
import subprocess

def execCmd(cmd, cwdOverride = './'):
    process = subprocess.Popen(shell='true', cwd=cwdOverride, args=cmd, stdout=subprocess.PIPE)
    return process.communicate()


output, error = execCmd('git submodule update --init --recursive')

output, error = execCmd('mkdir -p lib')
output, error = execCmd('make library-cross', cwdOverride = './EventQueue/')
output, error = execCmd('mv ./eventLib.lib ../../lib/eventLib.lib', cwdOverride = './EventQueue/lib/')
output, error = execCmd('make library-cross', cwdOverride = './TimerMap/')
output, error = execCmd('mv ./timerMapLib.lib ../../lib/timerMapLib.lib', cwdOverride = './TimerMap/lib/')


try:
    output, error = execCmd('cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_TOOLCHAIN_FILE=CMake/x86_64-w64-mingw32.cmake .', cwdOverride = './glfw/')
    output, error = execCmd('make', cwdOverride = './glfw/')
    output, error = execCmd('mv ./src/libglfw3.a ../lib/libglfw3.lib', cwdOverride = './glfw/')
except FileNotFoundError:
    print("Error, cmake needed to build glfw")
    #cmake needed for glfw

output, error = execCmd('mkdir -p ./tmp/mingw/manual/', cwdOverride = './glew/')
output, error = execCmd('make SYSTEM=mingw', cwdOverride = './glew/auto/')
output, error = execCmd('x86_64-w64-mingw32-g++ -c -I../../../include/ -O2 -DGLEW_STATIC ../../../src/glew.c', cwdOverride = './glew/tmp/mingw/manual/')
output, error = execCmd('x86_64-w64-mingw32-ar rvs ./libglew32.lib ./glew.o', cwdOverride = './glew/tmp/mingw/manual/')
output, error = execCmd('mv ./libglew32.lib ../../../../lib/', cwdOverride = './glew/tmp/mingw/manual/')

output, error = execCmd('mkdir -p build', cwdOverride = './libnoise/')
output, error = execCmd('x86_64-w64-mingw32-g++ -c ../src/*.cpp ../src/model/* ../src/module/* -I../src/noise/ -DNOISE_STATIC', cwdOverride = './libnoise/build/')
output, error = execCmd('x86_64-w64-mingw32-ar rvs libnoise.lib *.o', cwdOverride = './libnoise/build/')
output, error = execCmd('mv ./libnoise.lib ../../lib/libnoise.lib', cwdOverride = './libnoise/build/')