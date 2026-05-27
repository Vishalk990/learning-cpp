/*
 * array.cpp — IMPLEMENTATION for the non-copyable Array.
 *
 * The "avoid copies" rationale and the class declaration are in
 * array.hpp. Note what is DELIBERATELY MISSING here: there is no
 * copy-constructor definition, because the header `= delete`d it. A
 * deleted function has no body — defining one would be a "redefinition
 * of deleted function" error (which is exactly the bug this file
 * started with).
 */

#include "array.hpp"

#include <iostream>
using namespace std;

// Default constructor: fill the vector with 0..9. There is no `new`
// anywhere — std::vector allocates and grows its own storage as we
// push_back into it.
Array::Array() {
    cout << "Constructor\n";
    for (int i = 0; i < 10; i++) {
        data.push_back(i);
    }
}

// Destructor: EMPTY on purpose. `data` is a std::vector, which frees
// its own memory when the Array is destroyed. Since there is nothing
// to clean up by hand, this is really a "Rule of Zero" class — you
// could drop this destructor entirely (or write `~Array() = default;`)
// and the behavior would be identical.
Array::~Array() {
}

// NOTE: there is intentionally NO Array::Array(const Array&) here.
// The copy constructor was `= delete`d in the header, which is what
// makes this class non-copyable — the whole point of the example.

// Copy assignment is still enabled (not deleted), so `b = a` works.
// With a vector member you don't actually need to write this — the
// compiler-generated version would copy the vector correctly. It is
// spelled out to show the shape: self-assignment guard, then copy the
// elements over.
Array &Array::operator=(const Array &rhs) {
    cout << "Copy assignment operator\n";
    if (&rhs == this) {
        return *this;
    }
    data.clear();
    for (int i = 0; i < rhs.data.size(); i++) {
        data.push_back(rhs.data[i]);
    }
    return *this;
}

// Mutates an element, so this is NOT const.
void Array::Set(int index, int value) {
    data[index] = value;
}

// Only READS the data, so it is marked `const`. That const is what
// lets PrintArray be called through a `const Array&` — see print()
// in main.cpp. A read-only method that isn't const can't be used on
// a const reference, which would defeat the whole pass-by-const-ref
// approach to avoiding copies.
void Array::PrintArray() const {
    cout << "[";
    for (int i = 0; i < data.size(); i++) {
        cout << data[i] << (i < data.size() - 1 ? ", " : "");
    }
    cout << "]" << endl;
}
