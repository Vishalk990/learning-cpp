/*
 * Operator Overloading
 * --------------------
 *
 * Operator overloading lets you define what built-in operators
 * (+, ==, <<, ++, [], etc.) mean for your own types. The compiler
 * translates an expression like `v1 + v2` into a function call —
 * `v1.operator+(v2)` for a member operator, or `operator+(v1, v2)` for
 * a free function — and you supply that function.
 *
 * TWO PLACES YOU CAN WRITE AN OPERATOR
 *
 *   1. As a MEMBER function of the class.
 *      - The left operand of a binary operator is the implicit `*this`.
 *      - The function takes ONE explicit parameter (the right operand).
 *            Vector3f operator+(const Vector3f &rhs);   // this + rhs
 *
 *   2. As a NON-MEMBER (free) function — often declared `friend` if it
 *      needs access to private members.
 *      - BOTH operands are explicit parameters.
 *            Vector3f operator+(const Vector3f &lhs, const Vector3f &rhs);
 *
 *   General rules of thumb:
 *     - Operators where the LEFT operand is YOUR class can be members
 *       (e.g., `=`, `[]`, `()`, `+=`, `++`).
 *     - Operators where the LEFT operand is something else MUST be
 *       free functions. The canonical case is `operator<<` for streams:
 *       the left operand is `std::ostream`, which you can't modify, so
 *       the overload has to live outside your class.
 *     - For symmetric arithmetic (`+`, `-`, `*`), prefer FREE functions
 *       so implicit conversions work on either side (e.g. `2 + v`).
 *     - A handful of operators MUST be members: `=`, `[]`, `()`, `->`.
 *       The compiler enforces this.
 *
 * WHAT THIS FILE DEMONSTRATES
 *   - operator+   member, binary, builds a NEW vector (no mutation)
 *   - operator==  shown BOTH ways (member AND free) for comparison
 *   - operator++  member, PREFIX unary, returns *this by reference
 *   - operator<<  free function (it has to be — left operand is a stream)
 *
 * A few "don'ts" worth knowing:
 *   - Don't overload operators in surprising ways. `+` should add,
 *     `<<` should write. Cute repurposing hurts readers.
 *   - You CANNOT invent new operators (no `**` for power).
 *   - You CANNOT change precedence, associativity, or arity.
 *
 * Expected output:
 *   Inside class function     ← the MEMBER == is what overload resolution picks
 *   Vectors are not equal
 *   (5,5,5)
 */

#include <iostream>

class Vector3f {
  public:
    Vector3f() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    // operator+ as a MEMBER. Binary operator: the left operand is
    // *this (the calling object), and we take only the right operand
    // explicitly. Returns a NEW Vector3f by value — `v1 + v2` shouldn't
    // mutate either operand.
    Vector3f operator+(const Vector3f &rhs) {
        Vector3f result;
        result.x = x + rhs.x;
        result.y = y + rhs.y;
        result.z = z + rhs.z;

        return result;
    }

    // operator== as a MEMBER. See the free-function version BELOW the
    // class for the same operator written the other way. When both
    // exist, overload resolution prefers the member, so the free
    // version below is dormant unless this one is removed/commented.
    bool operator==(const Vector3f &rhs) {
        std::cout << "Inside class function\n";
        if (x == rhs.x && y == rhs.y && z == rhs.z)
            return true;
        return false;
    }

    // operator++ as a MEMBER, PREFIX form: `++v`. Mutates *this and
    // returns it BY REFERENCE so chained calls like `++(++v)` can
    // continue to operate on the same object.
    //
    // (The POSTFIX form `v++` is written differently — it takes a
    // dummy `int` parameter to disambiguate the two:
    //     Vector3f operator++(int);
    // Postfix returns the OLD value BY VALUE, which is usually less
    // efficient than prefix because it has to keep a copy around.)
    Vector3f &operator++() {
        x = x + 1;
        y = y + 1;
        z = z + 1;

        return *this;
    }

    float x, y, z;
};

// operator<< MUST be a free function. The left operand of `cout << v3`
// is `std::cout` (a std::ostream), not our Vector3f — and we cannot
// add member functions to std::ostream, so this overload can never be
// a member of Vector3f. The standard conventions:
//   - take the stream BY REFERENCE (we want to write into the caller's
//     stream, not a copy),
//   - take the value by `const T&` (no copy, no mutation),
//   - RETURN the stream BY REFERENCE, which is what enables chaining:
//     `cout << a << b << c` parses as `((cout << a) << b) << c`.
//
// If Vector3f's members were private, this function would need to be
// declared `friend` inside the class to read them. They're public
// here, so no friend declaration is needed.
std::ostream &operator<<(std::ostream &os, const Vector3f &obj) {
    os << "(" << obj.x << "," << obj.y << "," << obj.z << ")" << std::endl;
    return os;
}

// operator== shown a SECOND time, this time as a FREE function. Both
// operands are explicit parameters. This form is the more common
// choice for symmetric binary operators (== is symmetric: `a == b`
// should be the same as `b == a`) because it also lets implicit
// conversions apply to the LEFT operand. A member operator== could
// never make `42 == some_object` work; a free function can, if the
// right conversion from int to the class exists.
//
// In this file the MEMBER version above intercepts every call, so the
// "Inside free function" line below never actually runs. Comment out
// the member version to see this path get exercised.
bool operator==(const Vector3f &lhs, const Vector3f &rhs) {
    std::cout << "Inside free function\n";
    if (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z)
        return true;
    return false;
}

int main() {

    Vector3f v1;
    Vector3f v2;

    v1.x = 1.f;
    v1.y = 2.f;
    v1.z = 3.f;

    v2.x = 3.f;
    v2.y = 2.f;
    v2.z = 1.f;

    // Compiles to v1.operator==(v2) — the MEMBER version wins overload
    // resolution and prints "Inside class function". Returns false
    // because v1.x != v2.x.
    if (v1 == v2) {
        std::cout << "Vectors are equal\n";
    } else {
        std::cout << "Vectors are not equal\n";
    }
    Vector3f v3;

    // Compiles to v1.operator+(v2) → returns a temporary Vector3f
    // (4,4,4), which is then copy-assigned into v3.
    v3 = v1 + v2;

    // Compiles to v3.operator++() — prefix increment. v3 becomes (5,5,5).
    ++v3;

    // Compiles to operator<<(std::cout, v3) — the FREE function (it
    // HAS to be; std::cout is on the left). The stream-returns-stream
    // pattern is what makes this chainable.
    std::cout << v3;
    return 0;
}
