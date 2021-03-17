#! /usr/bin/env python3
import os
import subprocess

def execCmd(cmd, cwdOverride = './'):
    process = subprocess.Popen(shell='true', cwd=cwdOverride, args=cmd, stdout=subprocess.PIPE)
    return process.communicate()

GLFW_DIR='../../glfw/include/'
GLEW_DIR='../../glew/include/'
NOISE_DIR='../../libnoise/src/'

output, error = execCmd('mkdir -p ./src/assembly/')
output, error = execCmd('clang -std=c++2a -S ../*.cpp -I../../glm/ -I../../ -I../../include/ -I{} -I{} -I{}'.format(GLFW_DIR, GLEW_DIR, NOISE_DIR), './src/assembly/')
