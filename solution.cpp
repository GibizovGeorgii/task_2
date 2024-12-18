int COUNTER = 0;
pthread_mutex_t m;

struct Matrices {
	Matrix *A;	
	Matrix *C;
	int n;
	int col;

	Matrices (Matrix& A_matrix, Matrix& C_matrix, int num) {
		A = &A_matrix;
		C = &C_matrix;
		n = num;
	}
};

void *nulizer(void *arg) {
	Matrices *args = reinterpret_cast<Matrices *>(arg);
			
        Matrix *A = args->A;
	Matrix *C = args->C;
	int n = args->n;
	int col = args->col;
	int i;

	while (1) {
		// LOCK MUTEX
		pthread_mutex_lock(&m);
		if (COUNTER < n)	{
			i = COUNTER;
			COUNTER++;
		} else {
			pthread_mutex_unlock(&m);
			return NULL;
			// END THREAD
		}	
		// UNLOCK MUTEX
		pthread_mutex_unlock(&m);
		
		if (col != i) {
			for (int j = n - 1; j >= 0; --j) {
				(*C)[i][j] -= (*C)[col][j] * (*A)[i][col]; 
			}
			for (int j = n - 1; j >= col; --j) {
				(*A)[i][j] -= (*A)[col][j] * (*A)[i][col];
			}
		}
	}
	return NULL;
}

void better_call_Gauss(int n, Matrix& A, Matrix& C, int num_threads) {

	pthread_t *threads = new pthread_t[num_threads];


	const double eps = 0.000001;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			C[i][j] = (i == j);
		}
	}

	Matrices matrices_as_args (A, C, n);

	for (int col = 0; col < n; col++) {
		int main_row = col;
 
		while ( fabs(A[main_row][col]) < eps ) {
			if (main_row == n - 1) {
				std::cout << "Wrong matrix! \n\n";
				return;
			}
			main_row++;
		}
		
		
		if (main_row != col) {
			A.Swap(col, main_row);
			C.Swap(col, main_row);
		}

		matrices_as_args.col = col;		

		for (int j = n - 1; j >= 0; --j) {
			C[col][j] /= A[col][col];					
		}

		for (int j = n - 1; j >= col; --j) {
			A[col][j] /= A[col][col];						
		}
		
		pthread_mutex_init(&m, NULL);
		for (int THREAD = 0; THREAD < num_threads; ++THREAD) {
			pthread_create(&threads[THREAD], NULL, nulizer, &matrices_as_args);
		}
		
		// synchronization - threads must be done before new step
		for (int THREAD = 0; THREAD < num_threads; ++THREAD) {
			pthread_join(threads[THREAD], NULL);
		}
		pthread_mutex_destroy(&m);
		COUNTER = 0;
	}
	delete(threads);
}
