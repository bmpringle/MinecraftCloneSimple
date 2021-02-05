#build for macos
#for a release build type: 'scons' or 'scons DBG=0'
#for a debug build type: 'scons DBG=1'
# append -c to clean targets, eg, to clean debug targets: scons DBG=1 -c

#install the following libraries with brew:
#   brew install glfw
#   brew install glew
import os

DBG = int(ARGUMENTS.get('DBG', 0))
ARM = int(ARGUMENTS.get('ARM', 1))

env = Environment()

if env['PLATFORM'] == 'darwin': #macos
    CLANG = int(ARGUMENTS.get('CLANG', 1))
    CXX='clang++' if CLANG==1 else "g++"
    HOMEBREW = '/opt/homebrew' if ARM else '/usr/local'
    GLFW_DIR='{}/Cellar/glfw/3.3.2'.format(HOMEBREW)
    GLEW_DIR='{}/Cellar/glew/2.2.0'.format(HOMEBREW)
    LIBS=['GLEW','GLFW','pthread']
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