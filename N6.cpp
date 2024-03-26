#include <iostream>
#include <sstream>
#include <vector>

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

    ~SquareMatrice() { delete matrice_; }

    operator double() const {
        int sum = 0;
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
            return *this;
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

    SquareMatrice operator+=(const SquareMatrice& other) {
        if (l_ != other.l_) {
            return *this;
        }
        for (size_t i = 0; i < l_ * l_; i++) {
            matrice_[i] += other.matrice_[i];
        }
        return *this;
    }

    SquareMatrice operator+=(double scalar) {
        for (size_t i = 0; i < l_ * l_; i++) {
            matrice_[i] += scalar;
        }
        return *this;
    }

    SquareMatrice operator*(const SquareMatrice& other) {
        if (l_ != other.l_) {
            return *this;
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

    SquareMatrice operator*=(const SquareMatrice& other) {
        if (l_ != other.l_) {
            return *this;
        }
        for (size_t i = 0; i < l_ * l_; i++) {
            double sum = 0;
            int x = i % l_;
            for (size_t j = 0; j < l_; j++) {
                sum += matrice_[i - x + j] * other.matrice_[j * l_ + x];
            }
            matrice_[i] = sum;
        }
        return *this;
    }

    SquareMatrice operator*=(double scalar) {
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

int main() {
    SquareMatrice a = *(new SquareMatrice(3));
    double ** arr = new double*[3];
    arr[0] = new double[3] {0, 1, 2};
    arr[1] = new double[3] {1, 2, 3};
    arr[2] = new double[3] {2, 3, 4};
    SquareMatrice b = *(new SquareMatrice(3, arr));
    SquareMatrice c = a + b + 1;
    a += 2;
    b *= a;
    std::cout << std::string(a) << "\n" << std::string(b) << "\n" << std::string(c) << "\n" << a[1][0] << " = " << static_cast<double**>(a)[1][0];
}