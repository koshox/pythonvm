//
// Created by Kosho on 2020/8/14.
//

#ifndef PYTHONVM_ARRAYLIST_HPP
#define PYTHONVM_ARRAYLIST_HPP

#include <stdio.h>

class OopClosure;

template<typename T>
class ArrayList {
private:
    int _length;
    T *_array;
    int _size;

    void expand();

public:
    ArrayList(int n = 8);

    void add(T t);

    void insert(int index, T t);

    T get(int index);

    void set(int index, T t);

    int size();

    int length();

    int index(T t);

    T *value() { return _array; }

    T pop();

    void delete_index(int index);

    void *operator new(size_t size);

    void oops_do(OopClosure *closure);
};

class HiObject;
typedef ArrayList<HiObject *> *ObjList;

#endif //PYTHONVM_ARRAYLIST_HPP
