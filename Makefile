all: lab1 lab2 lab3 lab4

lab1: ./src/lab1.cpp
	mpicxx -Wall -o ./bin/lab1 ./src/lab1.cpp

lab2: ./src/lab2.cpp
	mpicxx -Wall -o ./bin/lab2 ./src/lab2.cpp


lab3: ./src/lab3.cpp
	mpicxx -Wall -o ./bin/lab3 ./src/lab3.cpp


lab4: ./src/lab4.cpp
	mpicxx -Wall -o ./bin/lab4 ./src/lab4.cpp


clean:
	rm -rf ./bin/lab1 ./bin/lab2 ./bin/lab3 ./bin/lab4 
