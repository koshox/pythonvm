//
// Created by Kosho on 2020/8/14.
//

#ifndef PYTHONVM_ARRAYLIST_HPP
#define PYTHONVM_ARRAYLIST_HPP

#include <stdio.h>

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

    T *value() { return _array; }

    T pop();

    void delete_index(int index);
};

class HiObject;
typedef ArrayList<HiObject *> *ObjList;

#endif //PYTHONVM_ARRAYLIST_HPP
