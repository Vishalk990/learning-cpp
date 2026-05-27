/*
 * Copy Constructor, Copy Assignment, and the RULE OF THREE
 * --------------------------------------------------------
 *
 * This Array class OWNS a raw heap resource: `int *data`, allocated
 * with `new[]` and freed with `delete[]`. The moment a class manages
 * a raw resource like this, the compiler-generated defaults become
 * dangerous, and you must take control of how the object is copied
 * and destroyed.
 *
 * SHALLOW vs DEEP copy
 *   If you DON'T write a copy constructor, the compiler generates one
 *   that copies each member as-is. For a pointer member that means
 *   copying the POINTER VALUE — both objects end up pointing at the
 *   SAME heap buffer (a "shallow copy"). Two problems follow:
 *     1. Aliasing: changing one object's data changes the other's.
 *     2. Double-free: when both destructors run, both call delete[]
 *        on the same buffer → undefined behavior / crash.
 *   The fix is a DEEP copy: allocate a fresh buffer and copy the
 *   ELEMENTS, so each object owns an independent copy.
 *
 * THE RULE OF THREE
 *   If a class needs ANY ONE of these three, it almost always needs
 *   all three, because they all deal with the same owned resource:
 *     1. Destructor             — frees the resource
 *     2. Copy constructor       — deep-copies on construction
 *     3. Copy assignment (=)    — deep-copies onto an existing object
 *   Forget one and you get leaks, double-frees, or aliasing bugs.
 *
 * THE RULE OF FIVE (modern C++11+)
 *   Add a MOVE constructor and MOVE assignment operator (taking
 *   Array&&) to allow cheap transfer of the buffer instead of a
 *   copy. See move_semantics/ for what those do. The modern ideal,
 *   though, is the "rule of zero": use std::vector<int> instead of a
 *   raw pointer, and you need to write NONE of these — the vector
 *   manages its own memory and copies/moves correctly for free.
 */

#include <iostream>
using namespace std;

class Array {
  public:
    // Default constructor: allocates the buffer and fills it with
    // squares (0, 1, 4, 9, ...). This is the `new[]` that every copy
    // below must be careful to duplicate rather than alias.
    Array() {
        data = new int[10];
        for (int i = 0; i < 10; i++) {
            data[i] = i * i;
        }
    }
    // COPY CONSTRUCTOR — runs when a NEW object is built from an
    // existing one (e.g. `Array a2 = a1;` or passing an Array by
    // value). `rhs` is the source; it's taken by `const Array&` so we
    // observe it without copying it (and promise not to modify it).
    //
    // The crucial line is `data = new int[10];` — we allocate our OWN
    // buffer and copy the elements, instead of just copying rhs's
    // pointer. This deep copy is what makes a2 independent of a1.
    Array(const Array &rhs) {
        cout << "Copy constructor called\n";
        data = new int[10];
        for (int i = 0; i < 10; i++) {
            data[i] = rhs.data[i];
        }
    }

    // COPY ASSIGNMENT OPERATOR — runs when you assign to an object
    // that ALREADY EXISTS (e.g. `a2 = a1;`). Unlike the copy
    // constructor, the target already owns a buffer, so there are two
    // extra responsibilities:
    //
    //   1. SELF-ASSIGNMENT CHECK (`if (&rhs == this)`): guards against
    //      `a = a;`. Without it, the next line would delete[] our own
    //      buffer and then try to copy from the freed memory.
    //   2. FREE THE OLD BUFFER (`delete[] data;`) before allocating a
    //      new one — otherwise the object's previous buffer leaks.
    //
    // Returning `*this` by reference is the convention that lets you
    // chain assignments like `a = b = c;`.
    Array &operator=(const Array &rhs) {
        cout << "Copy assignment operator\n";
        if (&rhs == this) {
            return *this;
        }
        delete[] data;
        data = new int[10];
        for (int i = 0; i < 10; i++) {
            data[i] = rhs.data[i];
        }
        return *this;
    }

    void Set(int index, int value) {
        data[index] = value;
    }

    void PrintArray() {
        cout << "[";
        for (int i = 0; i < 10; i++) {
            // The ternary MUST be parenthesized: `<<` binds tighter
            // than `?:`, so without the parens the separator logic is
            // applied to the stream, not to the string. `i < 9` puts a
            // comma BETWEEN elements but not after the last (index 9).
            cout << data[i] << (i < 9 ? ", " : "");
        }
        cout << "]" << endl;
    }

    // DESTRUCTOR — the third member of the Rule of Three. Frees the
    // buffer this object owns. Because the copy constructor and copy
    // assignment each gave every object its OWN buffer, each
    // destructor frees a distinct allocation — no double-free.
    ~Array() {
        delete[] data;
    }

  private:
    int *data;   // the owned raw resource — the reason this class
                 // needs all three special members above.
};

int main() {
    Array a1;
    a1.PrintArray();         // [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]

    // SUBTLE BUT IMPORTANT: despite the `=`, this is NOT assignment.
    // `Array a2 = a1;` constructs a brand-new object, so it calls the
    // COPY CONSTRUCTOR (prints "Copy constructor called"), not the
    // copy assignment operator. The `=` here is initialization syntax.
    Array a2 = a1;

    // This commented line WOULD call the copy ASSIGNMENT operator,
    // because a2 already exists at this point:
    // a2 = a1;             // → "Copy assignment operator"

    // Mutate a1's buffer. Thanks to the deep copy, a2 is unaffected —
    // they own separate buffers. (With a shallow copy, this would
    // also change a2.)
    a1.Set(0, 10);
    a1.Set(0, 20);

    a1.PrintArray();         // [20, 1, 4, 9, ...]  — a1 changed
    a2.PrintArray();         // [0, 1, 4, 9, ...]   — a2 untouched: proof
                             //                       the deep copy worked
    return 0;
    // Both destructors run here, each delete[]-ing its own buffer.
    // No double-free precisely because the copies were deep.
}