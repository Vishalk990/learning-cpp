/*
 * std::is_const, decltype, and const-correctness in function signatures
 * ----------------------------------------------------------------------
 *
 * std::is_const<T> (from <type_traits>) is a compile-time type trait
 * that tells you whether a type T is top-level `const`-qualified.
 *
 *   is_const<int>::value         == false
 *   is_const<const int>::value   == true
 *   is_const_v<T>                  C++17 shorthand for is_const<T>::value
 *
 * It only inspects the OUTER const — it does NOT see through pointers
 * or references:
 *
 *   is_const_v<const int*>   == false   // pointer is non-const,
 *                                       // pointee is const
 *   is_const_v<int* const>   == true    // pointer itself is const
 *   is_const_v<const int&>   == false   // references are never
 *                                       // themselves const-qualified
 *
 * decltype(expr) yields the declared type of an expression, preserving
 * const / reference / cv-qualifiers. It is the bridge that lets you ask
 * "what is the type of this variable?" without spelling the type out.
 *
 * Combining them — is_const_v<decltype(a)> — lets you query at compile
 * time whether some variable was declared const. Typical uses:
 *
 *   1. Template metaprogramming: branch behavior on whether a deduced
 *      type is const.
 *          if constexpr (is_const_v<T>) { ... } else { ... }
 *
 *   2. Compile-time assertions, e.g.
 *          static_assert(is_const_v<decltype(some_global)>,
 *                        "some_global must be const");
 *
 *   3. Building helper traits ("return const T& if input is const,
 *      T& otherwise") in combination with std::conditional,
 *      std::remove_const, std::add_const, etc.
 *
 * In everyday code you usually do NOT reach for is_const directly —
 * writing `const` in function parameters (see PassByReference below)
 * handles the common case. is_const earns its keep in generic code
 * where const-ness is unknown until template instantiation.
 */

#include <algorithm>
#include <iostream>
#include <type_traits>
#include <vector>
using namespace std;

// Pass by const reference: no copy is made, and `const` guarantees the
// function cannot mutate the caller's vector. Preferred for any large
// or non-trivially-copyable type when the function only needs to read.
void PassByReference(const vector<int> &alias) {
    // alias[0] = 1;   // would not compile: assignment of read-only location
}

// Pass by value: a full deep copy of the vector is made on entry.
// For a 100M-element vector this is ~400 MB of allocation + memcpy —
// almost always the wrong choice for large containers.
void PassByValue(vector<int> alias) {
    alias[0] = 1;      // mutates the local copy only; caller is unaffected
}

int main() {

    const int a = 42;

    // is_const_v<T> inspects the TYPE you spell out directly.
    cout << is_const_v<int>       << endl;   // 0 — plain int is not const
    cout << is_const_v<const int> << endl;   // 1 — const int is

    // decltype(a) recovers the declared type of `a` (which is `const int`),
    // so the trait below is equivalent to is_const_v<const int>.
    // ::value is the pre-C++17 spelling of the _v shorthand.
    cout << is_const<decltype(a)>::value << endl;   // 1

    // Practical motivation for `const T&` parameters:
    // 100,000,000 ints ≈ 400 MB. PassByValue allocates and copies the
    // entire buffer; PassByReference effectively just passes a pointer.
    vector<int> x(100000000);
    fill(begin(x), end(x), 1);

    PassByValue(x);        // expensive: deep copy of ~400 MB
    PassByReference(x);    // cheap:    no copy, and const prevents mutation
    return 0;
}