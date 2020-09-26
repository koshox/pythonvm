//
// Created by Kosho on 2020/9/18.
//

#ifndef PYTHONVM_STACK_HPP
#define PYTHONVM_STACK_HPP

// 主要用于gc dfs, 在堆外分配对象
template<typename V>
class Stack {
private:
    V *vector;
    int _len;
    int _size;

public:
    Stack(int n = 16) {
        _len = n;
        vector = new V[n];
        _size = 0;
    }

    ~Stack() {
        delete[] vector;
        _len = 0;
        _size = 0;
    }

    void push(V v) {
        vector[_size++] = v;
    }

    V pop() {
        return vector[--_size];
    }

    V top() {
        return vector[_size - 1];
    }

    V peek(int index) {
        return vector[_size - index - 1];
    }

    int len() {
        return _len;
    }

    int size() {
        return _size;
    }

    bool empty() {
        return _size == 0;
    }

    void copy(const Stack<V> *stack);

    void oops_do(OopClosure *f);
};

#endif //PYTHONVM_STACK_HPP
