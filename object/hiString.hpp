//
// Created by Kosho on 2020/8/14.
//

#ifndef PYTHONVM_HISTRING_HPP
#define PYTHONVM_HISTRING_HPP

#include "hiObject.hpp"

class StringKlass : public Klass {
private:
    StringKlass();

    static StringKlass *instance;

public:
    static StringKlass *get_instance();

    void initialize();

    virtual HiObject *equal(HiObject *x, HiObject *y);

    virtual void print(HiObject *obj);

    virtual HiObject *add(HiObject *x, HiObject *y);

    virtual HiObject *less(HiObject *x, HiObject *y);

    virtual HiObject *subscr(HiObject *x, HiObject *y);

    virtual HiObject *contains(HiObject *x, HiObject *y);

    virtual HiObject *len(HiObject *x);

    virtual HiObject *allocate_instance(HiObject *callable, ArrayList<HiObject *> *args);

    virtual size_t size();

    virtual void oops_do(OopClosure *closure, HiObject *obj);
};

class HiString : public HiObject {
    friend class StringKlass;

private:
    char *_value;
    int _length;

public:
    HiString(const char *x);

    HiString(const char *x, const int length);

    HiString(const int len);

    const char *value() {
        return _value;
    }

    char **value_address() {
        return &_value;
    }

    int length() {
        return _length;
    }

    void set(int i, char x) {
        _value[i] = x;
    }
};

HiObject *string_upper(ObjList args);

#endif //PYTHONVM_HISTRING_HPP
