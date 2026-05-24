/*
 * Pointer Arithmetic and Array Indexing
 * -------------------------------------
 *
 * Arithmetic on a pointer is NOT measured in bytes — it is measured
 * in elements of the pointee type. Adding 1 to an int* moves the
 * address by sizeof(int) bytes (usually 4); adding 1 to a double*
 * moves by 8; adding 1 to a char* moves by 1. The pointer's TYPE is
 * what tells the compiler how far one "step" really is.
 *
 *     int   xs[5];        // suppose xs starts at address 0x1000
 *     int  *p = xs;       // p == 0x1000
 *     p + 1;              // 0x1004  (1 * sizeof(int) = 4 bytes ahead)
 *     p + 3;              // 0x100C  (3 * sizeof(int) = 12 bytes ahead)
 *
 * This is exactly why array indexing is defined the way it is. The
 * expression `a[i]` is, by definition, shorthand for `*(a + i)`:
 *
 *     a[i]   ==   *(a + i)        // standard says these are equivalent
 *
 * That equivalence has one bizarre consequence — `+` commutes, so
 * `*(a + i)` is also `*(i + a)`, which spells out as `i[a]`:
 *
 *     a[i]   ==   *(a + i)   ==   *(i + a)   ==   i[a]
 *
 * The compiler will happily accept `0[xs]` or `3[xs]`. It's legal but
 * cursed — used only in obfuscation contests; never in real code.
 *
 * Array-to-pointer decay:
 *   When an array name appears in most expressions, it implicitly
 *   converts ("decays") to a pointer to its first element. That's why
 *   `int *p = odds;` works without an explicit `&`. The original array
 *   knows its size via sizeof; the resulting pointer does not.
 *
 * Valid range for pointer arithmetic:
 *   You may compute and compare pointers within an array, and you may
 *   form a "one past the end" pointer (this is what STL iterators do).
 *   You may NOT dereference one-past-the-end, and you may NOT walk
 *   before the start or past one-past-the-end — both are UB.
 *
 * Pointer subtraction:
 *   For two pointers into the same array, `p2 - p1` returns the number
 *   of ELEMENTS between them (as a std::ptrdiff_t), not bytes.
 */

#include <iostream>
using namespace std;

// Print a pointer's address and the value it currently points to.
// Useful for visualizing pointer arithmetic step by step.
void print_pointer(int *p) {
    cout << "  address: " << p
         << "    value: " << *p
         << "\n";
}

// Walk a pointer one element at a time through `count` ints starting
// at `start`. Each `++` advances by sizeof(int) bytes — note how the
// printed addresses go up by 4, not by 1.
void walk_with_pointer(int *start, int count) {
    cout << "Walking with a pointer (++ moves by sizeof(int) bytes):\n";
    int *cursor = start;
    for (int i = 0; i < count; i++) {
        print_pointer(cursor);
        cursor++;
    }
}

// Show that `arr[i]` and `*(arr + i)` are interchangeable — the
// subscript operator is literally defined as the latter.
void show_index_vs_arithmetic(int *arr, int count) {
    cout << "\nIndex form vs explicit arithmetic (identical by definition):\n";
    for (int i = 0; i < count; i++) {
        cout << "  arr[" << i << "] = " << arr[i]
             << "    *(arr + " << i << ") = " << *(arr + i)
             << "\n";
    }
}

int main() {
    // Five odd numbers laid out contiguously in memory.
    int odds[] = {1, 3, 5, 7, 9};
    const int n = sizeof(odds) / sizeof(odds[0]);    // 5

    // Array-to-pointer decay: `odds` becomes int* pointing at odds[0].
    int *p = odds;

    walk_with_pointer(p, n);
    show_index_vs_arithmetic(odds, n);

    // The "i[arr]" curiosity — legal because `+` commutes, so
    //   a[i] == *(a + i) == *(i + a) == i[a]
    // Never write this in real code.
    cout << "\nCaveat: 0[odds] = " << 0[odds]
         << "  (same as odds[0]; works because a[i] == *(a + i))\n";

    return 0;
}