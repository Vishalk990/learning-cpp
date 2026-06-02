/*
 * RAII — Resource Acquisition Is Initialization
 * ----------------------------------------------
 *
 * RAII is the C++ idiom that ties RESOURCE LIFETIME to OBJECT LIFETIME.
 * A resource (heap memory, a file handle, a socket, a mutex lock, ...)
 * is ACQUIRED in a constructor and RELEASED in the destructor. Because
 * the language guarantees the destructor runs when the object's
 * lifetime ends — whether by normal scope exit OR by an exception
 * unwinding the stack — the cleanup happens automatically and you
 * cannot accidentally forget it.
 *
 * THE GUARANTEE
 *   When a local (stack) object goes out of scope, its destructor is
 *   called. ALWAYS. That includes:
 *
 *     - normal return from a function,
 *     - early `return` mid-function,
 *     - `break` / `continue` out of a containing loop,
 *     - an exception propagating through the scope.
 *
 *   This is what makes RAII powerful: there is no `finally` block in
 *   C++ because there does not need to be — destructors play that
 *   role. Cleanup code is written ONCE, in the destructor, and runs
 *   on every exit path including the exceptional ones.
 *
 * YOU HAVE ALREADY BEEN USING RAII
 *   Every class and standard-library type that owns a resource follows
 *   this idiom:
 *
 *     std::unique_ptr / std::shared_ptr     delete in destructor
 *     std::vector / std::string             free their buffer in dtor
 *     std::lock_guard / std::scoped_lock    release mutex in dtor
 *     std::ifstream / std::ofstream         close the file in dtor
 *     classes/rule-of-three/Array           delete[] in ~Array()
 *
 *   This file just NAMES the pattern. The Collection class below
 *   acquires a heap buffer in its constructor and releases it in its
 *   destructor — the simplest possible RAII shape.
 *
 * THE BIGGER PICTURE
 *   Languages without RAII (Java, Python, ...) lean on try/finally,
 *   `with` blocks, garbage collection, or `defer` statements — all to
 *   fake the property "this cleanup must run no matter how we leave
 *   this scope." C++ gets it for free from the destructor guarantee.
 *   That is why "use the destructor" answers so many design questions
 *   in C++ that other languages need separate language features for.
 */

#include <iostream>

class Collection {

  public:
    // Constructor — ACQUIRES the resource. Whatever new / open / lock
    // you need to do, do it here. After construction succeeds, the
    // destructor below is guaranteed to run on this object eventually,
    // no matter how its scope exits.
    //
    // (Subtle but worth knowing: if a constructor THROWS before it
    // finishes, the destructor does NOT run for that object — it was
    // never fully constructed. That's a separate corollary to RAII.
    // The demo of "destructor still runs when an exception escapes a
    // FULLY-constructed object's scope" is in main() below.)
    Collection() {
        data = new int[10];
    }

    // Destructor — RELEASES the resource. Runs automatically when the
    // Collection's lifetime ends. We never write `delete[] data;`
    // anywhere else in the program because we don't have to.
    ~Collection() {
        std::cout << "Destructor is called\n";
        delete[] data;
    }

    // Subscript overload — `c[i]` becomes `c.operator[](i)`. Returns a
    // REFERENCE so it works on either side of an assignment:
    //     c[0] = 7;    // writes into data[0]
    //     x = c[0];    // reads data[0]
    // (See classes/operator-overloading/ for the broader topic.)
    int &operator[](std::size_t idx) {
        return data[idx];
    }

  private:
    int *data;   // the owned resource; the reason this class needs RAII
};

int main() {
    // The whole point of this main(): force the RAII unwinding case
    // to ACTUALLY happen. `c` is fully constructed inside the try; we
    // then deliberately throw, BYPASSING the normal `return`; and yet
    // "Destructor is called" still prints — proving the destructor
    // runs while the stack unwinds toward the catch handler.
    try {
        Collection c;                 // ctor runs here — buffer acquired
        c[0] = 7;
        c[1] = 2;
        std::cout << c[1] << std::endl;

        std::cout << "About to throw...\n";
        throw 20;                     // exception escapes this scope ↓
        std::cout << "Never prints\n";   // unreachable after the throw
    } catch (int e) {
        // Look at the ORDER of the output:
        //
        //     2
        //     About to throw...
        //     Destructor is called      ← happened HERE during unwinding
        //     Caught 20 in main
        //
        // By the time control reaches this line, `c` is already gone —
        // its destructor ran during stack unwinding, freeing the heap
        // buffer without a single `delete[]` written by us.
        std::cout << "Caught " << e << " in main\n";
    }

    return 0;
    // Even without the throw, the destructor would still run when `c`
    // left scope — RAII guarantees cleanup on every exit path.
}