all: $(wildcard *.c)
	gcc $(wildcard *.c) -o main

clean:
	rm main 
run: all
	./main
