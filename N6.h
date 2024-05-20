#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

class SquareMatrice {
    int l_;
    double *matrice_;

public:
    SquareMatrice(int l) : l_(l) {
        matrice_ = new double[l * l];
        for (int i = 0; i < l * l; i++) {
            matrice_[i] = 0;
        }
    }

    SquareMatrice(int l, double** mat) : l_(l) {
        matrice_ = new double[l * l];
        for (int i = 0; i < l; i++) {
            for (int j = 0; j < l; j++) {
                matrice_[i * l + j] = mat[j][i];
            }
        }
    }

    SquareMatrice(std::vector<double> v) : l_(v.size()) {
        matrice_ = new double[l_ * l_];
        for (int i = 0; i < l_ * l_; i++) {
            matrice_[i] = 0;
        }
        for (size_t i = 0; i < l_; i++) {
            matrice_[i * (l_ + 1)] = v.at(i);
        }
    }

    SquareMatrice(const SquareMatrice &other): l_(other.l_) {
        matrice_ = new double[l_ * l_];
        for (size_t i = 0; i < l_ * l_; i++) {
            matrice_[i] = other.matrice_[i];
        }
    }

    SquareMatrice(SquareMatrice &&other): l_(other.l_) {
        std::swap(matrice_, other.matrice_);
    }

    SquareMatrice& operator=(SquareMatrice other) {
        std::swap(l_, other.l_);
        std::swap(matrice_, other.matrice_);
        return *this;
    }

    ~SquareMatrice() { delete[] matrice_; }

    operator double() const {
        double sum = 0;
        for (size_t i = 0; i < l_ * l_; i++) {
            sum += matrice_[i];
        }
        return sum;
    }

    operator double**() const {
        double** res = new double*[l_];
        for (size_t x = 0; x < l_; x++) {
            double* vector = new double[l_];
            for (size_t y = 0; y < l_; y++) {
                vector[y] = matrice_[y * l_ + x];
            }
            res[x] = vector;
        }
        return res;
    }

    explicit operator std::string() const {
        std::stringstream ss;
        for (size_t x = 0; x < l_; x++) {
            for (size_t y = 0; y < l_; y++) {
                ss << matrice_[y * l_ + x] << " ";
            }
            ss << "\n";
        }
        return ss.str();
    }

    SquareMatrice operator+(const SquareMatrice& other) {
        if (l_ != other.l_) {
            throw "Addition of matrices of different sizes";
        }
        SquareMatrice* res = new SquareMatrice(l_);
        for (size_t i = 0; i < l_ * l_; i++) {
            res->matrice_[i] = matrice_[i] + other.matrice_[i];
        }
        return *res;
    }

    SquareMatrice operator+(double scalar) {
        SquareMatrice* res = new SquareMatrice(l_);
        for (size_t i = 0; i < l_ * l_; i++) {
            res->matrice_[i] = matrice_[i] + scalar;
        }
        return *res;
    }

    SquareMatrice& operator+=(const SquareMatrice& other) {
        if (l_ != other.l_) {
            throw "Addition of matrices of different sizes";
        }
        for (size_t i = 0; i < l_ * l_; i++) {
            matrice_[i] += other.matrice_[i];
        }
        return *this;
    }

    SquareMatrice& operator+=(double scalar) {
        for (size_t i = 0; i < l_ * l_; i++) {
            matrice_[i] += scalar;
        }
        return *this;
    }

    SquareMatrice operator*(const SquareMatrice& other) {
        if (l_ != other.l_) {
            throw "Multiplication of matrices of different sizes";
        }
        SquareMatrice* res = new SquareMatrice(l_);
        for (size_t i = 0; i < l_ * l_; i++) {
            double sum = 0;
            int x = i % l_;
            for (size_t j = 0; j < l_; j++) {
                sum += matrice_[i - x + j] * other.matrice_[j * l_ + x];
            }
            res->matrice_[i] = sum;
        }
        return *res;
    }

    SquareMatrice operator*(double scalar) {
        SquareMatrice* res = new SquareMatrice(l_);
        for (size_t i = 0; i < l_ * l_; i++) {
            res->matrice_[i] = matrice_[i] * scalar;
        }
        return *res;
    }

    SquareMatrice& operator*=(const SquareMatrice& other) {
        if (l_ != other.l_) {
            throw "Multiplication of matrices of different sizes";
        }
        double* res = new double[l_ * l_];
        for (size_t i = 0; i < l_ * l_; i++) {
            res[i] = 0;
            int x = i % l_;
            for (size_t j = 0; j < l_; j++) {
                res[i] += matrice_[i - x + j] * other.matrice_[j * l_ + x];
            }
        }
        delete[] matrice_;
        matrice_ = res;
        return *this;
    }

    SquareMatrice& operator*=(double scalar) {
        for (size_t i = 0; i < l_ * l_; i++) {
            matrice_[i] *= scalar;
        }
        return *this;
    }

    bool operator==(const SquareMatrice& other) {
        if (l_ != other.l_) {
            return false;
        }
        for (size_t i = 0; i < l_ * l_; i++) {
            if (matrice_[i] != other.matrice_[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const SquareMatrice& other) {
        return !operator==(other);
    }

    /*double* operator[](int x){
        double* res = new double[l_]; 
        for (size_t y = 0; y < l_; y++) {
            res[y] = matrice_[y * l_ + x];
        }
        return res;
    }*/

};