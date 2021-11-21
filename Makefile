all:
	g++ -Wall -g -std=c++11 -static-libstdc++ -I./include -L./lib src/*.cpp src/*.c -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -o beadando.o


