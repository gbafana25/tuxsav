all: $(wildcard *.c)
	g++ --std=c++17 $(wildcard *.cpp) -o main -l curl 

clean:
	rm main 
run: all
	./main
