#include <iostream>
#include <algorithm>

class Matrix {
    unsigned r_num = 0;
    unsigned c_num = 0;
    double *data = nullptr;
public:
    Matrix() = default;

    Matrix(unsigned n, unsigned m) : r_num(n), c_num(m), data(new double[r_num * c_num]){
        std::fill_n(data, r_num * c_num, 0.0);
        std::cout << "constructor of matrix " << r_num << "x" << c_num << std::endl;
    }

    Matrix(const std::initializer_list<std::initializer_list<int>> &list) {
        size_t max_row = 0;
        for (const auto &row: list) {
            max_row = std::max(max_row, row.size());
        }

        c_num = max_row;
        r_num = list.size();
        data = new double[r_num * c_num];

        unsigned int it = 0;
        for (const auto &row: list) {
            std::copy(row.begin(), row.end(), data+it);
            it += c_num;
        }


        std::cout << " constructor of " << r_num << "x" << c_num << " matrix from initializer_list \n ";

    }

    //move constr
    Matrix(Matrix && matrix): r_num(matrix.r_num), c_num(matrix.c_num), data(matrix.data){
        data = nullptr;
        r_num = 0;
        c_num = 0;
        std::cout << " move constructor" << std::endl;
    }

    //copy constr
    Matrix(Matrix & matrix){
        if(this!=&matrix){
            r_num = matrix.r_num;
            c_num = matrix.c_num;
            data = new double[r_num * c_num];
            std::copy(matrix.data, matrix.data + r_num * c_num, data);
        }

        std::cout << "copy constructor" << std::endl;
    }

    Matrix & operator=(Matrix&& matrix){
        std::swap(matrix.data, data);
        std::swap(matrix.r_num, r_num);
        std::swap(matrix.c_num, c_num);

        std::cout << "move assignment operator" << std::endl;

        return *this;
    }

    Matrix & operator=(Matrix & matrix){
        if(this!=&matrix){
            delete [] data;
            r_num = matrix.r_num;
            c_num = matrix.c_num;
            data = new double[r_num * c_num];
            std::copy(matrix.data, matrix.data + r_num * c_num, data);

        }

        std::cout << "copy assignment operator" << std::endl;

        return *this;
    }

    friend Matrix operator-(const Matrix& matrix){
        Matrix dif(matrix.r_num, matrix.c_num);
        std::transform(matrix.data, matrix.data+ matrix.r_num * matrix.c_num, dif.data, [](auto &m) -> auto{return -m; });

        return dif;
    }

    double & operator()(const unsigned i, const unsigned j) const{
        return data[(i-1) * c_num + (j - 1)];
    }

    friend std::ostream& operator<<(std::ostream & stream, Matrix & matrix){
        for(unsigned i=1; i <= matrix.r_num; ++i){
            for(unsigned j = 1; j<= matrix.c_num; ++j){
                stream << matrix.data[(i-1)*matrix.c_num + (j - 1)] << " ";
            }
            stream << std::endl;
        }
        return stream;
    }

    ~Matrix(){
        delete[] data;
    }

};

class MatrixWithLabel: public Matrix{
    std::string label;
public:
    using Matrix::Matrix;

    MatrixWithLabel(const std::string &lab, unsigned r_num, unsigned c_num):Matrix(r_num, c_num){
        label = lab;
    }

    MatrixWithLabel(const std::string &lab, const std::initializer_list<std::initializer_list<int>> & list):Matrix(list){
        label = lab;
    }

    void set_label(const std::string lab){
        label = lab;
    }

    std::string get_label(){
        return label;
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

    std::cout << "Inheritance \n";
    MatrixWithLabel l0("B", 3, 4);
    MatrixWithLabel l1({{1,2},{4,5}});
    l1.set_label("A");
    MatrixWithLabel l2 = l1;
    MatrixWithLabel l3 = std::move(l1);
    std::cout << l2.get_label() << " " << l3.get_label() << std::endl;
//     	cout << l1.getLabel() << endl;
//
    return 0;
}