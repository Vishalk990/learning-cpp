/*
 * vector.hpp — Vector3f, used to demonstrate the member initializer list.
 *
 * Just a declaration: three public floats and a default constructor.
 * The interesting code lives in vector.cpp, where the constructor's
 * member initializer list does the actual initialization.
 */

#pragma once

class Vector3f {

  public:
    Vector3f();

    float x, y, z;
};
