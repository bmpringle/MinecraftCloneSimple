#note: this is a modified version of the script for release builds i'm only using until the Sconstruct file can be fixed.
default:
	mkdir -p obj
	mkdir -p bin
	cd obj; clang++ -std=c++2a -c -O2 ../src/*.cpp -I../ -I../include/ -I/usr/local/Cellar/glfw/3.3.2/include/ -I/usr/local/Cellar/glew/2.2.0/include
	clang++ ./obj/*.o ./lib/*.a -L/opt/homebrew/Cellar/glew/2.2.0/lib/ -L/opt/homebrew/Cellar/glfw/3.3.2/lib/ -lGLFW -lGLEW -pthread -framework OpenGL -framework GLUT -o ./bin/game


#old, don't use
rel-x86:
	g++ -std=c++2a ./src/*.cpp -O2 ./lib/*.a -I./ -I./include/ -I/usr/local/Cellar/glfw/3.3.2/include/ -I/usr/local/Cellar/glew/2.2.0/include -L/usr/local/Cellar/glew/2.2.0/lib/ -L/usr/local/Cellar/glfw/3.3.2/lib/ -lGLFW -lGLEW -pthread -framework OpenGL -framework GLUT -o game
dbg-x86:
	g++ -std=c++2a -g -O0 ./src/*.cpp ./lib/*.a -I./ -I./include/ -I/usr/local/Cellar/glfw/3.3.2/include/ -I/usr/local/Cellar/glew/2.2.0/include -L/usr/local/Cellar/glew/2.2.0/lib/ -L/usr/local/Cellar/glfw/3.3.2/lib/ -lGLFW -lGLEW -pthread -framework OpenGL -framework GLUT -o game
rel-arm:
	g++ -std=c++2a ./src/*.cpp -O2 ./lib/*.a -I./ -I./include/ -I/opt/homebrew/Cellar/glfw/3.3.2/include/ -I/opt/homebrew/Cellar/glew/2.2.0/include -L/opt/homebrew/Cellar/glew/2.2.0/lib/ -L/opt/homebrew/Cellar/glfw/3.3.2/lib/ -lGLFW -lGLEW -pthread -framework OpenGL -framework GLUT -o game
dbg-arm:
	g++ -std=c++2a -g -O0 ./src/*.cpp ./lib/*.a -I./ -I./include/ -I/opt/homebrew/Cellar/glfw/3.3.2/include/ -I/opt/homebrew/Cellar/glew/2.2.0/include -L/opt/homebrew/Cellar/glew/2.2.0/lib/ -L/opt/homebrew/Cellar/glfw/3.3.2/lib/ -lGLFW -lGLEW -pthread -framework OpenGL -framework GLUT -o game
