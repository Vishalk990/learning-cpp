/*
 * student.cpp — CLASS IMPLEMENTATION (definitions)
 * ------------------------------------------------
 *
 * This file provides the BODIES of the member functions declared in
 * student.h. The new piece of syntax here is the SCOPE RESOLUTION
 * OPERATOR `::`:
 *
 *     void Student::print_name() { ... }
 *          ^^^^^^^^^
 *          "this print_name belongs to class Student"
 *
 * Without the `Student::` prefix, `print_name` would be an ordinary
 * free function with no connection to the class. The `Student::`
 * tells the compiler "I'm defining the member that was declared
 * inside class Student", linking this body back to the declaration
 * in the header. This is what "class scope across files" means — the
 * declaration lives in student.h, the definition lives here, and
 * `::` is the bridge.
 *
 * Inside a member function you refer to the object's own data members
 * directly by name (e.g. `m_name` in print_name). There is an
 * implicit `this` pointer to the current object, so `m_name` is
 * really shorthand for `this->m_name`.
 *
 * Build (same separate-compilation model as header_vs_impl):
 *     g++ student.cpp main.cpp -o prog
 */

#include "student.h"
#include <iostream>
using namespace std;

// CONSTRUCTOR definition. Runs when a Student object is created.
// Here it just announces itself; a real constructor would initialize
// the member variables (often via a constructor initializer list).
Student::Student() {
    cout << "Constructor\n";
}

Student::Student(string name) {
    m_name = name;
    cout << "Constructor Is : \n";
    cout << "m_nam  e is: " << m_name << endl;
}

// DESTRUCTOR definition. Runs when a Student object is destroyed.
// Here it just announces itself; a real destructor would release any
// resources the object owns (heap memory, file handles, etc.).
Student::~Student() {
    cout << "Destructor\n";
}

// Ordinary method definition. `m_name` refers to THIS object's
// m_name member — implicitly this->m_name.
void Student::print_name() {
    cout << m_name << endl;
}