/*
 * Store flags in a bitfield struct
 */

#include <iostream>

using namespace std;

using flag_t = unsigned char;

// union to access f1,...,f4 through a single variable
union status {
    struct {
        flag_t f1 : 1;
        flag_t f2 : 1;
        flag_t f3 : 1;
        flag_t f4 : 1;
    } bf;
    
    flag_t c;

    status &operator=(flag_t const f) {
        this->c = f;
        return *this;
    }
    
    flag_t operator|(flag_t const f) {
        return this->c | f;
    }

    flag_t operator&(flag_t const f) {
        return this->c & f;
    }

    //...
};

static const flag_t flag1 = 0x1 << 0;
static const flag_t flag2 = 0x1 << 1;
static const flag_t flag3 = 0x1 << 2;
static const flag_t flag4 = 0x1 << 3;

int main() {

    status st;

    // w/o operators
    // flag_t &flag = st.c;
    // flag = flag1 | flag3;

    // w/ operators
    st = flag1 | flag3;
    

    cout << sizeof(st.bf) << endl;
    cout << static_cast<int>(st.bf.f1) << " "
         << static_cast<int>(st.bf.f2) << " "
         << static_cast<int>(st.bf.f3) << " "
         << static_cast<int>(st.bf.f4) << endl;


    return 0;
}
