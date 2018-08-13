#include <iostream>
#include <vector>
#include <assert.h>
#include <cmath>
#include <chrono>

using namespace std;

template <typename E>
class VecExpression {
public:
    double operator[](size_t i) const {
        return static_cast<E const&>(*this)[i];
    }
    size_t size() const {
        return static_cast<E const&>(*this).size();
    }

    E &operator()() {
        return static_cast<E&>(*this);
    }
    E const &operator()() const {
        return static_cast<E const&>(*this);
    }
};


class Vec : public VecExpression<Vec> {
    vector<double> elems;

public:
    double operator[](size_t i) const { return elems[i]; }
    double &operator[](size_t i) { return elems[i]; }
    size_t size() const { return elems.size(); }

    Vec(size_t n) : elems(n) {}

    // construct Vec from a VecExpression, evaluating the latter
    template <typename E>
    Vec(VecExpression<E> const &vec) : elems(vec.size()) {
        for (size_t i = 0; i < vec.size(); i++)
            elems[i] = vec[i];
    }
};


template <typename E1, typename E2>
class VecSum : public VecExpression<VecSum<E1,E2>> {
    E1 const &_u;
    E2 const &_v;

public:
    VecSum(E1 const &u, E2 const &v) : _u(u), _v(v) { }

    double operator[](size_t i) const { return _u[i] + _v[i]; }
    size_t size() const { return _v.size(); }
};

template <typename E1, typename E2>
VecSum<E1,E2> const operator+(E1 const &u, E2 const &v) {
    return VecSum<E1,E2>(u,v);
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
