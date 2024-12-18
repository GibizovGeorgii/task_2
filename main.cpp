#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <pthread.h>

#include "matrix.hpp"
#include "solution.cpp"
#include "init.cpp"


class MyException {
  private:
	std::string exc;
	
  public:	
	MyException() : exc("File opening failure\n") {}
	MyException(std::string str) : exc(str) {}
		
	const std::string what() {
		return exc;
	}
};

class File {
  private:
	std::FILE* f;

  public:
	File(const std::string& name) {
		if (f = std::fopen(name.c_str(), "r"); f == nullptr) {
			throw MyException();
		}
	}

	~File() noexcept {
		std::fclose(f);
	}

	void Read (Matrix& A) const {
		int n = A.getSize();
		for (int i = 0; i != n; ++i) {
			for (int j = 0; j != n; ++j) {
				std::fscanf(f, "%lf", &A[i][j]);
			}
		}
	}
};


Matrix operator * (const Matrix& A, const Matrix& B) {
	int n = A.getSize();
	Matrix C(n);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			for (int k = 0; k < n; ++k) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	return C;
}


int main(int argc, char* argv[]) {
        int n, m, k, NUM_THREADS;
        n = argc;
	int start, end;
	std::string file_name;

	n = std::atoi(argv[1]);
	m = std::atoi(argv[2]);
	k = std::atoi(argv[3]);
        NUM_THREADS = std::atoi(argv[4]);

	Matrix A (n);
	
	if (k != 0) {
		init(A, n, k);
	} else {
                file_name = argv[5];
		try {
			File file(file_name);
			file.Read(A);
		} catch (MyException& exc) {
			std::cout << exc.what() << "\n";
			return 0;
		}
	}
	
	Matrix C (n);
	Matrix B = A;
        Matrix F (n);
        Matrix D = A;

	start = clock();	
        better_call_Gauss(n, B, C, NUM_THREADS);
        //better_call_Gauss(n, D, F);
	end = clock();

        std::cout << "time : " << 1000 * (end - start) / CLOCKS_PER_SEC << " miliseconds\n";
        Matrix E1 = A * C;
        //Matrix E2 = A * F;
        print(A, m);
        std::cout << "\n";
        print(C, m);
        std::cout << "\n";
        print(E1, m);
        //std::cout << "\n";
        //print(E2, m);

        std::cout << "\nnevyazochka vishla : " << nevyazka(E1) << "\n";
        //std::cout << "\nnevyazochka vishla : " << nevyazka(E2) << "\n";
	return 0;
}
