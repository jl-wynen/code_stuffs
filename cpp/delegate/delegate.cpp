// from
// https://blog.molecular-matters.com/2011/09/19/generic-type-safe-delegates-and-events-in-c/

#include <iostream>
#include <utility>  // for std::pair
#include <assert.h>

using namespace std;

class Delegate {
    typedef void *InstancePtr;
    typedef void (*InternalFunction)(InstancePtr, int);
    typedef std::pair<InstancePtr, InternalFunction> Stub;

    // turns a free function into our internal function stub
    template <void (*Function)(int)>
    static inline void FunctionStub(InstancePtr, int ARG0) {
        // we don't need the instance pointer because we're dealing with free functions
        return (Function)(ARG0);
    }

    // turns a member function into our internal function stub
    template <class C, void (C::*Function)(int)>
    static inline void ClassMethodStub(InstancePtr instance, int ARG0) {
        // cast the instance pointer back into the original class instance
        return (static_cast<C*>(instance)->*Function)(ARG0);
    }

public:
    Delegate(void)
        : stub(nullptr, nullptr) { }

    /// Binds a free function
    template <void (*Function)(int)>
    void Bind(void) {
        stub.first = nullptr;  // superfluous
        stub.second = &FunctionStub<Function>;
    }

    /// Binds a class method
    template <class C, void (C::*Function)(int)>
    void Bind(C* instance) {
        stub.first = instance;
        stub.second = &ClassMethodStub<C, Function>;
    }

    /// Invokes the delegate
    void Invoke(int ARG0) const {
        assert(stub.second != nullptr);
        return stub.second(stub.first, ARG0);
    }

private:
    Stub stub;
};



// ---- Test ----

void FreeFunction(int i) {
    cout << "free: " << i << endl;
}

class Class {
public:
    void MemberFunction(int i) {
        cout << "member: " << i << endl;
    }
};


int main() {
    Delegate delegate;
    delegate.Bind<&FreeFunction>();
    delegate.Invoke(10);

    Class c;
    delegate.Bind<Class, &Class::MemberFunction>(&c);
    delegate.Invoke(11);
    
    return 0;
}
