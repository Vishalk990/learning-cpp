/*
 * std::unique_ptr — exclusive ownership of heap memory
 * -----------------------------------------------------
 *
 * From pointers/pitfalls.cpp you know the four classic bugs of raw
 * pointer management: null deref, leak, dangling, double-free. The
 * root cause is almost always the same — when multiple raw pointers
 * refer to the same heap allocation, NO ONE clearly OWNS it. Who is
 * responsible for `delete`-ing it? If everyone assumes someone else
 * will, you leak. If two parties both delete, you double-free.
 *
 * `unique_ptr<T>` is a thin wrapper around a raw pointer with two
 * extra rules:
 *
 *   1. EXCLUSIVE OWNERSHIP: at most one unique_ptr owns a given
 *      object at any moment. Copying is FORBIDDEN at compile time —
 *      `unique_ptr<T> b = a;` will not compile.
 *
 *   2. AUTOMATIC DELETION (RAII): when a unique_ptr is destroyed
 *      (goes out of scope, is reassigned, or `.reset()` is called),
 *      it automatically calls `delete` on the object it owns. You
 *      never write `delete` yourself.
 *
 * Ownership can still be TRANSFERRED, using std::move (see
 * move_semantics/mv_semantics.cpp). After moving, the source is
 * empty (holds nullptr) and the destination owns the object.
 *
 * Why prefer unique_ptr over raw `new` / `delete`?
 *   - Exception safe: if anything between `new` and `delete` throws,
 *     a raw pointer leaks. unique_ptr cleans up automatically.
 *   - Self-documenting: a function returning unique_ptr<T> says "you
 *     own this now"; a parameter of unique_ptr<T> says "I'm taking
 *     ownership." No ambiguity about who deletes.
 *   - Zero overhead: in optimized builds unique_ptr compiles down to
 *     the same machine code as a raw pointer + manual delete. You
 *     pay nothing for the safety.
 *
 * Two ways to create one:
 *
 *     unique_ptr<UDT> a(new UDT);                       // explicit
 *     unique_ptr<UDT> a = std::make_unique<UDT>();      // PREFERRED
 *
 *   Prefer `make_unique<T>(args...)`: it forwards constructor args
 *   to T, never makes you spell out `new`, and avoids a subtle
 *   exception-safety hole the explicit form has when used inside a
 *   function-call expression.
 *
 * Array variant:
 *
 *     unique_ptr<UDT[]> arr(new UDT[10]);                // explicit
 *     unique_ptr<UDT[]> arr = std::make_unique<UDT[]>(10);  // factory
 *
 *   The `[]` in the template argument matters — it makes the
 *   unique_ptr call `delete[]` (not `delete`) when destroying.
 *
 * What this program prints when run:
 *
 *     UDT Created           ← from the constructor at the `new UDT`
 *     UDT Destroyed         ← when `joe` goes out of scope at the
 *                             end of main(). RAII at work — no
 *                             manual delete anywhere in this file.
 */

#include <iostream>
#include <memory>
using namespace std;

// A small user-defined type that announces its own creation and
// destruction. The print statements make RAII visible at runtime:
// you'll see exactly one "UDT Created" and exactly one "UDT Destroyed"
// even though there is no `delete` anywhere in main().
class UDT {
  public:
    UDT() {
        cout << "UDT Created" << endl;
    }
    ~UDT() {
        cout << "UDT Destroyed" << endl;
    }
};

int main() {

    // ── Raw-pointer pattern (commented out) ─────────────────────────
    // The pre-unique_ptr way to manage a heap allocation. The danger
    // is right there in the code: nothing says who OWNS the buffer.
    // Both `array` and `ptr` (and later `ptr2`) point at the same
    // memory. If two of them call delete[], double-free. If none do,
    // leak. The code "works" by social convention, not by structure.

    // int *array = new int[100];
    // for (int i = 0; i < 100; i++) {
    //     array[i] = i;
    // }
    // int *ptr = array;
    // cout << "*ptr = " << *ptr << endl;
    // int *ptr2 = ptr;
    // cout << "*ptr2 = " << *ptr2 << endl;
    // delete[] array;                       // hope nobody else does this

    // Stack-allocated alternative — no heap, no ownership question.
    // The downside: you can't return it from a function or store it
    // somewhere that outlives this scope.
    // UDT person;

    // ── unique_ptr — the modern owning pointer ──────────────────────

    // Explicit form: wrap an existing `new`'d object. `person` now
    // OWNS that heap UDT. When `person` goes out of scope, is
    // reassigned, or `.reset()` is called, the UDT is deleted
    // automatically. Modern preference would be:
    //     auto person = make_unique<UDT>();
    unique_ptr<UDT> person = unique_ptr<UDT>(new UDT);

    // FORBIDDEN: copying a unique_ptr would create two owners — the
    // whole point of "unique" is that this cannot happen. The class
    // explicitly deletes its copy constructor, so this is a COMPILE
    // ERROR, not a runtime bug. Try uncommenting to see:
    //   error: call to deleted constructor of 'unique_ptr<UDT>'
    // unique_ptr<UDT> joe = person;

    // ALLOWED: transfer ownership with std::move. After this line
    // `person` is empty (holds nullptr) and `joe` owns the UDT.
    // (See move_semantics/mv_semantics.cpp for what move actually
    // does — it's just a cast to rvalue reference, which lets the
    // move constructor be selected over the deleted copy constructor.)
    unique_ptr<UDT> joe = move(person);

    // ── Array form (commented out) ──────────────────────────────────
    // For a dynamically-allocated ARRAY, use unique_ptr<T[]>. The
    // `[]` in the template parameter is critical — it makes the
    // unique_ptr call `delete[]` (not `delete`) on destruction.
    //
    // unique_ptr<UDT[]> p_array = unique_ptr<UDT[]>(new UDT[10]);   // explicit
    // unique_ptr<UDT[]> p_array = make_unique<UDT[]>(10);           // factory

    // `main` is about to return. `joe` goes out of scope, its
    // destructor runs, and the UDT is deleted — printing "UDT
    // Destroyed". We never wrote `delete`. That is RAII.
    return 0;
}