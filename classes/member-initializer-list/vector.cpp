/*
 * vector.cpp — THE MEMBER INITIALIZER LIST
 * ----------------------------------------
 *
 * Below is the canonical syntax:
 *
 *     Vector3f::Vector3f() : x(0.0f), y(0.0f), z(0.0f) { }
 *                         ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *                         member initializer list
 *
 * Between the parameter list `()` and the body `{ }`, a colon
 * introduces a comma-separated list of `member-name(initializer-expr)`
 * entries. Each entry CONSTRUCTS that member with the given expression
 * BEFORE the constructor body runs.
 *
 * INITIALIZER LIST vs ASSIGNMENT IN THE BODY
 *   Compare these two ways to write the same constructor:
 *
 *       Vector3f() : x(0.0f), y(0.0f), z(0.0f) { }              // init list
 *       Vector3f()             { x = 0.0f; y = 0.0f; z = 0.0f; } // body
 *
 *   For plain `float` members the observable result is identical. But
 *   conceptually they are not the same thing:
 *     - INIT LIST  Each member is CONSTRUCTED directly with the given
 *                  expression. One step.
 *     - BODY FORM  Each member is first DEFAULT-CONSTRUCTED (a hidden
 *                  step you didn't ask for), then ASSIGNED. Two steps.
 *                  For class-type members that's real wasted work; for
 *                  some types (see below) it isn't even legal.
 *
 * THINGS YOU CAN ONLY DO WITH THE INITIALIZER LIST
 *   The body-form alternative simply doesn't work for these — the
 *   compiler will reject them:
 *
 *     - `const` members           const int id;     // can't be assigned to
 *     - reference members         std::string &ref; // must be bound at birth
 *     - members whose type has no default constructor
 *     - base class constructor calls (with arguments)
 *
 *   When you forget to use the initializer list for these, the compile
 *   error itself is the reminder.
 *
 * THE ORDER GOTCHA
 *   Members are initialized in DECLARATION order (the order they
 *   appear in the class body), NOT the order you write them in the
 *   initializer list. So:
 *
 *       class C {
 *           int a;
 *           int b;
 *       public:
 *           C(int x) : b(x), a(b) { }   // a is initialized FIRST,
 *                                       // reading an uninitialized b!
 *       };
 *
 *   Reordering the list for readability is harmless; RELYING on a
 *   different order than the declaration is a bug. Most compilers
 *   warn about this with `-Wreorder`.
 *
 * MODERN BRACE INIT (C++11+)
 *   You can also write `x{0.0f}` instead of `x(0.0f)`:
 *
 *       Vector3f::Vector3f() : x{0.0f}, y{0.0f}, z{0.0f} { }
 *
 *   Braces FORBID narrowing conversions (e.g. `int{3.7}` is an error)
 *   and dodge the "most vexing parse" ambiguity. Many modern style
 *   guides prefer braces for member initializers.
 *
 * DEFAULT MEMBER INITIALIZERS (C++11+)
 *   You can also give a member a default value AT ITS DECLARATION
 *   SITE in the header:
 *
 *       float x = 0.0f, y = 0.0f, z = 0.0f;
 *
 *   That default applies unless a constructor's initializer list
 *   overrides it. For a class with simple defaults like this one, the
 *   pattern can let you skip the explicit initializer list entirely.
 */

#include "vector.hpp"

// The whole point of the file — directly initialize the three floats
// using the member initializer list. The body is empty because there
// is nothing left to do.
Vector3f::Vector3f() : x(0.0f), y(0.0f), z(0.0f) {
}