#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>

typedef std::vector<std::vector<double>> Matrix;

Matrix multMatrix(const Matrix& a, const Matrix& b);

class Vector {
    private:
        double x, y, z;
    public:
        Vector(double x, double y, double z);
        double getX() const;
        double getY() const;
        double getZ() const;
        double magnitude() const;
        double dotProduct(const Vector& vec) const;
        double angle(const Vector& vec) const;
};

class Point {
    private:
        double x, y, z;
    public:
        Point(double x, double y, double z);
        double getX() const;
        double getY() const;
        double getZ() const;
};

class Transformation {
    private:
        Matrix matrix;
        void loadRotationX(double angle); // radians
        void loadRotationY(double angle); // radians
        void loadRotationZ(double angle); // radians
    public:
        Transformation();
        void loadTranslationTransform(double tx, double ty, double tz);
        void loadRotationTransform(const Vector& v, const Point& p, double angle);
        double* intoFlatMatrix();
        void printCurrentMatrix();
};

#endif