#include <iostream>
#include <algorithm>

class Matrix {
    unsigned N = 0;
    unsigned M = 0;
    double *data = nullptr;
public:
    Matrix() = default;

    Matrix(unsigned n, unsigned m) : N(n), M(m), data(new double[N*M]){
        std::fill_n(data,N * M, 0.0);
        std::cout << "constructor of matrix " << N << "x" << M << std::endl;
    }

    Matrix(const std::initializer_list<std::initializer_list<int>> &list) {
        size_t max_row = 0;
        for (const auto &row: list) {
            max_row = std::max(max_row, row.size());
        }

        M = max_row;
        N = list.size();
        data = new double[N * M];

        unsigned int it = 0;
        for (const auto &row: list) {
            std::copy(row.begin(), row.end(), data+it);
            it += M;
        }


        std::cout << " constructor of " << N << "x" << M << " matrix from initializer_list \n ";

    }

    //move constr
    Matrix(Matrix && matrix):N(matrix.N), M(matrix.M), data(matrix.data){
        data = nullptr;
        N = 0;
        M = 0;
        std::cout << " move constructor" << std::endl;
    }

    //copy constr
    Matrix(Matrix & matrix){
        if(this!=&matrix){
            N = matrix.N;
            M = matrix.M;
            data = new double[N*M];
            std::copy(matrix.data, matrix.data+N*M, data);
        }

        std::cout << "copy constructor" << std::endl;
    }

    Matrix & operator=(Matrix&& matrix){
        std::swap(matrix.data, data);
        std::swap(matrix.N, N);
        std::swap(matrix.M, M);

        std::cout << "move assignment operator" << std::endl;

        return *this;
    }

    Matrix & operator=(Matrix & matrix){
        if(this!=&matrix){
            delete [] data;
            N = matrix.N;
            M = matrix.M;
            data = new double[N*M];
            std::copy(matrix.data, matrix.data+N*M, data);

        }

        std::cout << "copy assignment operator" << std::endl;

        return *this;
    }

    friend Matrix operator-(const Matrix& matrix){
        Matrix dif(matrix.N, matrix.M);
        std::transform(matrix.data, matrix.data+matrix.N*matrix.M, dif.data, [](auto &m) -> auto{return -m; });

        return dif;
    }

    double & operator()(const unsigned i, const unsigned j) const{
        return data[(i-1)*M+(j-1)];
    }

    friend std::ostream& operator<<(std::ostream & stream, Matrix & matrix){
        for(unsigned i=1; i <= matrix.N; ++i){
            for(unsigned j = 1; j<=matrix.M; ++j){
                stream << matrix.data[(i-1)*matrix.M+(j-1)] << " ";
            }
            stream << std::endl;
        }
        return stream;
    }

    ~Matrix(){
        delete[] data;
    }

};


int main(){

    Matrix m1;
    Matrix m2(3,4);
    Matrix m3({{1,2,3},{32, 23, 22},{3,234,23,44}});
    std::cout << m2(1,1) << std::endl;  // 0
    std::cout << m3(2,2) << std::endl;  // 23
    std::cout << m3;

    std::cout << "Copy semantics \n";
    Matrix m4 = m2;
    m4 = m3;

    std::cout << "Move semantics \n";
    Matrix m7 = std::move(m2);
    m4 = -m3;

    std::cout << "Copy elision \n";
    Matrix m6 = -m4;
    Matrix * pm = new Matrix(-m4);
    std::cout << m6(2,1) << std::endl; // 32
//
//    std::cout << "Inheritance \n";
//    MatrixWithLabel l0("B", 3, 4);
//    MatrixWithLabel l1({{1,2},{4,5}});
//    l1.setLabel("A");
//    MatrixWithLabel l2 = l1;
//    MatrixWithLabel l3 = std::move(l1);
//    std::cout << l2.getLabel() << " " << l3.getLabel() << std::endl;
//    // 	cout << l1.getLabel() << endl;
//
    return 0;
}