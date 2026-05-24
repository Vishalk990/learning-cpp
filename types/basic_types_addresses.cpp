#include <iostream>
using namespace std;

int main() {

    int x = 23;
    int y = 34.f;
    char a = 'a';
    unsigned char b = 'b';

    cout << "x: " << &(x) << "\n";
    cout << "y: " << &(y) << "\n";
    cout << "a: " << (void *)&(a) << "\n";
    cout << "b: " << (void *)&(b) << "\n";

    return 0;
}