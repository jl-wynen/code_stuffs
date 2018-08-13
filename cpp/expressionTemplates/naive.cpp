#include <iostream>
#include <vector>
#include <assert.h>
#include <cmath>
#include <chrono>

using namespace std;

class Vec {
    vector<double> elems;

public:
    Vec(size_t n) : elems(n) {}

    double &operator[](size_t i) { return elems[i]; }
    double operator[](size_t i) const { return elems[i]; }
    size_t size() const { return elems.size(); }
};

Vec operator+(Vec const &u, Vec const &v) {
    Vec sum(u.size());
    for (size_t i = 0; i < u.size(); i++)
        sum[i] = u[i] + v[i];
    return sum;
}

using Clock = chrono::high_resolution_clock;

int main() {
    size_t const N = 100000;
    
    Vec x(N), y(N), z(N), w(N);
    for (size_t i = 0; i < N; i++) {
        x[i] = sqrt(exp(i));
        y[i] = log(sqrt(i+1));
        z[i] = cos(sin(i*i));
        w[i] = x[i]*y[i];
    }

    chrono::time_point<Clock> start = Clock::now();
    Vec r = x + y + z + w;
    chrono::time_point<Clock> end = Clock::now();

    chrono::microseconds diff = chrono::duration_cast<chrono::microseconds>(end-start);

    cout << "time: " << diff.count() << "µs" << endl;
    
    return 0;
}
