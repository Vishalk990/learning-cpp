/*
 * student.h — CLASS INTERFACE (declaration)
 * -----------------------------------------
 *
 * A CLASS is a user-defined type that bundles DATA (member variables)
 * and BEHAVIOR (member functions) together under one name. An OBJECT
 * is one concrete instance of a class — `Student john;` makes a
 * Student object called john.
 *
 * This header declares the SHAPE of a Student: what members it has
 * and what their signatures look like. It deliberately does NOT
 * contain the method bodies — those live in student.cpp. This is the
 * same interface/implementation split from basics/header_vs_impl,
 * now applied to a class instead of a single free function.
 *
 * Any .cpp that wants to create or use a Student #includes this
 * header, so the compiler knows the type's layout (how big a Student
 * is, what you can call on it). Both main.cpp and student.cpp include
 * it.
 *
 * ACCESS SPECIFIERS control who may touch each member:
 *   public:     usable from anywhere (inside and outside the class)
 *   private:    usable only from within the class's own methods
 *               (this is the DEFAULT for `class`)
 *   protected:  like private, but also visible to derived classes
 *
 * Naming convention: the `m_` prefix on `m_name` marks a variable as
 * a class MEMBER, so inside methods you can tell member data apart
 * from locals and parameters at a glance. Pure convention — the
 * compiler doesn't care.
 */

#pragma once

#include <string>

class Student {

  public:
    // ── Special member functions ──
    Student();                 // CONSTRUCTOR: runs automatically when a
                               //   Student is created. Same name as the
                               //   class, no return type. Sets up the
                               //   object's initial state.
    Student(std::string name); // OVERLOADED constructor: a second way to
                               //   build a Student, taking an initial name.
                               //   Must be `std::string`, not bare `string`:
                               //   a header must NOT do `using namespace std;`
                               //   (it would leak into every file that
                               //   includes it), so standard-library names
                               //   stay fully qualified here. student.cpp can
                               //   write `string` only because it has its own
                               //   using-directive.
    ~Student();                // DESTRUCTOR: runs automatically when a
                               //   Student is destroyed (goes out of scope).
                               //   Same name with a leading ~, no parameters,
                               //   no return type. Used for cleanup.

    void print_name(); // An ordinary member function (method).
                       //   Its body is defined in student.cpp.

  private:
    // ── Data member ──
    // PRIVATE: outside code cannot touch m_name directly. It is set
    // through the Student(std::string) constructor instead, so the
    // class fully controls its own state. main.cpp supplies the name
    // by passing it to that constructor, not by poking at the member.
    std::string m_name;
};