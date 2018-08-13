#include <iostream>

using namespace std;

// gcc
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))

// MSVC
// #define PACK( __Declaration__ ) __pragma(pack(push,1)) __Declaration__ __pragma(pack(pop))

PACK(
struct St {
    unsigned char a;
    unsigned short b;
    unsigned long c;
});

int main() {
    cout << sizeof(St) << endl;
    return 0;
}
