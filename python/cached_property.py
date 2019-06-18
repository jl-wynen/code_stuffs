"""
Taken from
 https://stackoverflow.com/questions/41228551/storing-calculated-values-in-an-object/41230310#41230310
answer by
 bruno desthuilliers
"""


class cached_property(object):
    """
    Descriptor that converts a method with a single self argument
    into a property cached on the instance.

    It also has a hook to allow for another property setter to
    invalidated the cache, cf the `Square` class below for
    an example.
    """

    def __init__(self, func):
        self.func = func
        self.__doc__ = getattr(func, '__doc__')
        self.name = self.encode_name(func.__name__)

    def __get__(self, instance, type=None):
        if instance is None:
            return self
        if self.name not in instance.__dict__:
            instance.__dict__[self.name] = self.func(instance)
        return instance.__dict__[self.name]

    def __set__(self, instance, value):
        raise AttributeError("attribute is read-only")

    @classmethod
    def encode_name(cls, name):
        return "_p_cached_{}".format(name)

    @classmethod
    def clear_cached(cls, instance, *names):
        for name in names:
            cached = cls.encode_name(name)
            if cached in instance.__dict__:
                del instance.__dict__[cached]

    @classmethod
    def invalidate(cls, *names):
        def _invalidate(setter):
            def _setter(instance, value):
                cls.clear_cached(instance, *names)
                return setter(instance, value)
            _setter.__name__ = setter.__name__
            _setter.__doc__ =  getattr(setter, '__doc__')
            return _setter
        return _invalidate



class Square(object):
    def __init__(self, size):
        self._size = size

    @cached_property
    def area(self):
        print("calc area")
        return self.size * self.size

    @property
    def size(self):
        return self._size

    @size.setter
    @cached_property.invalidate("area")
    def size(self, size):
        self._size = size


s = Square(3)
print(s.area)
print(s.area)
s.size = 2
print(s.area)
