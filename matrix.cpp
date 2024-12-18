#include "matrix.hpp"
#include <iostream>

Matrix::Matrix (int n) {
	size = n;
	matrix.resize(size);
	for (int i = 0; i < size; ++i) {
		matrix[i] = std::make_shared<std::vector<double>>(size);
	}
}

Matrix::Matrix (const Matrix& m) {
	size = m.size;
	matrix.resize(size);
	for (int i = 0; i < size; ++i) {
		matrix[i] = std::make_shared<std::vector<double>>(size);
	}
	for (int i = 0; i < size; ++i) {
		*(matrix[i]) = *(m.matrix[i]);
	}	
}

void Matrix::Swap (int i, int j) {
	std::swap(matrix[i], matrix[j]);
}

std::vector<double>& Matrix::operator[] (int i) {
	return *(matrix[i]);
}

const std::vector<double>& Matrix::operator[] (int i) const {
	return *(matrix[i]);
}	

int Matrix::getSize() const {
	return size;
}

void Matrix::Print () {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			std::cout << (*(matrix[i]))[j] << " ";
		}
		std::cout << "\n";
	}		
}
