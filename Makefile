all: $(wildcard *.c)
	g++ $(wildcard *.cpp) -o main

clean:
	rm main 
run: all
	./main
