all : matrix.o pagerank.o
	g++ -O3 -std=c++11 -fopenmp main.cpp matrix.o pagerank.o -o matrix_mult.exe
pagerank.o: pagerank.cpp
	g++ -c -fopenmp pagerank.cpp
matrix.o: matrix.cpp
	g++ -c -fopenmp matrix.cpp
clean:
	rm -rf *.o matrix_mult*
