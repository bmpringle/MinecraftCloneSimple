default:
	g++ -std=c++2a *.cpp ./lib/*.a -I/usr/local/Cellar/glfw/3.3.2/include/ -I/usr/local/Cellar/glew/2.2.0/include -L/usr/local/Cellar/glew/2.2.0/lib/ -L/usr/local/Cellar/glfw/3.3.2/lib/ -lGLFW -lGLEW -pthread -framework OpenGL -framework GLUT -o game
	