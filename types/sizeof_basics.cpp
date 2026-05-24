/*
 * The sizeof Operator
 * -------------------
 *
 * sizeof is a COMPILE-TIME unary operator that returns the size, in
 * bytes, of a type or an expression. It is not a function — it does
 * not execute its operand at runtime, it only inspects its TYPE. The
 * result has type std::size_t (an unsigned integer).
 *
 * Two valid forms:
 *
 *     sizeof(type)        // parentheses required for type names
 *     sizeof expr         // parentheses optional for expressions
 *
 * For example:
 *
 *     sizeof(int)         // 4 on most platforms
 *     sizeof x            // size of x's type
 *     sizeof(*px)         // size of what px points to, NOT of px
 *
 * What the standard guarantees:
 *   - sizeof(char) == 1, always. (One "byte" in C++ is whatever a
 *     char is on this platform — almost always 8 bits.)
 *   - sizeof(T*) is the same for every pointer type on a given target
 *     (typically 8 bytes on 64-bit, 4 bytes on 32-bit).
 *   - sizeof gives the in-memory FOOTPRINT of an object, padding for
 *     alignment included.
 *
 * All other sizes (int, long, double, struct layouts, ...) are
 * implementation-defined.
 *
 * Critical gotcha #1 — array decay:
 *   sizeof on an array in the SCOPE where it was declared returns the
 *   total bytes of the whole array. The moment the array decays to a
 *   pointer (passed to a function, assigned to a T*, returned, etc.),
 *   sizeof falls back to giving the POINTER size:
 *
 *       int a[4];
 *       sizeof(a);          // 16  — still an array type here
 *       int *p = a;
 *       sizeof(p);          // 8   — p is just a pointer now
 *
 *   This is why the classic C-array length idiom only works in the
 *   array's own scope:
 *
 *       size_t n = sizeof(a) / sizeof(a[0]);   // 4
 *
 * Critical gotcha #2 — heap allocations:
 *   sizeof on a pointer returned by `new[]` tells you about the
 *   POINTER, not the buffer it points to. The compiler has no idea
 *   how big the heap allocation is — only the runtime allocator does.
 *   There is no portable way to ask "how big is this `new[]` block?"
 *
 * Critical gotcha #3 — class types like std::vector:
 *   sizeof(vector<int>) is the size of the vector OBJECT itself —
 *   typically 3 pointers (begin, end, capacity_end) on most libc++
 *   and libstdc++ implementations. It does NOT include the bytes its
 *   elements occupy on the heap. For element count use v.size(); for
 *   the buffer's byte size use v.size() * sizeof(v[0]).
 *
 * (Numbers below assume a typical 64-bit Linux/macOS build: int = 4,
 *  pointer = 8, vector<T> header = 24.)
 */

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int x = 1;
    int *px = &x;

    // Plain stack array. `arr` here still has ARRAY TYPE (int[4]),
    // so sizeof sees the whole thing — not just the first element.
    int arr[] = {1, 2, 4, 3};

    // Heap allocation. `dynamic_array` is just an int*, regardless of
    // how many ints `new[]` actually reserved.
    int *dynamic_array = new int[100];

    // std::vector owns a heap buffer, but the vector OBJECT itself is
    // a small handle that just bookkeeps that buffer.
    vector<int> v = {1, 2, 1, 5, 3, 15, 5};

    cout << "x:             " << sizeof(x)             << endl;  // 4   — sizeof(int)
    cout << "px:            " << sizeof(px)            << endl;  // 8   — sizeof(int*) on 64-bit
    cout << "arr:           " << sizeof(arr)           << endl;  // 16  — 4 ints * 4 bytes (array NOT decayed)
    cout << "dynamic_array: " << sizeof(dynamic_array) << endl;  // 8   — just a pointer, NOT 400!

    cout << "sizeof v:      " << sizeof(v)             << endl;  // 24  — the vector handle, not its contents
    cout << "v.size():      " << v.size()              << endl;  // 7   — element count of the actual data

    // Always pair `new[]` with `delete[]`. The sizeof demo doesn't
    // need the buffer past this point, but leaking it would contradict
    // the lesson from memory/dynamic_mem_allocation.cpp.
    delete[] dynamic_array;
    return 0;
}
