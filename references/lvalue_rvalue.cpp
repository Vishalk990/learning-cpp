/*
 * lvalues, rvalues, and their references
 * --------------------------------------
 *
 * Every expression in C++ has a "value category". For everyday use,
 * the two that matter are:
 *
 *   lvalue:  has a NAME and an ADDRESS. Lives in a storage location
 *            that persists beyond the current expression. You can
 *            assign to it (put it on the left of `=`) and take its
 *            address with `&`.
 *              examples: a variable, arr[i], *ptr
 *
 *   rvalue:  a TEMPORARY with no name. Exists only during the
 *            expression. You can read it, but you can't assign to it
 *            and you can't take its address.
 *              examples: 10, "abc", a + b, foo() (a return value)
 *
 * Quick test — "can I put it on the LEFT side of `=`?"
 *              yes → lvalue, no → rvalue.
 *
 * REFERENCES bind to one category or the other:
 *
 *   T&    — lvalue reference.  Binds ONLY to lvalues.
 *              int i = 10;
 *              int& ref = i;        // OK
 *              int& ref = 10;       // ERROR — 10 is an rvalue
 *
 *   T&&   — rvalue reference.  Binds ONLY to rvalues.
 *              int&& ref = 10;            // OK
 *              string&& s = s1 + s2;      // OK — s1+s2 is a temporary
 *              int&& ref = i;             // ERROR — i is an lvalue
 *
 *   const T& — special case: binds to BOTH, and extends the lifetime
 *              of a bound temporary. That's why function parameters
 *              are usually written `const T&`.
 *
 * Why bother with rvalue references?
 *   They let you give a name to a temporary and "steal" its resources
 *   instead of copying it — the basis of move semantics.
 *   (See move_semantics/ once you get there.)
 */

#include <iostream>
#include <string>
using namespace std;

// Returns a temporary int. The CALL EXPRESSION `get31()` is an rvalue —
// the returned value has no name, no address; it only exists briefly.
int get31() {
    return 31;
}

int main() {
    // ── lvalues vs rvalues — what's allowed on which side of `=` ──

    // `i` is an lvalue (named, has an address). `10` is an rvalue.
    // int i = 10;            // OK:    lvalue = rvalue

    // 10 cannot appear on the LEFT — it's an rvalue with nowhere to write.
    // 10 = i;                // ERROR: cannot assign to an rvalue

    // `a` and `b` are lvalues, but the expression `(a + b)` produces a
    // TEMPORARY int with no name — that temporary is an rvalue, read
    // out into `c` before it disappears.
    // int a = 2;
    // int b = 2;
    // int c = (a + b);       // c (lvalue) ← (a + b) (rvalue)

    // `10` is an rvalue, so a regular `int&` can't bind to it — but an
    // rvalue reference (T&&) can. `ref` now names that temporary.
    // int&& ref = 10;        // OK

    // `arr[10 + a]` is an lvalue: it names a specific array element with
    // a real address, even though the index expression is computed.
    // int arr[100];
    // arr[10 + a];

    // The RETURN VALUE of get31() is an rvalue — there's nothing to
    // assign INTO, hence the error.
    // get31() = 100;         // ERROR: cannot assign to an rvalue

    // ── Active demo: an lvalue reference and an rvalue reference ──

    int i = 10;
    int &ref = i;              // lvalue reference: ref is an alias for i.
                               // After this, `ref = 5;` is the same as
                               // `i = 5;` — changing one changes the other.

    string s1 = "abv";
    string s2 = "f33";

    // `s1 + s2` produces a brand-new temporary string with no name —
    // that temporary is an rvalue, exactly what an rvalue reference
    // can bind to. `s3` gives the temporary a name and extends its
    // lifetime to the end of this scope.
    string &&s3 = s1 + s2;
    cout << s3 << endl;        // prints "abvf33"
    return 0;
}