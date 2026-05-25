/*
 * std::shared_ptr — shared ownership via reference counting
 * ----------------------------------------------------------
 *
 * Where unique_ptr (see pointers/unique_ptr.cpp) says "exactly one
 * owner at a time," shared_ptr says "however many owners you need —
 * the object lives until the LAST one goes away." It is the right
 * tool when the lifetime of a heap object genuinely depends on
 * multiple parts of the program and any of them might be the final
 * user.
 *
 * How it works:
 *   Every shared_ptr to a given object shares access to a small
 *   bookkeeping struct called the CONTROL BLOCK. The control block
 *   holds (among other things) a reference count:
 *
 *     - Copying a shared_ptr increments the count.
 *     - Destroying a shared_ptr decrements the count.
 *     - When the count reaches 0, the managed object is deleted.
 *
 * Memory layout after `make_shared<UDT>()`:
 *
 *     shared_ptr<UDT> ptr1 ──┐
 *     shared_ptr<UDT> ptr2 ──┤      (copies of ptr1 — same target)
 *                            ▼
 *                       ┌────────────────────┐
 *                       │  control block     │
 *                       │   refcount = N     │   ← atomic
 *                       │   weakcount = ...  │   (used by weak_ptr)
 *                       │   deleter   = ...  │
 *                       │   managed UDT      │   ← inline with control
 *                       └────────────────────┘     block (one alloc)
 *
 * Two ways to create one:
 *
 *     shared_ptr<UDT> p(new UDT);                   // explicit (2 allocs)
 *     shared_ptr<UDT> p = std::make_shared<UDT>();  // PREFERRED (1 alloc)
 *
 *   Prefer `make_shared` — it allocates the object and the control
 *   block together as one chunk. Faster, fewer cache misses, and
 *   exception safe.
 *
 * Thread safety — the source of much confusion:
 *   The REFCOUNT is thread-safe. Two threads can safely copy or
 *   destroy shared_ptrs to the same object concurrently — increments
 *   and decrements use atomic operations.
 *   The OBJECT it points to is NOT automatically thread-safe. If
 *   thread A writes through the pointer and thread B reads, you
 *   still need a mutex (or atomic, or other sync) for the data
 *   access itself.
 *
 * When to reach for shared_ptr:
 *   - Multiple long-lived owners, and lifetime really IS shared
 *     (e.g. an object referenced from several caches or graphs).
 *   - Reaching across thread/component boundaries with no clear
 *     "final user."
 *
 * When NOT to reach for shared_ptr:
 *   - You have a clear single owner — use unique_ptr.
 *   - You only want to OBSERVE without owning — pass a raw pointer
 *     or reference alongside the owning unique_ptr/shared_ptr.
 *
 * The big pitfall — REFERENCE CYCLES:
 *   If two shared_ptrs hold each other (A → B and B → A), neither
 *   refcount ever reaches 0, even after every external owner is
 *   gone. Result: leak. Break cycles with std::weak_ptr (a
 *   non-owning observer that does NOT contribute to the count).
 *
 * Costs vs unique_ptr:
 *   - sizeof(shared_ptr<T>) ≈ 2 pointers (vs 1 for unique_ptr).
 *   - Every copy/destroy does atomic counter operations.
 *   - The control block is an extra allocation unless you use
 *     make_shared (which fuses it with the object).
 *
 * Expected output (note that destruction happens BEFORE the final
 * cout, because the outer scope ends first):
 *
 *     UDT Created
 *     use count: = 2
 *     use count: = 1
 *     UDT Destroyed
 *     Destructed will be called before reaching this line
 */

#include <iostream>
#include <memory>

using namespace std;

// Same UDT as in unique_ptr.cpp — prints on construction and
// destruction so the reference-counted lifetime is observable in
// the program output.
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
    // ── Outer scope — establishes the first owner. ──────────────────
    {
        // ptr1 is the first shared_ptr to this UDT. make_shared
        // allocates the UDT and its control block as one contiguous
        // chunk. Refcount starts at 1.
        shared_ptr<UDT> ptr1 = make_shared<UDT>();

        // ── Inner scope — adds a second owner, then drops it. ───────
        {
            // Copying ptr1 into ptr2 increments the shared refcount.
            // Both ptr1 and ptr2 now refer to the SAME UDT and the
            // SAME control block. Refcount: 1 → 2.
            shared_ptr<UDT> ptr2 = ptr1;

            // use_count() returns the current refcount.
            cout << "use count: = " << ptr2.use_count() << endl;   // 2
        }
        // ptr2 went out of scope here. Its destructor decremented the
        // refcount: 2 → 1. The UDT is NOT destroyed yet — ptr1 still
        // owns it.

        cout << "use count: = " << ptr1.use_count() << endl;       // 1
    }
    // Outer scope ended. ptr1's destructor decremented the refcount:
    // 1 → 0 → the UDT is finally destroyed, RIGHT HERE, BEFORE the
    // line below runs. "UDT Destroyed" prints first.

    cout << "Destructed will be called before reaching this line\n";
    return 0;
}