/*
 * main.cpp — RULE OF FIVE IN ACTION
 * ---------------------------------
 *
 * Demonstrates where MOVE semantics actually pay off: pushing a
 * temporary IntArray into a std::vector. With only the Rule of THREE
 * each push_back would COPY the IntArray (allocate a fresh int[10]
 * and copy 10 ints). With the move constructor in place, the buffer
 * is STOLEN — no new allocation, no element copy.
 *
 * Watch the loop's output:
 *   "0 was constructed"          ← temp is created
 *   "0 was move constructed"     ← vector got it by MOVE (not "copy")
 *   " was destructed "           ← temp's destructor runs with an
 *                                  EMPTY NAME because the move zeroed
 *                                  it out. The empty buffer (nullptr)
 *                                  is also a delete[]-no-op.
 *
 * The reserve(10) call up front is important: without it, push_back
 * would eventually need to GROW the vector, which means relocating
 * the existing elements. std::vector will only use your move
 * constructor for that relocation if it is `noexcept`; otherwise it
 * silently falls back to COPYING for the strong exception guarantee.
 * Reserving capacity up front sidesteps the whole question.
 *
 * Expected output shape (42 lines):
 *   array1 was constructed
 *   <10 iterations of: temp ctor, move ctor, temp dtor>
 *   <vector destruction: "0 was destructed " ... "9 was destructed ">
 *   array1 was destructed
 *
 * (The order in which std::vector destroys its elements is NOT
 * standardized — different implementations destroy forward or reverse.
 * libstdc++ as observed here destroys in forward order (0..9). Don't
 * rely on a particular order for correctness.)
 */

#include "int_array.hpp"
#include <vector>

int main() {
    // Plain stack object. Its destructor runs LAST in main, since
    // destruction order is reverse of declaration order — array1 was
    // declared first, so it's destroyed after `arr`.
    IntArray array1("array1");
    // IntArray array2 = array1;   // would invoke the COPY constructor

    // Reserve capacity for 10 elements up front. No reallocation will
    // happen during the push_backs below — every push_back is a clean
    // single move into already-allocated capacity.
    std::vector<IntArray> arr;
    arr.reserve(10);

    for (int i = 0; i < 10; i++) {
        // Build a stack-local temporary named "0", "1", ..., "9".
        IntArray temp(std::to_string(i));

        // std::move(temp) turns the lvalue `temp` into an rvalue
        // expression, so overload resolution inside push_back picks
        // the MOVE constructor instead of the COPY constructor. The
        // vector grabs temp's pointer; temp is left with m_data=nullptr
        // and m_name="". See move_semantics/mv_semantics.cpp for the
        // detail of what std::move actually does (it's just a cast).
        arr.push_back(std::move(temp));

        // End of loop iteration: temp goes out of scope. Its
        // destructor prints " was destructed " (note the empty name)
        // and delete[] on a nullptr — a clean no-op.
    }

    return 0;
    // At end of main, local objects are destroyed in REVERSE
    // declaration order:
    //   1. arr is destroyed. Its 10 elements' destructors run — order
    //      among the elements is implementation-defined; libstdc++
    //      destroys forward (0..9), but don't rely on it.
    //   2. array1 is destroyed LAST (it was declared first).
}