all : matrix.o
	g++ -g main.cpp matrix.o -o matrix_mult_g.exe
	g++ -O3 main.cpp matrix.o -o matrix_mult_o3.exe
matrix.o: matrix.cpp
	g++ -c matrix.cpp
clean:
	rm -rf *.o matrix_mult*