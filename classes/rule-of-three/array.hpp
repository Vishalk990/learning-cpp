/*
 * array.hpp — INTERFACE for a resource-owning class (THE RULE OF THREE)
 * ---------------------------------------------------------------------
 *
 * This Array class OWNS a raw heap buffer (`int *data`, allocated with
 * new[] in array.cpp). Any class that manages a raw resource must take
 * control of how it is copied and destroyed — otherwise the compiler's
 * generated defaults copy the POINTER (a "shallow copy"), leaving two
 * objects sharing one buffer. That causes aliasing bugs and a
 * double-free when both destructors run.
 *
 * THE RULE OF THREE
 *   If a class needs to define ANY ONE of the following, it almost
 *   always needs ALL THREE — they each manage the same owned resource:
 *
 *     1. Destructor          ~Array();
 *          frees the resource.
 *     2. Copy constructor    Array(const Array &rhs);
 *          deep-copies when a NEW object is built from an existing one.
 *     3. Copy assignment     Array &operator=(const Array &rhs);
 *          deep-copies onto an object that ALREADY exists.
 *
 *   Omit one and you get a leak, a double-free, or aliasing.
 *
 * THE RULE OF FIVE (modern C++11+)
 *   Add a move constructor `Array(Array&&)` and move assignment
 *   `Array &operator=(Array&&)` so the buffer can be cheaply STOLEN
 *   instead of copied. See move_semantics/ for what those do.
 *
 * THE RULE OF ZERO (the modern ideal)
 *   Don't hand-manage raw resources at all. If `data` were a
 *   std::vector<int> instead of an int*, you'd need to write NONE of
 *   these — vector copies, moves, and frees itself correctly. Write
 *   the Rule of Three by hand only when you genuinely own a raw
 *   resource, which is rare in modern application code.
 *
 * This header only DECLARES the members; their bodies (and the
 * reasoning behind each) live in array.cpp.
 */

#pragma once

class Array {
  public:
    Array();                              // default constructor

    // ── The Rule of Three (this class owns a raw buffer) ──
    ~Array();                             // 1. destructor      — frees it
    Array(const Array &rhs);              // 2. copy constructor — deep copy
    Array &operator=(const Array &rhs);   // 3. copy assignment  — deep copy

    // ── Ordinary members ──
    void PrintArray();
    void Set(int index, int value);

  private:
    int *data;   // the owned raw resource — the reason this class
                 // needs all three special members above.
};