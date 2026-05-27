/*
 * main.cpp — USING THE CLASS
 * --------------------------
 *
 * Creates and uses a Student object. It only #includes student.h
 * (the interface) — it never sees student.cpp's source. The linker
 * connects the calls here to the method bodies compiled from
 * student.cpp.
 *
 * Object lifetime drives the special member functions automatically.
 * You never call the constructor or destructor by name; the compiler
 * inserts those calls for you:
 *   - `Student john;`  → the CONSTRUCTOR runs   (prints "Constructor")
 *   - end of main()    → john goes out of scope,
 *                        the DESTRUCTOR runs     (prints "Destructor")
 *
 * Expected output:
 *     Constructor
 *     john
 *     Destructor
 */

#include "student.h"

int main() {
    // Declaring the object calls Student's constructor right here.
    Student st("john"); // an INSTANCE (object) of class Student

    // m_name is public, so we can assign it directly. In real code
    // you'd usually pass the name to the constructor or a setter and
    // keep m_name private — an object should own and guard its own
    // state rather than letting outside code poke at it freely.

    // Call a member function with the `.` operator on the object.
    st.print_name(); // prints "john"

    return 0;
    // john's destructor runs here, as main() returns.
}