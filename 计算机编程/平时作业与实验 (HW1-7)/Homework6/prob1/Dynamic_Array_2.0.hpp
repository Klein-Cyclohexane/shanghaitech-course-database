#ifndef DYNARRAY_HPP
#define DYNARRAY_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>
#include <algorithm>

class Dynarray {

public:
    using size_type = std::size_t;
    using value_type = int;
    using pointer = int*;
    using reference = int&;
    using const_reference = const int&;
    using const_pointer = const int*;
    Dynarray() : m_storage(nullptr), m_size(0) {}

    explicit Dynarray(int length) : m_storage(new value_type[length]{}), m_size(length) {}

    Dynarray(int length, int value) : Dynarray(length) {
        for (int i = 0; i < length; ++i)
            m_storage[i] = value;
    }

    Dynarray(const_pointer begin, const_pointer end) : Dynarray(end - begin) {
        for (std::size_t i = 0; i < m_size; i++)
            m_storage[i] = begin[i];
    }

    Dynarray(const Dynarray &other) : Dynarray(other.m_size) {
        for (std::size_t i = 0; i < m_size; ++i)
            m_storage[i] = other.m_storage[i];
    }

    Dynarray(Dynarray &&other) noexcept : m_storage(nullptr), m_size(0) {
        *this = std::move(other);
        other.m_storage = nullptr;
        other.m_size = 0;
    }

    ~Dynarray() {
        delete[] m_storage;
    }

    Dynarray &operator=(const Dynarray &other) {
        if (this != &other) {
            Dynarray temp(other);
            *this = std::move(temp);
        }
        return *this;
    }

    Dynarray &operator=(Dynarray &&other) noexcept {
        if (this != &other) {
            delete[] m_storage;
            m_storage = other.m_storage;
            m_size = other.m_size;
            other.m_storage = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    size_type size() const {
        return m_size;
    }

    bool empty() const {
        return m_size == 0;
    }

    reference at(std::size_t index) {
        if (index >= m_size) {
            throw std::out_of_range{"Dynarray index out of range!"};
        }
        return m_storage[index];
    }

    const_reference at(std::size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range{"Dynarray index out of range!"};
        }
        return m_storage[index];
    }

    //Subscript operator overloading
    reference operator[](size_type index) {
        return m_storage[index];
    }

    const_reference operator[](size_type index) const {
        return m_storage[index];
    }

    //Relational operator overloading
    bool operator==(const Dynarray &rhs) const {
        return std::equal(m_storage, m_storage + m_size, rhs.m_storage, rhs.m_storage + rhs.m_size);
    }

    bool operator!=(const Dynarray &rhs) const {
        return !(*this == rhs);
    }

    bool operator<(const Dynarray &rhs) const {
        return std::lexicographical_compare(m_storage, m_storage + m_size, rhs.m_storage, rhs.m_storage + rhs.m_size);
    }

    bool operator>(const Dynarray &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Dynarray &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Dynarray &rhs) const {
        return !(*this < rhs);
    }

    //Output operator overloading
    friend std::ostream &operator<<(std::ostream &os, const Dynarray &dynarray) {
        os << "[";
        for (size_type i = 0; i < dynarray.m_size; i++) {
            os << dynarray.m_storage[i];
            if (i != dynarray.m_size - 1)
                os << ", ";
        }
        os << "]";
        return os;
    }

private:
    int* m_storage;
    std::size_t m_size;
};

#endif // DYNARRAY_HPP
