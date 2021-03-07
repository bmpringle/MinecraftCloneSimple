import os
import subprocess

process = subprocess.Popen(shell='true', cwd='./', args='git submodule update --init --recursive', stdout=subprocess.PIPE)
output, error = process.communicate()

process = subprocess.Popen(shell='true', cwd='./', args='mkdir -p lib', stdout=subprocess.PIPE)
output, error = process.communicate()

process = subprocess.Popen(shell='true', cwd='./EventQueue/', args='make library', stdout=subprocess.PIPE)
output, error = process.communicate()

process = subprocess.Popen(shell='true', cwd='./EventQueue/lib/', args='mv ./eventLib.a ../../lib/eventLib.a', stdout=subprocess.PIPE)
output, error = process.communicate()

process = subprocess.Popen(shell='true', cwd='./TimerMap/', args='make library', stdout=subprocess.PIPE)
output, error = process.communicate()

process = subprocess.Popen(shell='true', cwd='./TimerMap/lib/', args='mv ./timerMapLib.a ../../lib/timerMapLib.a', stdout=subprocess.PIPE)
output, error = process.communicate()

try:
    process = subprocess.Popen(shell='true', cwd='./glfw/', args='cmake -DBUILD_SHARED_LIBS=OFF .', stdout=subprocess.PIPE)
    output, error = process.communicate()

    process = subprocess.Popen(shell='true', cwd='./glfw/', args='make', stdout=subprocess.PIPE)
    output, error = process.communicate()

    process = subprocess.Popen(shell='true', cwd='./glfw/', args='mv ./src/libglfw3.a ../lib/', stdout=subprocess.PIPE)
    output, error = process.communicate()
except FileNotFoundError:
    print("Error, cmake needed to build glfw")
    #cmake needed for glfw

process = subprocess.Popen(shell='true', cwd='./glew/auto/', args='make', stdout=subprocess.PIPE)
output, error = process.communicate()

process = subprocess.Popen(shell='true', cwd='./glew/', args='make', stdout=subprocess.PIPE)
output, error = process.communicate()

process = subprocess.Popen(shell='true', cwd='./glew/', args='mv ./lib/libGLEW.a ../lib/', stdout=subprocess.PIPE)
output, error = process.communicate()

process = subprocess.Popen(shell='true', cwd='./libnoise/', args='mkdir -p build', stdout=subprocess.PIPE)
output, error = process.communicate()

process = subprocess.Popen(shell='true', cwd='./libnoise/build/', args='cmake ..', stdout=subprocess.PIPE)
output, error = process.communicate()

process = subprocess.Popen(shell='true', cwd='./libnoise/build/', args='make', stdout=subprocess.PIPE)
output, error = process.communicate()

process = subprocess.Popen(shell='true', cwd='./libnoise/build/', args='mv ./src/libnoise.a ../../lib/', stdout=subprocess.PIPE)
output, error = process.communicate()