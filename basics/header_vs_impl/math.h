/*
 * math.h — INTERFACE (header file)
 * --------------------------------
 *
 * A header declares WHAT exists; it does not say HOW. Think of it as
 * the public contract of a translation unit (a .cpp file). Any other
 * .cpp that wants to USE `sum` includes this header to learn the
 * function's signature; the actual code for `sum` lives in math.cpp.
 *
 * What belongs in a header:
 *   - Function DECLARATIONS (signatures), like the one below.
 *   - Class declarations and the layout of their members.
 *   - Type aliases (`using` / `typedef`).
 *   - `constexpr` constants.
 *   - `inline` functions (their bodies are exempt from the
 *     one-definition rule).
 *   - Templates (their bodies must be visible at every use site,
 *     so they live in headers too).
 *
 * What does NOT belong in a header:
 *   - Function bodies (definitions) for ordinary, non-inline
 *     functions — they cause "multiple definition" errors at link
 *     time if the header is included by more than one .cpp.
 *
 * `.h` vs `.hpp`:
 *   Both are valid in C++. `.h` is inherited from C and stays common
 *   in mixed C/C++ codebases. `.hpp` is a C++-only convention that
 *   makes "this is C++" obvious to humans and to some tooling. The
 *   compiler does not care — pick a convention per project and stay
 *   consistent.
 *
 * THE INCLUDE GUARD (the #ifndef/#define/#endif sandwich below)
 * -------------------------------------------------------------
 * If two .cpp files in your project both include this header —
 * directly or transitively through other headers — the preprocessor
 * would paste the contents twice into the same translation unit,
 * giving you redeclaration errors. Example chain:
 *
 *     main.cpp includes math.h
 *     main.cpp also includes utils.h, which itself includes math.h
 *     → math.h's contents pasted into main.cpp twice → error
 *
 * The classic guard fixes this:
 *
 *     #ifndef MATH_H        // "if MATH_H is not yet defined,
 *     #define MATH_H        //  define it, then include the body"
 *     ...
 *     #endif                // on a second #include, MATH_H is
 *                           //  already defined, so the body is
 *                           //  skipped entirely.
 *
 * Modern alternative — `#pragma once`:
 *
 *     #pragma once
 *
 * Single line, no hand-picked unique macro name. Technically not in
 * the C++ standard but universally supported (GCC, Clang, MSVC).
 * Many modern codebases use it exclusively.
 *
 * This file demonstrates BOTH — `#pragma once` for the common fast
 * path (compilers deduplicate by file inode, which is quicker than
 * macro lookup) and the traditional `#ifndef` guard as a portable
 * fallback. Picking one is the usual convention; using both is
 * defensive and rarely necessary in modern codebases.
 */

#pragma once

// #ifndef MATH_H
// #define MATH_H

// DECLARATION only — tells the compiler "a function called `sum`
// taking two ints and returning an int exists somewhere". The actual
// body lives in math.cpp; the linker hooks the two together later.
int sum(int a, int b);

// #endif
