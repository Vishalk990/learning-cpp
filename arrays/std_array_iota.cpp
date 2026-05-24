/*
 * std::array and std::iota
 * ------------------------
 *
 * std::array<T, N> (from <array>) is a fixed-size container that wraps a
 * C-style array in a class. Size is part of the type and is known at
 * compile time. Storage lives wherever the object is declared (stack,
 * static, etc.) — no heap allocation, just like a built-in array.
 *
 * Differences vs a built-in T[N] array:
 *   - Knows its own size via .size(); no need to pass length separately.
 *   - Does not decay to a pointer when passed to a function.
 *   - Can be copied, assigned, and returned from functions by value.
 *   - Provides STL-style iterators (begin(), end()), so it plays well
 *     with <algorithm> and <numeric>.
 *   - .at(i) performs bounds checking and throws std::out_of_range on
 *     invalid index; operator[] does NOT (same as a raw array).
 *
 * std::iota (from <numeric>) fills a range [first, last) with
 * sequentially increasing values, starting from the given seed.
 * Here it fills `ids` with 1, 2, 3, ..., 100. Name comes from the APL
 * iota (⍳) operator, which generates a sequence of integers.
 */

#include <array>
#include <iostream>
#include <numeric>
using namespace std;

int main() {

    array<int, 100> ids;

    // Fill ids with 1, 2, 3, ..., 100.
    iota(begin(ids), end(ids), 1);

    // Out-of-bounds access via .at() — throws std::out_of_range at runtime.
    // (Use operator[] for unchecked, faster access; .at() for safety.)
    ids.at(10000) = 9;

    for (int i = 0; i < 100; i++) {
        cout << ids[i] << " ";
    }
    return 0;
}