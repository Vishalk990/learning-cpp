/*
 * std::move — turning an lvalue into an rvalue
 * --------------------------------------------
 *
 * `std::move` is one of the most misleadingly named things in C++. It
 * does NOT actually move anything. It is just a CAST: given an lvalue,
 * it returns an rvalue reference to it. That is the whole job.
 *
 *     std::move(x)   ==   static_cast<T&&>(x)        // literally
 *
 * Why do we want a cast like that?
 *
 * Recall from references/lvalue_rvalue.cpp:
 *   - A named variable (like `a` below) is an LVALUE.
 *   - The move constructor / move assignment operator only accept
 *     RVALUES (their parameter type is T&&).
 *
 * So if you have a named variable and you want to invoke the
 * "steal its resources" path instead of the copy path, you need a way
 * to tell the compiler "yes, treat this lvalue as if it were a
 * temporary about to die." That is exactly what std::move does — it
 * relabels the value category, and overload resolution that follows
 * picks the move overload instead of the copy.
 *
 * The actual stealing happens INSIDE the move constructor or move
 * assignment operator. std::move just gets you to the right overload.
 *
 * What happens to the moved-from object?
 *   It is left in a "valid but unspecified" state. For std::string
 *   and std::vector this typically means empty (their buffers were
 *   stolen and the source pointers nulled out), but the standard
 *   makes no guarantee about the exact contents. You can safely:
 *     - destroy it,
 *     - reassign to it (e.g. `a = "new content";`),
 *     - call any operation that doesn't depend on its prior value.
 *   You CANNOT assume it still holds what it had before.
 *
 * For built-in types (int, double, raw pointers) there is nothing to
 * steal — `int b = std::move(a);` is identical to `int b = a;`. Move
 * matters only for types that OWN heap-allocated resources.
 *
 * Rule of thumb:
 *   Use std::move when you have a named lvalue you are DONE WITH and
 *   want to give its resources away — passing into a sink function,
 *   storing it in a container, etc. Do NOT std::move a local variable
 *   when you simply `return` it; the compiler already elides the copy
 *   (return value optimization), and writing std::move there can
 *   actually prevent the optimization.
 */

#include <iostream>
#include <string>
using namespace std;

int main() {
    // `a` is an lvalue — a named string owning a heap buffer with content.
    string a = "copy construct me";

    // Default-constructed empty string.
    string newval;

    cout << "mystring: " << a      << endl;     // "copy construct me"
    cout << "newvalue: " << newval << endl;     // (empty)

    // Two equivalent ways to say "treat `a` as an rvalue":
    //
    //   newval = static_cast<string &&>(a);    // the explicit cast
    //   newval = std::move(a);                 // the readable sugar
    //
    // Both turn `a` (an lvalue) into an rvalue-reference expression,
    // which triggers `newval`'s MOVE ASSIGNMENT operator instead of
    // the copy assignment. The move operator steals `a`'s heap buffer
    // and leaves `a` in a valid-but-unspecified state (usually empty).
    // newval = static_cast<string &&> (a);
    newval = move(a);

    cout << "mystring: " << a      << endl;     // typically "" — buffer stolen
    cout << "newvalue: " << newval << endl;     // "copy construct me"
    return 0;
}