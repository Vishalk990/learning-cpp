/*
 * Function Pointers (and the std::function wrapper)
 * -------------------------------------------------
 *
 * A function pointer is a value that holds the ADDRESS of a function's
 * machine code. Unlike data pointers (which point into the stack or
 * heap), a function pointer points into the program's executable code
 * segment — the ".text" section that holds compiled instructions. You
 * can store, copy, and pass it around like any other value, then
 * INVOKE the function by calling through the pointer.
 *
 * Functions implicitly convert to pointers when used as values:
 *
 *     auto fp = add;            // implicit conversion to a function ptr
 *     auto fp = &add;           // explicit; identical to the line above
 *     fp(2, 3);                 // call through the pointer
 *     (*fp)(2, 3);              // same — dereference is also implicit
 *
 * The DECLARATION syntax is the famously gnarly part. Read it outward
 * from the variable's name:
 *
 *     int (*op)(int, int);
 *           |        |
 *           |        +--- "...taking (int, int)..."
 *           +------------ "op is a pointer to a function..."
 *                         "...returning int."
 *
 * The parentheses around `*op` are MANDATORY. Without them you get a
 * completely different beast:
 *
 *     int  *op(int, int);       // function RETURNING int*  (not a ptr!)
 *     int (*op)(int, int);      // pointer to function returning int
 *
 * Why function pointers exist:
 *   - Callbacks: pass behavior into another function (sort comparators,
 *     event handlers, signal handlers, ...).
 *   - Dispatch tables: an array of handlers indexed by some key —
 *     classic in interpreters, state machines, GUI frameworks.
 *   - Strategy / polymorphism without classes: pick the algorithm at
 *     runtime by selecting which function to call.
 *   - Interop with C APIs (qsort, signal, pthread_create, ...).
 *
 * THREE WAYS THIS FILE DEMONSTRATES HOLDING A CALLABLE
 * ----------------------------------------------------
 *
 *   1. RAW function pointer        int (*op)(int, int);
 *      The C-style mechanism. Zero overhead — one indirect jump per
 *      call. Drawbacks: the syntax is hard to read, and it can only
 *      hold a plain function (not a lambda with captures, not a bound
 *      member function, not a stateful functor).
 *
 *   2. TYPEDEF / using ALIAS       typedef int (*PFn)(int, int);
 *                                  using   PFn = int(*)(int, int);
 *      Same mechanism as #1, hidden behind a name. The declarations
 *      at use sites become readable:  PFn op = add;
 *      Prefer the C++11 `using` form — it reads left-to-right and
 *      composes more naturally with templates.
 *
 *   3. std::function<int(int, int)>           (from <functional>)
 *      A type-erased wrapper that can hold ANY callable matching the
 *      signature: a function pointer, a stateful lambda, a bound
 *      member function, a functor. The flexibility costs a small
 *      heap allocation (sometimes) and an extra indirection per call.
 *      The right choice when the callable type isn't known at the
 *      call site (e.g., an API that says "give me any int(int,int)").
 *
 * Modern rule of thumb:
 *   - Plain function only, perf-sensitive, C interop  →  raw pointer
 *   - Lambda / functor / bound member ever possible   →  std::function
 *   - Generic context where the type IS known         →  templates
 *     (zero overhead, but the function itself must be a template)
 */

#include <functional>
#include <iostream>

// Form #2 from the header — a C-style typedef giving a name to the
// raw function-pointer type "pointer to a function taking (int, int)
// and returning int". The Hungarian-style "PFn" prefix means "pointer
// to function"; the rest names what the function does.
//
// Equivalent in modern C++ form:
//   using PFnIntegerOperations = int(*)(int, int);
typedef int (*PFnIntegerOperations)(int, int);

// Two plain functions matching the signature `int(int, int)`. Either
// one can be assigned to any of the three callable forms below.
int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main() {

    // Three different ways to declare `op` — pick one by uncommenting.
    // For the same call site (`op(a, b)` further down) they're
    // interchangeable when the callable is a plain function.

    // int (*op)(int, int);                  // 1. Raw function pointer
    // PFnIntegerOperations op;              // 2. Same type, via the typedef
    std::function<int(int, int)> op;         // 3. Type-erased callable

    int n, a, b;
    std::cout << "1 for add or 2 for multiply: ";
    std::cin >> n;
    std::cout << "Enter first number: ";
    std::cin >> a;
    std::cout << "Enter second number: ";
    std::cin >> b;

    // Pick which function to point at based on user input. `add` and
    // `multiply` implicitly convert to function pointers here; if `op`
    // is a std::function, that pointer is then implicitly wrapped.

    op = (n == 1) ? add : multiply;

    // Invoke through the wrapper / pointer. Same call syntax regardless
    // of which of the three declarations above is active.
    std::cout << "Operation: " << op(a, b) << std::endl;
}