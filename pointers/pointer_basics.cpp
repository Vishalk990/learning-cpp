/*
 * Pointer Basics
 * --------------
 *
 * A pointer is a variable whose value is a MEMORY ADDRESS — the
 * location where another object lives. Pointers give you indirect
 * access: instead of working with a value directly, you work with
 * "where the value is stored" and follow that address when you need
 * the value itself.
 *
 * Two operators do all the work:
 *
 *   &x   "address-of"    — yields the address where x is stored.
 *   *p   "dereference"   — yields the object that p points to.
 *
 * The same `*` token also appears in DECLARATIONS, where it means
 * "pointer to":
 *
 *   int  x   = 7;     // x is an int holding the value 7
 *   int *px  = &x;    // px is a pointer-to-int holding the address of x
 *
 *   px       // the address itself (a number like 0x7ffe...)
 *   *px      // the int that px points to (== x, == 7)
 *
 * Key facts to remember:
 *
 *   - The TYPE of a pointer (int*, double*, MyClass*) tells the
 *     compiler how to interpret the bytes at that address and how
 *     pointer arithmetic should step.
 *   - The SIZE of any pointer is fixed by the platform, not by the
 *     pointee type — typically 8 bytes on 64-bit systems.
 *     sizeof(int*) == sizeof(double*) == sizeof(char*).
 *   - An uninitialized, null (nullptr), or dangling pointer must NOT
 *     be dereferenced — doing so is undefined behavior.
 *   - Prefer references (T&) when you just want an alias and don't
 *     need to re-seat, reassign to null, or do arithmetic. Prefer
 *     smart pointers (unique_ptr / shared_ptr) for owning heap data;
 *     raw pointers are best used as non-owning observers.
 *
 * Why pointers matter:
 *   - Cheap pass-by-reference semantics in C-style APIs.
 *   - Dynamic allocation on the heap (new / delete, malloc / free).
 *   - Runtime polymorphism (virtual dispatch through a base*).
 *   - Building linked data structures (lists, trees, graphs).
 *   - Talking to hardware / OS APIs that traffic in raw addresses.
 */

#include <iostream>
using namespace std;

int main() {
    int x = 7; // ordinary int; lives on the stack

    int *px = &x; // px is a pointer-to-int, initialized with x's address.
                  // Read the declaration right-to-left:
                  // "px is a pointer to int."

    cout << "x stores: " << x << endl;          // 7            — the value itself
    cout << "x address: " << &x << endl;        // 0x...        — where x lives
    cout << "px stores: " << px << endl;        // same address — px holds it
    cout << "px dereferenced: " << *px << endl; // 7            — follow px to get x

    return 0;
}