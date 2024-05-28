#pragma once
#include <cmath>

struct Vec2 {
    float x, y;

    // Default constructor
    Vec2() : x(0), y(0) {}

    // Constructor with parameters
    Vec2(float x, float y) : x(x), y(y) {}

    // Overloaded operators for vector operations
    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(float scalar) const;
    Vec2 operator/(float scalar) const;

    // Dot product of two vectors
    float dot(const Vec2& other) const;

    // Cross product of two vectors (returns scalar in 2D)
    float cross(const Vec2& other) const;

    // Magnitude (length) of the vector
    float magnitude() const;

    // Normalize the vector
    Vec2 normalize() const;

};

struct Vec3 {
    float x, y, z;

    // Default constructor
    Vec3() : x(0), y(0), z(0) {}

    // Constructor with parameters
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Overloaded operators for vector operations
    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator*(float scalar) const;
    Vec3 operator/(float scalar) const;

    // Dot product of two vectors
    float dot(const Vec3& other) const;

    // Cross product of two vectors
    Vec3 cross(const Vec3& other) const;

    // Magnitude (length) of the vector
    float magnitude() const;

    // Normalize the vector
    Vec3 normalize() const;

};