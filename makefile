rel-x86:
	g++ -std=c++2a *.cpp -O2 ./lib/*.a -I/usr/local/Cellar/glfw/3.3.2/include/ -I/usr/local/Cellar/glew/2.2.0/include -L/usr/local/Cellar/glew/2.2.0/lib/ -L/usr/local/Cellar/glfw/3.3.2/lib/ -lGLFW -lGLEW -pthread -framework OpenGL -framework GLUT -o game
dbg-x86:
	g++ -std=c++2a -g -O0 *.cpp ./lib/*.a -I/usr/local/Cellar/glfw/3.3.2/include/ -I/usr/local/Cellar/glew/2.2.0/include -L/usr/local/Cellar/glew/2.2.0/lib/ -L/usr/local/Cellar/glfw/3.3.2/lib/ -lGLFW -lGLEW -pthread -framework OpenGL -framework GLUT -o game
rel-arm:
	g++ -std=c++2a *.cpp -O2 ./lib/*.a -I/opt/homebrew/Cellar/glfw/3.3.2/include/ -I/opt/homebrew/Cellar/glew/2.2.0/include -L/opt/homebrew/Cellar/glew/2.2.0/lib/ -L/opt/homebrew/Cellar/glfw/3.3.2/lib/ -lGLFW -lGLEW -pthread -framework OpenGL -framework GLUT -o game
dbg-arm:
	g++ -std=c++2a -g -O0 *.cpp ./lib/*.a -I/opt/homebrew/Cellar/glfw/3.3.2/include/ -I/opt/homebrew/Cellar/glew/2.2.0/include -L/opt/homebrew/Cellar/glew/2.2.0/lib/ -L/opt/homebrew/Cellar/glfw/3.3.2/lib/ -lGLFW -lGLEW -pthread -framework OpenGL -framework GLUT -o game