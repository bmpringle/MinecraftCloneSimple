#! /usr/bin/env python3
import os
import subprocess

def execCmd(cmd, cwdOverride = './'):
    process = subprocess.Popen(shell='true', cwd=cwdOverride, args=cmd, stdout=subprocess.PIPE)
    return process.communicate()

output, error = execCmd('git submodule update --init --recursive')
output, error = execCmd('mkdir -p lib')
output, error = execCmd('make library', cwdOverride = './EventQueue/')
output, error = execCmd('mv ./eventLib.a ../../lib/eventLib.a', cwdOverride = './EventQueue/lib/')
output, error = execCmd('make library', cwdOverride = './TimerMap/')
output, error = execCmd('mv ./timerMapLib.a ../../lib/timerMapLib.a', cwdOverride = './TimerMap/lib/')

try:
    output, error = execCmd('cmake -DBUILD_SHARED_LIBS=OFF .', cwdOverride = './glfw/')
    output, error = execCmd('make', cwdOverride = './glfw/')
    output, error = execCmd('mv ./src/libglfw3.a ../lib/', cwdOverride = './glfw/')
except FileNotFoundError:
    print("Error, cmake needed to build glfw")
    #cmake needed for glfw

output, error = execCmd('make', cwdOverride = './glew/auto/')
output, error = execCmd('make', cwdOverride = './glew/')
output, error = execCmd('mv ./lib/libGLEW.a ../lib/', cwdOverride = './glew/')
output, error = execCmd('mkdir -p build', cwdOverride = './libnoise/')
output, error = execCmd('cmake ..', cwdOverride = './libnoise/build/')
output, error = execCmd('make', cwdOverride = './libnoise/build/')
output, error = execCmd('mv ./src/libnoise.a ../../lib/', cwdOverride = './libnoise/build/')