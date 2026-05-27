/*
 * main.cpp — AVOID COPIES: problem statement and fix
 * --------------------------------------------------
 *
 * THE PROBLEM (a small example)
 *   A function that takes its argument BY VALUE copies it:
 *
 *       void print(Array arr) { ... }   // 'arr' is a COPY of the caller's
 *       print(myArray);                 // the whole vector is duplicated here
 *
 *   For a class holding a large buffer that copy is pure waste when
 *   the function only needs to READ the data — and it is silent: the
 *   call site `print(myArray)` gives no hint that a copy happened.
 *
 * THE FIX
 *   1. Take the argument by CONST REFERENCE — no copy, and `const`
 *      promises the function won't modify the caller's object:
 *          void print(const Array &arr) { arr.PrintArray(); }
 *
 *   2. To make the "by value" mistake impossible, the class `= delete`s
 *      its copy constructor (see array.hpp). With that in place, the
 *      old `void print(Array arr)` no longer even COMPILES:
 *          error: use of deleted function 'Array::Array(const Array&)'
 *      The compiler turns a silent performance bug into a loud,
 *      can't-miss-it error that forces the reference form.
 *
 * Expected output:
 *     Constructor                 (myArray)
 *     Constructor                 (myArray2)
 *     Copy assignment operator    (myArray2 = myArray)
 *     [1234567, 1, 2, 3, 4, 5, 6, 7, 8, 9]
 */

#include "array.hpp"

// Takes the Array by CONST REFERENCE: no copy is made, and `const`
// means we may only read it. (Calling arr.PrintArray() works because
// PrintArray is a const member function — see array.cpp.)
//
// If you changed this to `void print(Array arr)` (by value), it would
// fail to compile, because Array's copy constructor is = deleted.
void print(const Array &arr) {
    arr.PrintArray();
}

int main() {
    Array myArray; // default constructor → "Constructor"

    myArray.Set(0, 1234567); // mutate one element

    Array myArray2;     // default constructor → "Constructor"
    myArray2 = myArray; // copy ASSIGNMENT (allowed) →
                        //   "Copy assignment operator"

    // print() takes a const reference, so NO copy is made here. Were
    // the copy constructor not deleted and print took its arg by value,
    // this single line would silently duplicate the whole vector.
    print(myArray2); // prints [1234567, 1, 2, ..., 9]

    return 0;
    // Destructors run here. Nothing to free by hand — each Array's
    // std::vector releases its own memory (Rule of Zero).
}