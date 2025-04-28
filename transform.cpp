#include "transform.h"
#include <cmath>
#include <iostream>

Matrix multMatrix(const Matrix& a, const Matrix& b) {
    // Ensure that the matrices are 4x4
    Matrix m = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                m[i][j] += a[i][k] * b[k][j];  // Correctly accumulate the result
            }
        }
    }

    return m;
}

void printMatrix(const Matrix& m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

Vector::Vector(double x, double y, double z) {
    this->x = x, this->y = y, this->z = z;
}
double Vector::getX() const { return x; }
double Vector::getY() const { return y; }
double Vector::getZ() const { return z; }
double Vector::magnitude() const {
    return sqrt(x*x + y*y + z*z);
}
double Vector::dotProduct(const Vector& vec) const {
    return x*vec.getX() + y*vec.getY() + z*vec.getZ();           
}
double Vector::angle(const Vector& vec) const { // angulo entre 2 vetores
    double dotProd = dotProduct(vec);
    if (dotProd == 0) {
        return M_PI/2;
    }
    double cos = dotProd / (magnitude()*vec.magnitude());
    return acos(cos);
}

Point::Point(double x, double y, double z) {
    this->x = x, this->y = y, this->z = z;
}
double Point::getX() const { return x; }
double Point::getY() const { return y; }
double Point::getZ() const { return z; }


Transformation::Transformation() {
    matrix = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
}
void Transformation::loadTranslationTransform(double tx, double ty, double tz) {
    Matrix translationMatrix = {
        {1, 0, 0, tx},
        {0, 1, 0, ty},
        {0, 0, 1, tz},
        {0, 0, 0, 1}
    };

    matrix = multMatrix(translationMatrix, matrix);
}
void Transformation::loadRotationX(double angle) {
    Matrix rotationMatrix = {
        {1, 0, 0, 0},
        {0, cos(angle), -sin(angle), 0},
        {0, sin(angle), cos(angle), 0},
        {0, 0, 0, 1}
    };

    matrix = multMatrix(rotationMatrix, matrix);
}
void Transformation::loadRotationY(double angle) {
    Matrix rotationMatrix = {
        {cos(angle), 0, sin(angle), 0},
        {0, 1, 0, 0},
        {-sin(angle), 0, cos(angle), 0},
        {0, 0, 0, 1}
    };

    matrix = multMatrix(rotationMatrix, matrix);
}
void Transformation::loadRotationZ(double angle) {
    Matrix rotationMatrix = {
        {cos(angle), -sin(angle), 0, 0},
        {sin(angle), cos(angle), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    matrix = multMatrix(rotationMatrix, matrix);
}
void Transformation::loadRotationTransform(const Vector& v, const Point& p, double angle) {
    std::cout << "\nMatriz de transformação inicial:\n";
    printCurrentMatrix();
    double tx = p.getX(), ty = p.getY(), tz = p.getZ();

    Vector projectionXZ(v.getX(), 0, v.getZ()); // projecao no plano xz
    double angleRotationY = projectionXZ.angle(Vector(1, 0, 0));
    if (projectionXZ.getZ() > 0) angleRotationY = -angleRotationY;
    double angleRotationZ = v.angle(projectionXZ);
    if (v.getY() < 0) angleRotationZ = -angleRotationZ;
    
    loadTranslationTransform(-tx, -ty, -tz);
    loadRotationY(angleRotationY);
    loadRotationZ(angleRotationZ);
    loadRotationX(angle);
    loadRotationZ(-angleRotationZ);
    loadRotationY(-angleRotationY);
    loadTranslationTransform(tx, ty, tz);

    std::cout << "\nMatriz de transformação final:\n";
    printCurrentMatrix();
}

double* Transformation::intoFlatMatrix() {
    double* flatMatrix = new double[16];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; j++) {
            flatMatrix[i*4+j] = matrix[i][j];
        }
    }
    return flatMatrix;
}

void Transformation::printCurrentMatrix() {
    printMatrix(matrix);
}
