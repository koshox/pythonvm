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

    virtual HiObject *less(HiObject *x, HiObject *y);

    virtual HiObject *subscr(HiObject *x, HiObject *y);

    virtual HiObject *contains(HiObject *x, HiObject *y);

    virtual HiObject *len(HiObject *x);
};

class HiString : public HiObject {
private:
    char *_value;
    int _length;

public:
    HiString(const char *x);

    HiString(const char *x, const int length);

    const char *value() {
        return _value;
    }

    int length() {
        return _length;
    }
};

HiObject *string_upper(ObjList args);

#endif //PYTHONVM_HISTRING_HPP
