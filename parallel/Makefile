CC := g++ -pthread -std=c++11

all : a.out

a.out : main.o
		$(CC) main.o -o PhonePricePrediction.out

main.o: main.cpp
	$(CC) -c main.cpp -o main.o

.PHONY : clean
clean:
	rm -r *.o
	rm -r *.out
