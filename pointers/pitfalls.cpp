/*
 * Raw Pointer Pitfalls
 * --------------------
 *
 * Raw pointers are powerful but unforgiving — almost every "C++ is
 * unsafe" horror story traces back to one of a small set of mistakes
 * with raw pointers. This file walks through four classic ones:
 *
 *   1. NULL POINTER DEREFERENCE
 *      Reading or writing through a null (or uninitialized) pointer.
 *      Result: undefined behavior — usually a SIGSEGV on modern OSes
 *      because virtual address 0 is unmapped, but the standard makes
 *      no promises. On systems without memory protection it may
 *      silently corrupt data.
 *
 *   2. MEMORY LEAK
 *      Calling `new` or `new[]` without ever pairing it with the
 *      matching `delete` / `delete[]`. The block stays allocated for
 *      the lifetime of the process. Symptom: the program's memory
 *      footprint grows over time; eventually the OS kills it or the
 *      allocator fails.
 *
 *   3. DANGLING POINTER
 *      Holding a pointer to memory that is no longer valid. Common
 *      flavors:
 *        - returning the address of a LOCAL variable (its storage
 *          ceases to exist when the function returns);
 *        - keeping a pointer to a vector element after the vector
 *          reallocates (push_back can move the underlying buffer);
 *        - using a pointer after `delete` (a.k.a. use-after-free).
 *      Dereferencing a dangling pointer is undefined behavior — it
 *      may "work" because the freed bytes haven't been overwritten
 *      yet, right up until one day it doesn't.
 *
 *   4. DOUBLE-FREE
 *      Calling `delete` (or `delete[]`) twice on the same pointer.
 *      Undefined behavior; in practice, modern allocators detect the
 *      reuse and abort the program (e.g. glibc's "double free or
 *      corruption" message). Even when not detected, it corrupts the
 *      heap's internal bookkeeping.
 *
 * How to detect these in practice:
 *   - Compiler warnings (-Wall -Wextra) catch a few cases at compile
 *     time (e.g. -Wreturn-stack-address for pitfall #3 below).
 *   - AddressSanitizer (-fsanitize=address) catches use-after-free,
 *     double-free, leaks, and out-of-bounds heap access at runtime.
 *   - Valgrind catches the same (slower, no recompile required).
 *
 * Modern C++ defense:
 *   Don't manage raw memory by hand. Use:
 *     std::unique_ptr<T>           — sole ownership, auto-deletes
 *     std::shared_ptr<T>           — shared ownership, ref-counted
 *     std::vector<T>, std::string  — owning containers (RAII)
 *   These make every one of the four pitfalls below either impossible
 *   or significantly harder to write by accident.
 *
 * NOTE: the live code at the bottom of main() intentionally performs
 * a double-free, so running this program will crash. Comment out the
 * second `delete x;` if you want a clean run.
 */

#include <iostream>
using namespace std;

// Demonstrates pitfall #3 (dangling pointer via "return address of
// local"). `x` lives on this function's stack frame; when the
// function returns, that frame is reclaimed. The returned address is
// still a number, but it no longer names a valid object — reading or
// writing through it is undefined behavior.
//
// Most compilers warn about this:
//   warning: address of stack memory associated with local variable
//   returned [-Wreturn-stack-address]
//
// int *return_memory() {
//     int x = 42;
//     return &x;          // BUG: x ceases to exist on return
// }

int main() {

    // ── 1. NULL POINTER DEREFERENCE ────────────────────────────────
    // nullptr is a pointer that points to nothing. Writing through it
    // is UB; in practice on Linux/macOS/Windows it triggers a
    // segmentation fault because virtual address 0 is unmapped.
    //
    // int *px = nullptr;
    // *px = 2;                       // crash: segfault

    // ── 2. MEMORY LEAK ─────────────────────────────────────────────
    // new[] reserves heap memory; without a matching delete[] the
    // block stays allocated until the process exits. Run code like
    // this in a loop and watch the program's RSS climb until the
    // OS kills it.
    //
    // int *arr = new int[1000];
    // // ... no delete[] arr; — leaked

    // ── 3. DANGLING POINTER (use-after-scope) ──────────────────────
    // px holds an address whose lifetime has already ended. The
    // numeric value is "valid" (you can print it), but reading or
    // writing through it is undefined behavior.
    //
    // int *px = return_memory();
    // cout << "*px: " << *px << endl;     // UB

    // ── 4. DOUBLE-FREE ─────────────────────────────────────────────
    // The pointer below is deleted twice. After the first delete the
    // pointer is dangling; the second delete tells the allocator to
    // free the same block again, which corrupts its internal
    // bookkeeping. glibc typically aborts with:
    //     "double free or corruption (...)".
    //
    // Two ways to defuse this if you must use raw new/delete:
    //   1. Set the pointer to nullptr after delete — `delete nullptr`
    //      is well-defined and a no-op.
    //   2. Better: stop using raw new/delete and switch to
    //      std::unique_ptr<int> x = std::make_unique<int>(431);
    int *x = new int;
    *x = 431;

    delete x;
    delete x;          // BUG: double-free — undefined behavior
    return 0;
}