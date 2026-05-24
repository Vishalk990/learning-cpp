/*
 * Dynamic Memory Allocation with new[] / delete[]
 * -----------------------------------------------
 *
 * C++ programs use two main regions of memory:
 *
 *   - The STACK: fast, automatically managed, size known at compile
 *     time. Local variables (like `int x = 7;`) live here. Memory is
 *     reclaimed when the enclosing scope ends.
 *
 *   - The HEAP (a.k.a. "free store"): a large pool used for objects
 *     whose size is only known at runtime, or whose lifetime needs to
 *     outlive the scope in which they were created. The programmer
 *     decides when to allocate and when to free.
 *
 * The classic C++ operators for heap allocation are:
 *
 *     T*  p = new  T;          // single object   — pair with `delete`
 *     T*  p = new  T(args);    // single object with constructor args
 *     T*  a = new  T[n];       // array of n Ts   — pair with `delete[]`
 *           delete   p;
 *           delete[] a;
 *
 * `new T[n]` does two things at once:
 *   1. Allocates enough memory on the heap to hold n objects of type T.
 *   2. Returns a pointer to the FIRST element. Array indexing (a[i]) is
 *      really shorthand for *(a + i) — plain pointer arithmetic.
 *
 * Crucial pairing rule (mismatch is undefined behavior):
 *   - Allocated with `new`    →  free with `delete`
 *   - Allocated with `new[]`  →  free with `delete[]`
 *
 * Why dynamic allocation exists:
 *   - The size depends on user input or other runtime information
 *     (as in this file: number_of_students is not known until cin).
 *   - You need an object to outlive the function that created it.
 *   - The object is too large to comfortably sit on the stack.
 *   - You're building a data structure whose shape grows at runtime
 *     (linked lists, trees, dynamic arrays, ...).
 *
 * Common hazards with raw new / delete:
 *   - MEMORY LEAK: forgetting `delete[]` — the OS doesn't reclaim the
 *     block until the process exits, and long-running programs bleed
 *     memory.
 *   - DOUBLE-FREE: calling delete[] twice on the same pointer — UB.
 *   - DANGLING POINTER: using the pointer after delete[] — UB.
 *   - MISMATCHED FORMS: using `delete` instead of `delete[]` (or vice
 *     versa) — UB, even if it appears to "work" in some builds.
 *   - EXCEPTION SAFETY: if code between `new` and `delete[]` throws,
 *     the cleanup is skipped — instant leak.
 *
 * Modern C++ recommendation:
 *   Avoid raw `new` / `delete` in application code. Prefer:
 *     - std::vector<T>  for dynamic arrays (handles allocation,
 *                       growth, and cleanup automatically — RAII).
 *     - std::unique_ptr<T> / std::make_unique<T>(...)  for single
 *                       owned heap objects.
 *     - std::shared_ptr<T> when ownership is genuinely shared.
 *   In modern code, this whole file would be one line:
 *       std::vector<int> student_ids(number_of_students);
 */

#include <iostream>
using namespace std;

int main() {
    int number_of_students = 0;
    cout << "Enter number of students: ";
    cin >> number_of_students;

    // Allocate an array of `number_of_students` ints on the HEAP.
    // The size is a runtime value — this would not be legal on the
    // stack as `int student_ids[number_of_students];` in standard C++.
    // `student_ids` is just a pointer to the first element.
    int *student_ids = new int[number_of_students];

    for (int i = 0; i < number_of_students; i++) {
        student_ids[i] = i; // same as *(student_ids + i) = i;
    }

    // Release the heap memory. MUST use `delete[]` (not plain `delete`)
    // because the allocation was made with `new[]`. After this line,
    // `student_ids` is a dangling pointer — do not dereference it.
    delete[] student_ids;
    return 0;
}