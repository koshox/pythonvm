//
// Created by Kosho on 2020/8/14.
//

#include "arrayList.hpp"
#include "object/hiObject.hpp"
#include "runtime/universe.hpp"

template<typename T>
ArrayList<T>::ArrayList(int n) {
    _length = n;
    _size = 0;
    _array = new T[n];
}

template<typename T>
void ArrayList<T>::add(T t) {
    if (_size >= _length)
        expand();

    _array[_size++] = t;
}

template<typename T>
void ArrayList<T>::insert(int index, T t) {
    add(NULL);
    for (int i = _size - 1; i > index; i--) {
        _array[i] = _array[i - 1];
    }

    _array[index] = t;
}

template<typename T>
void ArrayList<T>::expand() {
    T *new_array = new T[_length << 1];
    for (int i = 0; i < _length; ++i) {
        new_array[i] = _array[i];
    }

    delete[] _array;
    _array = new_array;

    _length <<= 1;
    printf("expand an array to %d, size is %d\n", _length, _size);
}

template<typename T>
int ArrayList<T>::size() {
    return _size;
}

template<typename T>
int ArrayList<T>::length() {
    return _length;
}

template<typename T>
T ArrayList<T>::get(int index) {
    return _array[index];
}

template<typename T>
void ArrayList<T>::set(int index, T t) {
    if (_size <= index) {
        _size = index + 1;
    }

    while (_size > _length) {
        expand();
    }

    _array[index] = t;
}

template<typename T>
T ArrayList<T>::pop() {
    return _array[--_size];
}

template<typename T>
void ArrayList<T>::delete_index(int index) {
    for (int i = index; i < _size - 1; ++i) {
        _array[i] = _array[i + 1];
    }

    _size--;
}

template <typename T>
int ArrayList<T>::index(T t) {
    return 0;
}

template <>
int ArrayList<HiObject*>::index(HiObject* t) {
    for (int i = 0; i < _size; i++) {
        if (_array[i]->equal(t) == Universe::HiTrue) {
            return i;
        }
    }

    return -1;
}

class HiObject;
template class ArrayList<HiObject *>;

class HiString;
template class ArrayList<HiString *>;

class Block;
template class ArrayList<Block*>;