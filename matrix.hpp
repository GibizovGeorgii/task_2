#include <vector>
#include <memory>

class Matrix {
  private:
	std::vector<std::shared_ptr<std::vector<double>>> matrix;
	int size;

  public:
	Matrix (int n);
	Matrix (const Matrix& m);

	void Swap (int i, int j);

	std::vector<double>& operator[] (int i);

	const std::vector<double>& operator[] (int i) const;
	
	int getSize() const;

	void Print ();
};
