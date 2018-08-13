// from http://www.cs.rpi.edu/~musser/design/blitz/meta-art.html
// by Todd Veldhuizen

#include <iostream>

using namespace std;

inline void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

template <int I, int J>
class IntSwap {
public:
    static inline void compareAndSwap(int *data) {
        if (data[I] > data[J])
            swap(data[I], data[J]);
    }
};


template <int I, int J>
class IntBubbleSortLoop {
private:
    enum {go = (J <= I-2)};
public:
    static inline void loop(int *data) {
        IntSwap<J, J+1>::compareAndSwap(data);
        IntBubbleSortLoop<go ? I : 0, go ? (J+1) : 0>::loop(data);
    }
};

template <>
class IntBubbleSortLoop<0,0> {
public:
    static inline void loop(int*) {}
};


template <int N>
class IntBubbleSort {
public:
    static inline void sort(int *data) {
        IntBubbleSortLoop<N-1, 0>::loop(data);
        IntBubbleSort<N-1>::sort(data);
    }
};

template <>
class IntBubbleSort<1> {
public:
    static inline void sort(int *data) {}
};


int main() {

    int data[50];

    IntBubbleSort<50>::sort(data);

    for (int i = 0; i < 50; i++)
        cout << data[i] << endl;

    return 0;
}
