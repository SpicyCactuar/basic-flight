#ifndef MATRIX4_H
#define MATRIX4_H

#include "Cartesian3.h"
#include "Homogeneous4.h"

#define DEG2RAD(x) (M_PI*(float)(x)/180.0)

class Matrix4 {
public:
    // stored in row-major form
    float coordinates[4][4]{};

    // default to the zero matrix
    Matrix4();

    float* operator [](int rowIndex);

    const float* operator [](int rowIndex) const;

    Matrix4 operator *(float factor) const;

    Homogeneous4 operator *(const Homogeneous4& vector) const;

    Cartesian3 operator *(const Cartesian3& vector) const;

    Matrix4 operator *(const Matrix4& other) const;

    Matrix4 transpose() const;

    static Matrix4 identity();

    static Matrix4 translation(const Cartesian3& vector);

    // rotations around main axes
    static Matrix4 rotationX(float degrees);

    static Matrix4 rotationY(float degrees);

    static Matrix4 rotationZ(float degrees);
};

std::ostream& operator <<(std::ostream& outStream, const Matrix4& value);

#endif
