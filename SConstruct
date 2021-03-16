#build for macos
#for a release build type: 'scons' or 'scons DBG=0'
#for a debug build type: 'scons DBG=1'
# append -c to clean targets, eg, to clean debug targets: scons DBG=1 -c

#install the following libraries with brew:
#   brew install glfw
#   brew install glew

#install libnoise using the version updated with cmake (you can find it on github)

import os
import subprocess

DBG = int(ARGUMENTS.get('DBG', 0))
ARM = int(ARGUMENTS.get('ARM', 1))
WARN = int(ARGUMENTS.get('WARN', 0))

env = Environment()

if env['PLATFORM'] == 'darwin': #macos
    CLANG = int(ARGUMENTS.get('CLANG', 1))
    CXX='clang++' if CLANG==1 else "g++"
    GLFW_DIR='./glfw/'
    GLEW_DIR='./glew/'
    NOISE_DIR='./libnoise/src/'

    LIBS=['pthread']
    LINK='{} -framework OpenGL -framework Cocoa -framework IOKit'.format(CXX)

    if int(ARGUMENTS.get('W64', 0))==1:
        CXX='/usr/local/bin/x86_64-w64-mingw32-g++'
        LINK='{} -static-libgcc -static-libstdc++ -static'.format(CXX)
        LIBS=['glu32', 'gdi32', 'opengl32', 'stdc++fs']

elif env['PLATFORM'] == 'posix': #linux
    CLANG = int(ARGUMENTS.get('CLANG', 1))
    CXX='clang++' if CLANG==1 else "g++"
    GLFW_DIR='/usr/local' # must 'make install' GLFW 
    GLEW_DIR='/usr/'
    LINK='{}'.format(CXX)
    LIBS=['GLEW','glfw3','pthread', 'GL', 'X11', 'Xrandr', 'Xi', 'GLU', 'dl']
elif env['PLATFORM'] == 'win32': #Min-GW-64, TODO add MS compiler support
    CLANG = int(ARGUMENTS.get('CLANG', 0)) # 
    CXX='clang++' if CLANG==1 else "g++"
    path = ['C:\\Program Files\\mingw-w64\\x86_64-8.1.0-win32-seh-rt_v6-rev0\\mingw64\\bin']
    temp = 'C:\\Temp'
    # note:
    # download glfw-3.3.2.bin.WIN64 and glew-2.1.0 (TODO either check in or create automatic process to install)
    # renamed glfw-3.3.2.bin.WIN64/lib-*minwg* -> glfw-3.3.2.bin.WIN64/lib
    env = Environment(ENV={'PATH': path, 'TEMP': temp}, 
                      tools=['mingw'])
    GLFW_DIR=os.sep.join(['.','glfw-3.3.2.bin.WIN64'])
    GLEW_DIR=os.sep.join(['.','glew-2.1.0'])
    LINK='{}'.format(CXX)
    LIBS=['glew32s','glfw3', 'glu32', 'gdi32', 'opengl32']
elif env['PLATFORM'] == 'msys': #msys2, TODO add MS compiler support
    CLANG = int(ARGUMENTS.get('CLANG', 0)) #
    CXX='clang++' if CLANG==1 else "g++"
    path = ['/mingw64/bin/']
    temp = '/Temp/'
    # note:
    # download glfw-3.3.2.bin.WIN64 and glew-2.1.0 (TODO either check in or create automatic process to install)
    # renamed glfw-3.3.2.bin.WIN64/lib-*minwg* -> glfw-3.3.2.bin.WIN64/lib
    env = Environment(ENV={'PATH': path, 'TEMP': temp},
                      tools=['mingw'])
    GLFW_DIR=os.sep.join(['./arc','glfw-3.3.2.bin.WIN64'])
    GLEW_DIR=os.sep.join(['./arc','glew-2.1.0'])
    LINK='{} -static-libgcc -static-libstdc++ -static'.format(CXX)
    LIBS=['glew32s','glfw3', 'glu32', 'gdi32', 'opengl32']

else:
    print("Error, unsupported platform->", env['PLATFORM'])
    exit()

GLFW_INCLUDE=os.sep.join([GLFW_DIR,'include'])
GLEW_INCLUDE=os.sep.join([GLEW_DIR,'include'])

BLD = 'dbg' if DBG == 1 else 'rel'
OPT = 0 if DBG == 1 else 3

env.Append(CPPPATH = ['include']) 

CCFLAGS='-static -O{} -I {} -I {} -I {} -I {} -I {} -I {} {} -D_USE_MATH_DEFINES -DNOISE_STATIC -Wall -Wpedantic {} -g -std=c++2a -DGLEW_STATIC'.format(OPT, "./glm/", './', './include/', GLFW_INCLUDE, GLEW_INCLUDE, NOISE_DIR, '-DDBG' if DBG==1 else '', '-Werror' if int(ARGUMENTS.get('W64', 0))==0 and WARN == 0 else '')

LIBSSTATIC = Glob(os.sep.join(['lib', '*.a']))

if int(ARGUMENTS.get('W64', 0))==1:
    LIBSSTATIC = Glob(os.sep.join(['lib', '*.lib']))

VariantDir(os.sep.join(['obj', BLD]), "src", duplicate=0)
tst = env.Program(os.sep.join(['bin', BLD, 'tstGame']),
                    source=[Glob('{}/*.cpp'.format(os.sep.join(['obj', BLD]))), LIBSSTATIC],
                    CXX=CXX,
                    CCFLAGS=CCFLAGS,
                    LINK=LINK,
                    LIBS=LIBS)

Default(tst)

if int(ARGUMENTS.get('DIST', 0)) == 1:
    process1 = subprocess.Popen(shell='true', cwd='./', args="mkdir -p dist", stdout=subprocess.PIPE)
    output, error = process1.communicate()

    process1 = subprocess.Popen(shell='true', cwd='./', args="mkdir -p dist/shaders/", stdout=subprocess.PIPE)
    output, error = process1.communicate()

    process1 = subprocess.Popen(shell='true', cwd='./', args="mkdir -p dist/src/assets/", stdout=subprocess.PIPE)
    output, error = process1.communicate()

    if int(ARGUMENTS.get('W64', 0)) == 1:
        process1 = subprocess.Popen(shell='true', cwd='./', args="cp ./bin/rel/tstGame.exe ./dist/", stdout=subprocess.PIPE)
        output, error = process1.communicate()

        process1 = subprocess.Popen(shell='true', cwd='./', args="cp -r shaders/ ./dist/shaders/", stdout=subprocess.PIPE)
        output, error = process1.communicate()

        process1 = subprocess.Popen(shell='true', cwd='./', args="cp -r src/assets/ ./dist/src/assets/", stdout=subprocess.PIPE)
        output, error = process1.communicate()