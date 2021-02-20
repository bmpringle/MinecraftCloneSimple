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
COMPILE_LIBS = int(ARGUMENTS.get('COMPILE_LIBS', 0))

env = Environment()


if COMPILE_LIBS == 1 and env['PLATFORM'] != 'win32' and env['PLATFORM'] != 'msys': 
    command1 = 'mkdir -p ./temp/ ./TimerMapLib/ ./EventLib/ ./lib/ ./stbi/ ./BinaryTree/'
    command2 = 'git clone https://github.com/bmpringle/EventQueue.git'
    command3 = 'git clone https://github.com/bmpringle/TimerMap.git'
    command4 = 'make library' #run twice (one for each lib)
    command5 = 'mv ./TimerMap/lib/timerMapLib.a ../lib/timerMapLib.a'
    command5a = 'mv ./TimerMap/TimerMap.h ../TimerMapLib/TimerMap.h'
    command6 = 'mv ./EventQueue/lib/eventLib.a ../lib/eventLib.a'
    command6a = 'mv ./EventQueue/Event.h ../EventLib/'
    command6b = 'mv ./EventQueue/EventQueue.h ../EventLib/'
    command6c = 'mv ./EventQueue/Listener.h ../EventLib/'
    command7 = 'rm -rf temp/'
    command8 = 'git clone https://github.com/g-truc/glm'
    command9 = 'curl https://raw.githubusercontent.com/nothings/stb/master/stb_image.h -o stb_image.h'
    command10 = 'curl https://raw.githubusercontent.com/bmpringle/BinaryTree/master/BinaryNode.h -o BinaryNode.h'
    command11 = 'curl https://raw.githubusercontent.com/bmpringle/BinaryTree/master/BinaryTree.h -o BinaryTree.h'
    
    process1 = subprocess.Popen(shell='true', cwd='./', args=command1, stdout=subprocess.PIPE)
    output, error = process1.communicate()

    process2 = subprocess.Popen(shell='true', cwd='./temp/', args=command2, stdout=subprocess.PIPE)
    output, error = process2.communicate()

    process3 = subprocess.Popen(shell='true', cwd='./temp/', args=command3, stdout=subprocess.PIPE)
    output, error = process3.communicate()

    process4a = subprocess.Popen(shell='true', cwd='./temp/EventQueue/', args=command4, stdout=subprocess.PIPE)
    output, error = process4a.communicate()

    process4b = subprocess.Popen(shell='true', cwd='./temp/TimerMap/', args=command4, stdout=subprocess.PIPE)
    output, error = process4b.communicate()

    process5 = subprocess.Popen(shell='true', cwd='./temp/', args=command5, stdout=subprocess.PIPE)
    output, error = process5.communicate()

    process5a = subprocess.Popen(shell='true', cwd='./temp/', args=command5a, stdout=subprocess.PIPE)
    output, error = process5a.communicate()

    process6 = subprocess.Popen(shell='true', cwd='./temp/', args=command6, stdout=subprocess.PIPE)
    output, error = process6.communicate()

    process6a = subprocess.Popen(shell='true', cwd='./temp/', args=command6a, stdout=subprocess.PIPE)
    output, error = process6a.communicate()

    process6b = subprocess.Popen(shell='true', cwd='./temp/', args=command6b, stdout=subprocess.PIPE)
    output, error = process6b.communicate()

    process6c = subprocess.Popen(shell='true', cwd='./temp/', args=command6c, stdout=subprocess.PIPE)
    output, error = process6c.communicate()

    process7 = subprocess.Popen(shell='true', cwd='./', args=command7, stdout=subprocess.PIPE)
    output, error = process7.communicate()

    process8 = subprocess.Popen(shell='true', cwd='./', args=command8, stdout=subprocess.PIPE)
    output, error = process8.communicate()

    process9 = subprocess.Popen(shell='true', cwd='./stbi/', args=command9, stdout=subprocess.PIPE)
    output, error = process9.communicate()

    process10 = subprocess.Popen(shell='true', cwd='./BinaryTree/', args=command10, stdout=subprocess.PIPE)
    output, error = process10.communicate()

    process11 = subprocess.Popen(shell='true', cwd='./BinaryTree/', args=command11, stdout=subprocess.PIPE)
    output, error = process11.communicate()

elif COMPILE_LIBS == 1:
    print("auto-compilation not supported for non-unix platforms.")

if env['PLATFORM'] == 'darwin': #macos
    CLANG = int(ARGUMENTS.get('CLANG', 1))
    CXX='clang++' if CLANG==1 else "g++"
    HOMEBREW = '/opt/homebrew' if ARM else '/usr/local'
    GLFW_DIR='{}/Cellar/glfw/3.3.2'.format(HOMEBREW)
    GLEW_DIR='{}/Cellar/glew/2.2.0'.format(HOMEBREW)
    LIBS=['GLEW','GLFW','pthread', 'liblibnoise']
    LINK='{} -framework OpenGL -framework GLUT'.format(CXX)
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
GLFW_LIB=os.sep.join([GLFW_DIR,'lib'])
GLEW_LIB=os.sep.join([GLEW_DIR,'lib'])

BLD = 'dbg' if DBG == 1 else 'rel'
OPT = 0 if DBG == 1 else 3

CCFLAGS='-static -O{} -I {} -I {} -I {} -I {} -I {} -Wall -g -std=c++2a -DGLEW_STATIC'.format(OPT, "./glm/", './', './include/', GLFW_INCLUDE, GLEW_INCLUDE)

VariantDir(os.sep.join(['obj', BLD]), "src", duplicate=0)
tst = env.Program(os.sep.join(['bin', BLD, 'tstGame']),
                    source=[Glob('{}/*.cpp'.format(os.sep.join(['obj', BLD]))), Glob(os.sep.join(['lib', '*.a']))],
                    CXX=CXX,
                    CCFLAGS=CCFLAGS,
                    LINK=LINK,
                    LIBPATH=[GLFW_LIB,GLEW_LIB],
                    LIBS=LIBS)

Default(tst)