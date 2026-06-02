/*
 * int_array.hpp — THE RULE OF FIVE
 * --------------------------------
 *
 * This is classes/rule-of-three/Array with two extra members added:
 * a MOVE constructor and a MOVE assignment operator. Together they
 * form the "Rule of Five" — the modern C++11+ extension of the
 * Rule of Three:
 *
 *     1. Destructor
 *     2. Copy constructor       \  "Rule of Three" — required for
 *     3. Copy assignment         > CORRECTNESS when a class owns a
 *                                /  raw resource.
 *
 *     4. Move constructor       \  Added in C++11. Optional for
 *     5. Move assignment         > correctness, but huge for
 *                                /  PERFORMANCE — they let the
 *                                   resource be TRANSFERRED instead
 *                                   of duplicated.
 *
 * The rule: if you wrote any one of these five, you should consider
 * writing all five. (And if you can possibly use the Rule of Zero —
 * e.g. replace `int*` with std::vector — write NONE of them.)
 *
 * COPY vs MOVE in one picture
 *
 *     Copy:   a → [1,2,3,4]          b allocates a NEW buffer
 *             b → [1,2,3,4]          and copies elements one by one
 *
 *     Move:   a → [1,2,3,4]          b STEALS the pointer
 *             b → [1,2,3,4]          a → nullptr
 *                                    (no allocation, no copying)
 *
 *   For an Array of 10 ints the difference is small. For an Array of
 *   10 million ints — or a std::string holding a paragraph, or a
 *   std::vector<std::string>, or a network connection — the difference
 *   is enormous. Every modern STL container relies on move semantics
 *   to avoid copying as it grows.
 *
 * WHEN DOES THE COMPILER PICK MOVE OVER COPY?
 *   Overload resolution picks the move overload when the source is
 *   an RVALUE — a temporary, a function's return value, or anything
 *   you wrap in std::move(). See:
 *     - references/lvalue_rvalue.cpp        what an rvalue IS
 *     - move_semantics/mv_semantics.cpp     what std::move actually does
 *
 *   Triggers a MOVE:
 *       Array b = make_array();        // make_array() returns an rvalue
 *       Array b = std::move(a);        // std::move makes `a` an rvalue
 *       v.push_back(IntArray("x"));    // IntArray("x") is a temporary
 *
 *   Triggers a COPY:
 *       Array b = a;                   // a is an lvalue
 *       v.push_back(a);                // a is an lvalue
 *
 * ONE FOOTGUN WORTH KNOWING — `noexcept` ON MOVES
 *   Move operations should be marked `noexcept`. std::vector (and
 *   other containers) will only USE your move during reallocation if
 *   it's noexcept — otherwise they silently fall back to COPYING for
 *   the strong exception guarantee. The moves below are NOT marked
 *   noexcept, which would make vector copy instead of move on grow.
 *   main.cpp sidesteps that by calling reserve(10) up front so no
 *   reallocation happens.
 */

#pragma once

#include <string>

class IntArray {
  public:
    IntArray(std::string name);    // primary constructor — takes a name

    // ── The Rule of Three ──
    ~IntArray();                                   // 1. destructor
    IntArray(const IntArray &rhs);                 // 2. copy constructor   — DEEP COPY
    IntArray &operator=(const IntArray &rhs);      // 3. copy assignment    — DEEP COPY

    // ── The two extras that make this the Rule of Five ──
    IntArray(IntArray &&source);                   // 4. move constructor   — STEAL
    IntArray &operator=(IntArray &&source);        // 5. move assignment    — STEAL

  private:
    std::string m_name;   // identifier so we can SEE what's happening in output
    int *m_data;          // the owned resource — the reason this class needs all five
};