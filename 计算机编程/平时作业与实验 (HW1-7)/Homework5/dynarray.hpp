#ifndef DYNARRAY_HPP
#define DYNARRAY_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

class Dynarray {
public:
    Dynarray() : m_storage(nullptr), m_size(0) {}

    explicit Dynarray(int length) : m_storage(new int[length]{}), m_size(length) {}

    Dynarray(int length, int value) : Dynarray(length) {
        for (int i = 0; i < length; ++i)
            m_storage[i] = value;
    }

    Dynarray(const int* begin, const int* end) : Dynarray(end - begin) {
        for (std::size_t i = 0; i < m_size; i++)
            m_storage[i] = begin[i];
    }

    Dynarray(const Dynarray &other) : Dynarray(other.m_size) {
        for (std::size_t i = 0; i < m_size; ++i)
            m_storage[i] = other.m_storage[i];
    }

    Dynarray(Dynarray &&other) noexcept : m_storage(nullptr), m_size(0) {
        *this = std::move(other);
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

    std::size_t size() const {
        return m_size;
    }

    bool empty() const {
        return m_size == 0;
    }

    int &at(std::size_t index) {
        if (index >= m_size) {
            throw std::out_of_range{"Dynarray index out of range!"};
        }
        return m_storage[index];
    }

    const int &at(std::size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range{"Dynarray index out of range!"};
        }
        return m_storage[index];
    }

private:
    int* m_storage;
    std::size_t m_size;
};

#endif // DYNARRAY_HPP
