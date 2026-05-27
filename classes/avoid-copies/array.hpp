/*
 * array.hpp — AVOIDING COPIES: a non-copyable class
 * -------------------------------------------------
 *
 * Compare this with classes/rule-of-three/array.hpp. Two things
 * changed, and they are connected:
 *
 *   1. `data` is now a std::vector<int>, not a raw `int*`.
 *      A vector manages its own heap memory — it copies, moves, and
 *      frees itself correctly. So this class no longer needs to write
 *      a destructor, copy constructor, or copy assignment for
 *      CORRECTNESS. That is the "Rule of Zero": prefer members that
 *      manage their own resources, and you write none of the special
 *      members yourself.
 *
 *   2. We still touch the copy operations — but now for a different
 *      reason: to FORBID copying.
 *
 * THE PROBLEM: SILENT, EXPENSIVE COPIES
 *   Copying would still WORK by default (the vector deep-copies), but
 *   copies are easy to trigger by accident and can be costly:
 *
 *       void print(Array a);     // takes the Array BY VALUE
 *       print(myArray);          // ← silently copies the whole vector
 *
 *   Every such call duplicates the entire buffer just to read it.
 *
 * THE FIX: MAKE THE CLASS NON-COPYABLE WITH `= delete`
 *   `= delete` on a special member tells the compiler "this operation
 *   does not exist — reject any code that uses it." Deleting the copy
 *   constructor means the class can no longer be copied, so the
 *   accidental `print(Array a)` above becomes a COMPILE ERROR instead
 *   of a silent performance bug. The compiler then forces callers to
 *   pass by reference (`const Array&`), which is what you wanted all
 *   along. See main.cpp for the before/after.
 *
 *   Historical note: before C++11 there was no `= delete`. The old
 *   trick was to DECLARE the copy constructor `private` and never
 *   define it, so callers couldn't reach it. `= delete` is the
 *   clearer modern replacement (and gives a better error message).
 */

#pragma once

#include <vector>

class Array {
  public:
    Array();   // default constructor

    // With a std::vector member, none of the special members below are
    // required for memory correctness (Rule of Zero). They appear here
    // only to control COPYABILITY:
    ~Array();                            // empty — the vector frees itself
    Array(const Array &rhs) = delete;    // DELETED: this class cannot be
                                         //   copied, so accidental
                                         //   pass-by-value copies are
                                         //   rejected at compile time.
    Array &operator=(const Array &rhs);  // Copy assignment is left ENABLED
                                         //   (so `b = a` still works). For a
                                         //   fully non-copyable type you'd
                                         //   = delete this one too; it's kept
                                         //   to show = delete can be selective.

    // ── Ordinary members ──
    void PrintArray() const;             // `const`: only reads, so it can be
                                         //   called through a const Array&.
    void Set(int index, int value);      // mutates state, so NOT const.

  private:
    std::vector<int> data;   // owns its own memory; no manual new/delete.
};