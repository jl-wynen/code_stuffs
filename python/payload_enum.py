"""
Enums with payloads (sum types).

A simple test to try and implement enums with payloads in a more
straight-forward way than the adt package.
"""

from types import MethodType, FunctionType
from typing import Never, Callable, Any, Generic, TypeVar

T = TypeVar('T')


def _convert_to_method(name: str, func: Callable[..., Any]):
    method = FunctionType(
        func.__code__, func.__globals__, name, func.__defaults__, func.__closure__
    )
    method.__kwdefaults__ = func.__kwdefaults__
    method.__annotations__ = func.__annotations__
    method.__doc__ = func.__doc__
    return method


def prevent_subclass(cls: type, **kwargs: object) -> Never:
    raise TypeError("Enums cannot be subclassed")


def _make_variant_cls(name: str, template: type, payload_type: type) -> type:
    class V:
        __slots__ = ('_payload',)
        __match_args__ = ('_payload',)

        def __init__(self, payload: payload_type, /) -> None:
            self._payload = payload

        def __str__(self) -> str:
            return f'{name}({self._payload})'

        def __repr__(self) -> str:
            return f'{template.__name__}.{name}({self._payload!r})'

    for n, attr in template.__dict__.items():
        if not n.startswith('__'):
            setattr(V, n, attr)

    V.__name__ = f'{template.__name__}.{name}'
    return V


def enum(template: type) -> type:
    cls = type(template.__name__, (), {})
    for i, (name, ty) in enumerate(template.__annotations__.items()):
        variant = _make_variant_cls(name, template, ty)
        setattr(cls, name, variant)
    cls.__init_subclass__ = MethodType(_convert_to_method('__init_subclass__', prevent_subclass), cls)
    return cls


@enum
class Alt:
    Foo: int
    Bar: str

    def work(self) -> None:
        print(self, 'working')


print(Alt)
print(Alt.Foo, Alt.Bar)
x = Alt.Bar('asd')
x = Alt.Foo(2)
print(x)
print(repr(x))

match x:
    case Alt.Foo(y) if y == 2:
        print('Foo-2:')
    case Alt.Foo(y):
        print('Foo:', y, type(y))
    case Alt.Bar(y):
        print('Bar:', y, type(y))
    case _:
        raise RuntimeError('fallthrough')

x.work()

# needs handling of type params
# @enum
# class Option(Generic[T]):
#     Some: T
#     Null: None
#

# def attempt(x: int) -> Option[int]:
#     if x > 0:
#         return Option.Some(x)
#     return Option.Null
#
#
# print(attempt(1))
# print(attempt(-1))
