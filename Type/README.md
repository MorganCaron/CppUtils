# [CppUtils](../README.md) / Type

[![Back to Parent Page](https://img.shields.io/badge/-Back_to_Parent_Page-blue?style=for-the-badge)](../README.md)

## Cloneable

The ``ICloneable`` class allows to store instances of different types in the same container.
The stored elements are cloneable with the ``clone()`` method of the ``Cloneable`` class.

<p align="center"><img src="resources/Cloneable.drawio.svg" alt="Cloneable diagram"/></p>

---

## NonCopyable

Types inheriting from the ``NonCopyable`` class will be non-copyable.
The child type must be given as a template of the ``NonCopyable`` class.

<p align="center"><img src="resources/NonCopyable.drawio.svg" alt="NonCopyable diagram"/></p>

---

## NonInstantiable

Types inheriting from the ``NonInstantiable`` class will be non-instantiable.

<p align="center"><img src="resources/NonInstantiable.drawio.svg" alt="NonInstantiable diagram"/></p>

---

## NonMovable

Types inheriting from the ``NonMovable`` class will be non-movable.

<p align="center"><img src="resources/NonMovable.drawio.svg" alt="NonMovable diagram"/></p>

---
