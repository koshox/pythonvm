//
// Created by Kosho on 2020/9/18.
//

#include "util/stack.hpp"

template<>
void Stack<HiObject *>::oops_do(OopClosure *f) {
    for (int i = 0; i < _len; i++) {
        f->do_oop(&vector[i]);
    }
}

template<typename T>
void Stack<T>::copy(const Stack<T> *stack) {
    _size = stack->_size;
    _len = stack->_len;
    for (int i = 0; i < _size; i++) {
        vector[i] = stack->vector[i];
    }
}

template
class Stack<Block>;