/*
 * std::weak_ptr — non-owning observer of a shared_ptr object
 * -----------------------------------------------------------
 *
 * A weak_ptr refers to an object managed by a shared_ptr but does
 * NOT participate in ownership. Creating, copying, or destroying a
 * weak_ptr never changes the shared refcount, and a weak_ptr cannot
 * keep an object alive.
 *
 * Two main use cases:
 *
 *   1. BREAKING REFERENCE CYCLES (the canonical reason it exists —
 *      see the "big pitfall" section of pointers/shared_ptr.cpp).
 *      Two shared_ptrs that hold each other (A → B and B → A)
 *      create a cycle whose refcount never reaches zero — both
 *      objects leak. Make one direction a weak_ptr and the cycle
 *      becomes acyclic from the refcount's perspective; both
 *      objects clean up correctly when external owners drop.
 *
 *      Classic example: in a tree, parent → child is a shared_ptr
 *      (parent owns the child), but child → parent is a weak_ptr
 *      (child observes the parent but does not own it).
 *
 *   2. CACHES AND OBSERVERS. When you want to access an object IF
 *      it still exists, but you don't want your reference to keep
 *      it alive. Common for caches, signal/slot connections, and
 *      "back-pointers" in graph structures.
 *
 * You CANNOT dereference a weak_ptr directly. Two ways to use it:
 *
 *   weak.expired()   // bool — returns true if the object is gone
 *   weak.lock()      // returns a shared_ptr:
 *                    //   - empty (nullptr) if the object is gone
 *                    //   - a valid shared_ptr otherwise, with the
 *                    //     refcount BUMPED while you hold it.
 *
 *   `lock()` is the right way to actually USE the observed object,
 *   because it atomically checks-and-promotes in one step:
 *
 *       if (auto s = weak.lock()) {
 *           s->doSomething();    // safe — refcount bumped while
 *                                // `s` is in scope
 *       } else {
 *           // object has been destroyed
 *       }
 *
 *   Using `expired()` and then accessing the object in two separate
 *   steps is a race condition in multithreaded code — between the
 *   check and the use, another thread could drop the last
 *   shared_ptr. `lock()` avoids this.
 *
 * Lifetime of the control block:
 *   The control block (see shared_ptr.cpp) outlives the object
 *   itself when weak_ptrs are still around. The OBJECT is destroyed
 *   when the strong refcount reaches 0; the CONTROL BLOCK is freed
 *   only when both the strong AND weak counts reach 0. That is what
 *   lets a weak_ptr still know "your object is gone" after the fact.
 *
 * use_count() reports STRONG owners only — weak_ptrs never count
 * themselves. If you have one shared_ptr and ten weak_ptrs to the
 * same object, use_count() is 1.
 *
 * Expected output of this program:
 *
 *     UDT Created
 *     Inside scope count = 1            ← weak_ptr did not add to count
 *     use count = 1
 *     UDT Destroyed                     ← outer scope ended; refcount → 0
 *     Has the weak_ptr expired: 1       ← 1 = true = object is gone
 *     We should see the destructor by now
 */

#include <iostream>
#include <memory>

using namespace std;

// Same UDT as in the previous smart-pointer files — prints on
// construction and destruction so you can observe the lifetime.
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

    // ptr2 is declared in the OUTER scope on purpose, so it will
    // outlive the shared_ptr that owns the UDT. That is what makes
    // the "object expired" demo meaningful at the bottom.
    weak_ptr<UDT> ptr2;

    {
        // Strong owner — creates the UDT, refcount goes to 1.
        shared_ptr<UDT> ptr1 = make_shared<UDT>();
        {
            // Assigning a shared_ptr to a weak_ptr does NOT increment
            // the shared refcount. ptr2 now OBSERVES the UDT without
            // taking ownership; ptr1 is still the only real owner.
            ptr2 = ptr1;

            // use_count() reports the STRONG (shared_ptr) count only.
            // weak_ptrs never count themselves — so this prints 1,
            // not 2.
            cout << "Inside scope count = " << ptr2.use_count() << endl;   // 1
        }
        // Inner scope ends, but nothing dramatic happens. ptr2 lives
        // in the outer scope; the assignment from ptr1 just copied
        // the observer link, which has no lifetime of its own here.

        cout << "use count = " << ptr1.use_count() << endl;                // 1
    }
    // Outer scope ended → ptr1's destructor ran → refcount: 1 → 0 →
    // the UDT was destroyed RIGHT HERE. ptr2 outlives it, but now
    // observes a destroyed object. The control block is still alive
    // (because ptr2 is); it just records "your object is gone."

    // expired() returns true (printed as 1) when the observed object
    // has been destroyed.
    //
    // NOTE: in real code, prefer ptr2.lock() — it returns a shared_ptr
    // (empty if gone, valid otherwise) and is the race-free way to
    // actually USE the observed object.
    cout << "Has the weak_ptr expired: " << ptr2.expired() << endl;        // 1
    cout << "We should see the destructor by now\n";

    return 0;
}