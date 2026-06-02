/*
 * int_array.cpp — bodies of the five special members.
 *
 * The concept and overall layout live in int_array.hpp. This file
 * implements each member with print statements so you can SEE which
 * one ran in the output. Compare the two pairs:
 *
 *   COPY pair  → allocates a new buffer and copies elements (slow)
 *   MOVE pair  → steals the source's pointer (fast — just a few
 *                assignments, no `new`, no element copy)
 */

#include "int_array.hpp"

#include <iostream>

// Primary constructor: take a name, allocate the buffer. The member
// initializer list directly initializes both members in one step —
// see classes/member-initializer-list/ for why this form is preferred
// over assigning in the body.
IntArray::IntArray(std::string name) : m_name(name), m_data(new int[10]) {
    std::cout << m_name << " was constructed" << std::endl;
}

// Destructor: free the owned buffer. Note `delete[] nullptr` is a
// well-defined no-op — that's exactly what we want, because after a
// move the source's m_data is nullptr and the source's destructor
// still runs at end of scope. Without that guarantee a moved-from
// object would either leak or double-free.
IntArray::~IntArray() {
    std::cout << m_name << " was destructed " << std::endl;
    delete[] m_data;
}

// COPY CONSTRUCTOR — DEEP COPY. Allocate a brand-new buffer and copy
// the elements over one by one. The "_copy" suffix on the name is a
// visual marker for the output, so you can tell at a glance whether
// a print came from a copy, a move, or an original.
IntArray::IntArray(const IntArray &rhs) {
    m_name = rhs.m_name + "_copy";
    std::cout << "was copy constructed from " << rhs.m_name << std::endl;
    m_data = new int[10];
    for (int i = 0; i < 10; i++) {
        m_data[i] = rhs.m_data[i];
    }
}

// COPY ASSIGNMENT — DEEP COPY onto an object that ALREADY exists.
// Three responsibilities (same as classes/rule-of-three/):
//   1. Self-assignment guard (`this != &rhs`)
//   2. Free the old buffer before reallocating (otherwise it leaks)
//   3. Allocate fresh and copy element-by-element
// Returns *this by reference so chained `a = b = c` works.
IntArray &IntArray::operator=(const IntArray &rhs) {

    if (this != &rhs) {
        delete[] m_data;

        m_name = rhs.m_name + "_copy";
        std::cout << " was copy assigned from " << std::endl;

        m_data = new int[10];
        for (int i = 0; i < 10; i++) {
            m_data[i] = rhs.m_data[i];
        }
    }
    return *this;
}

// MOVE CONSTRUCTOR — STEAL the source's buffer instead of copying.
//   - Take the pointer:     m_data = source.m_data
//   - Null out the source:  source.m_data = nullptr
//     (so its destructor's delete[] becomes a no-op — no double free)
//   - Mirror the same for the name (clear the source's name too).
//
// No `new`, no element copy — just three assignments. THIS is the
// performance win that justifies writing two extra special members.
//
// Should ideally be marked `noexcept` (see the header) so std::vector
// will use it during reallocation:
//     IntArray(IntArray &&source) noexcept { ... }
IntArray::IntArray(IntArray &&source) {
    m_name = source.m_name;
    source.m_name = "";
    m_data = source.m_data;
    source.m_data = nullptr;
    std::cout << m_name << " was move constructed" << std::endl;
}

// MOVE ASSIGNMENT — STEAL onto an existing object.
// Same pattern: self-assignment guard, take the pointer, null out
// the source.
//
// SUBTLE BUG worth noticing: this implementation does NOT delete the
// existing m_data BEFORE stealing the new one. If the target already
// owned a buffer (which it does — the constructor allocated one),
// that buffer LEAKS here. The correct shape is:
//
//     if (this != &source) {
//         delete[] m_data;          // ← release what we already own
//         m_data = source.m_data;
//         source.m_data = nullptr;
//         m_name = source.m_name;
//         source.m_name = "";
//         ...
//     }
//
// Also should be `noexcept`. Both fixes are worth applying in your
// own code — left as-is here to keep this lesson faithful to the
// version you wrote, but call them out as the next iteration.
IntArray &IntArray::operator=(IntArray &&source) {
    if (this != &source) {
        m_name = source.m_name;
        source.m_name = "";
        m_data = source.m_data;
        source.m_data = nullptr;
        std::cout << m_name << " used move assignment" << std::endl;
    }
    return *this;
}