#include <iostream>
#include <functional>

using namespace std;

template <typename IT, typename GT>
class Cache {
public:
    Cache(const IT &initial, const GT &gen)
        : _item{initial}, _valid{true}, _gen{gen} { }

    explicit Cache(const GT &gen)
        : _item{}, _valid{false}, _gen{gen} { }

    explicit Cache(GT &&gen)
        : _item{}, _valid{false}, _gen{std::move(gen)} { }

    Cache(const Cache &other) = default;
    Cache &operator=(const Cache &other) = default;
    Cache(Cache &&other) = default;
    Cache &operator=(Cache &&other) = default;
    ~Cache() = default;

    void invalidate() noexcept {
        _valid = false;
    }

    operator IT&() {
        _updateIfNeeded();
        return _item;
    }

    operator const IT&() const {
        _updateIfNeeded();
        return _item;
    }

    IT &get() {
        _updateIfNeeded();
        return _item;
    }

    const IT &get() const {
        _updateIfNeeded();
        return _item;
    }


private:
    mutable IT _item;
    mutable bool _valid;
    GT _gen;

    void _updateIfNeeded() const {
        if (_valid)
            return;

        _item = _gen();
        _valid = true;
    }
};

struct Item;

template <typename GT>
auto makeCache(const GT &gen) {
    return Cache<std::decay_t<decltype(std::declval<GT>()())>, GT>{gen};
}

template <typename GT>
auto makeCache(GT &&gen) {
    return Cache<std::decay_t<decltype(std::declval<GT>()())>, GT>{std::move(gen)};
}


struct Item {
    Item() { cout << "default construct\n"; }
    Item(int x_) : x(x_) { cout << "construct from int\n"; }
    Item(const Item &other) {
        x = other.x;
        cout << "copy item\n";
    }
    Item(Item &&other) {
        x = other.x;
        cout << "move item\n";
    }
    Item &operator=(const Item &other) {
        x = other.x;
        cout << "copy assign item\n";
        return *this;
    }
    Item &operator=(Item &&other) {
        x = other.x;
        cout << "move assign item\n";
        return *this;
    }

    ~Item() {
        cout << "destroy item\n";
    }
    int x = 0;
};


struct Maker {
    Maker() = default;
    Maker(const Maker &other) {
        y = other.y;
        cout << "copy maker\n";
    }

    Maker(Maker &&other) {
        y = other.y;
        cout << "move maker\n";
    }
    
    int y = 1;

    Item make(int x) noexcept {
        return Item(x+y);
    }
};

inline std::ostream &operator<<(std::ostream &os, const Item& it) {
    os << it.x;
    return os;
}

int main() {
    using namespace std::placeholders;

    Maker maker;
    // Cache<Item, std::function<Item()>> c(std::bind(&Maker::make, std::ref(maker), 1));
    auto c = makeCache(std::bind(&Maker::make, std::ref(maker), 1));

    cout << c.get() << '\n';
    c.get() = 5;
    cout << c.get() << '\n';

    // c.invalidate();
    // Item &it = c;
    // // it.x = 4;
    // cout << c.get() << '\n';
    
    return 0;
}
