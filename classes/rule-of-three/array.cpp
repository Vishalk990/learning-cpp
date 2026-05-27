/*
 * array.cpp — IMPLEMENTATION of the Array class.
 *
 * The Rule of Three concept and the class declaration live in
 * array.hpp; this file provides the method bodies. Notice the
 * definition form used throughout:
 *
 *     <return-type> Array::<method>(...)   { ... }
 *
 * The `Array::` attaches to the METHOD NAME (after the return type),
 * linking each definition back to the declaration in the header.
 * Constructors and the destructor have no return type, so they read
 * `Array::Array(...)` and `Array::~Array()`.
 */

#include "array.hpp"

#include <iostream>
using namespace std;

// Default constructor: allocate the buffer and fill it with squares
// (0, 1, 4, 9, ...). This is the `new[]` that the copy operations
// below must DUPLICATE (deep copy), not merely alias (shallow copy).
Array::Array() {
    data = new int[10];
    for (int i = 0; i < 10; i++) {
        data[i] = i * i;
    }
}
// COPY CONSTRUCTOR — runs when a NEW object is built from an
// existing one (e.g. `Array a2 = a1;` or passing an Array by
// value). `rhs` is the source; it's taken by `const Array&` so we
// observe it without copying it (and promise not to modify it).
//
// The crucial line is `data = new int[10];` — we allocate our OWN
// buffer and copy the elements, instead of just copying rhs's
// pointer. This deep copy is what makes a2 independent of a1.
Array::Array(const Array &rhs) {
    cout << "Copy constructor called\n";
    data = new int[10];
    for (int i = 0; i < 10; i++) {
        data[i] = rhs.data[i];
    }
}

// COPY ASSIGNMENT OPERATOR — runs when you assign to an object
// that ALREADY EXISTS (e.g. `a2 = a1;`). Unlike the copy
// constructor, the target already owns a buffer, so there are two
// extra responsibilities:
//
//   1. SELF-ASSIGNMENT CHECK (`if (&rhs == this)`): guards against
//      `a = a;`. Without it, the next line would delete[] our own
//      buffer and then try to copy from the freed memory.
//   2. FREE THE OLD BUFFER (`delete[] data;`) before allocating a
//      new one — otherwise the object's previous buffer leaks.
//
// Returning `*this` by reference is the convention that lets you
// chain assignments like `a = b = c;`.
Array &Array::operator=(const Array &rhs) {
    cout << "Copy assignment operator\n";
    if (&rhs == this) {
        return *this;
    }
    delete[] data;
    data = new int[10];
    for (int i = 0; i < 10; i++) {
        data[i] = rhs.data[i];
    }
    return *this;
}

void Array::Set(int index, int value) {
    data[index] = value;
}

void Array::PrintArray() {
    cout << "[";
    for (int i = 0; i < 10; i++) {
        // The ternary MUST be parenthesized: `<<` binds tighter
        // than `?:`, so without the parens the separator logic is
        // applied to the stream, not to the string. `i < 9` puts a
        // comma BETWEEN elements but not after the last (index 9).
        cout << data[i] << (i < 9 ? ", " : "");
    }
    cout << "]" << endl;
}

// DESTRUCTOR — the third member of the Rule of Three. Frees the
// buffer this object owns. Because the copy constructor and copy
// assignment each gave every object its OWN buffer, each
// destructor frees a distinct allocation — no double-free.
Array::~Array() {
    delete[] data;
}
