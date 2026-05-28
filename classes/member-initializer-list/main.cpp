/*
 * main.cpp — see the initializer list's effect at runtime.
 *
 * A default-constructed Vector3f. Because vector.cpp's constructor
 * uses a member initializer list (: x(0.0f), y(0.0f), z(0.0f)), the
 * three floats are SET TO 0.0f before the (empty) body runs. Without
 * the initializer list — and without default member initializers in
 * the header — those floats would be left UNINITIALIZED, because
 * built-in types are not zeroed automatically when they appear as
 * class members.
 *
 * Expected output:
 *     (0,0,0)
 */

#include "vector.hpp"
#include <iostream>

// Free-function operator<< (left operand is std::ostream, so it can't
// be a member). See classes/operator-overloading/main.cpp for the full
// rationale.
std::ostream &operator<<(std::ostream &os, const Vector3f &obj) {
    os << "(" << obj.x << "," << obj.y << "," << obj.z << ")" << std::endl;
    return os;
}

int main() {
    Vector3f v;          // calls Vector3f() — init list sets x, y, z to 0
    std::cout << v;      // prints (0,0,0)
}