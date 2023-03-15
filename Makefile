all: $(wildcard *.c)
	g++ $(wildcard *.cpp) -o main -l curl 

clean:
	rm main 
run: all
	./main
