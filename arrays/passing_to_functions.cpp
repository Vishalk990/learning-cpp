/*
 * Passing Arrays Into Functions
 * -----------------------------
 *
 * In C++ you cannot really "pass an array" to a function. The language
 * inherited from C the rule of ARRAY-TO-POINTER DECAY: when an array
 * name appears in an expression context — including a function-call
 * argument list — it implicitly converts to a pointer to its first
 * element. The size, which lived only in the array's TYPE, is lost
 * the moment that conversion happens.
 *
 * Consequence: every legacy-style C array parameter is actually a
 * pointer parameter in disguise, and the function must receive the
 * size separately.
 *
 *     void f(int arr[]);      // looks like an array parameter
 *     void f(int arr[10]);    // looks like a fixed-size array parameter
 *     void f(int *arr);       // ...all three are EXACTLY the same to
 *                             //    the compiler: pointer-to-int.
 *
 * Inside such a function, sizeof(arr) returns sizeof(int*) — i.e. the
 * pointer's size (8 on 64-bit), NOT the array's total bytes. The
 * famous `sizeof(arr) / sizeof(arr[0])` length idiom only works in
 * the scope where the array was DECLARED, because that is the only
 * place the compiler still knows the full array type.
 * (See types/sizeof_basics.cpp gotcha #1.)
 *
 * Why the legacy form survives:
 *   It is the C convention, kept for interop with C libraries and for
 *   the "you don't pay for what you don't use" principle — implicit
 *   array copying on function call would be expensive.
 *
 * Modern C++ alternatives (each preserves the size at the boundary):
 *
 *     void f(int (&arr)[10]);                       // reference to a
 *                                                   //   fixed-size array
 *     template <size_t N> void f(int (&arr)[N]);    // size deduced
 *     void f(std::array<int, 10>& arr);             // size lives in the type
 *     void f(const std::vector<int>& v);            // dynamic, knows size
 *     void f(std::span<int> s);                     // C++20: ptr+size view
 *
 * Rule of thumb: in new code, prefer vector or span. Reach for raw
 * arrays + a size parameter only when interfacing with C APIs.
 *
 * Why size_t for the count parameter:
 *   sizeof, .size(), strlen, and STL indices all return size_t (an
 *   unsigned integer wide enough to hold any object's size). Matching
 *   that type avoids signed/unsigned comparison warnings and reads
 *   naturally at call sites. Trade-off: unsigned arithmetic wraps,
 *   so loops like `for (size_t i = n - 1; i >= 0; --i)` are infinite.
 */

#include <iostream>
#include <vector>
using namespace std;

// C-style: a "decayed" pointer plus a separate size parameter.
// Despite the `int arr[]` syntax, this is identical to `int *arr` —
// the compiler silently rewrites it. The size cannot be recovered
// from `arr` alone, so the caller must supply it.
void print_array(int arr[], size_t size) {
    // sizeof(arr) HERE returns 8 (the pointer's size), NOT the array's
    // total bytes — proof that the array type was lost at the boundary.
    // cout << sizeof(arr) << endl;   // → 8, not 16

    for (size_t i = 0; i < size; i++) {
        cout << arr[i] << ' ';
    }
    cout << endl;
}

// Modern: take the container by const reference. No copy is made (the
// vector could own megabytes — see functions/is_const.cpp), no
// separate size needed because v.size() works, and `const` both
// documents and enforces that the function will not mutate the caller.
void print_vector(const vector<int> &v) {
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
}

int main() {

    int arr[] = {1, 3, 5, 7};
    vector<int> v = {1, 2, 3};

    // The `sizeof(arr) / sizeof(arr[0])` idiom works HERE because
    // `arr` still has array type `int[4]` in this scope. Inside
    // print_array the same expression would yield 8/4 = 2 (pointer
    // size / element size) — usually wrong, and definitely not 4.
    print_array(arr, sizeof(arr) / sizeof(arr[0]));

    print_vector(v);

    return 0;
}