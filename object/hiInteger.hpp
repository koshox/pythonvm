//
// Created by Kosho on 2020/8/14.
//

#ifndef PYTHONVM_HIINTEGER_HPP
#define PYTHONVM_HIINTEGER_HPP

#include "hiObject.hpp"

class IntegerKlass : public Klass {
private:
    IntegerKlass();
    static IntegerKlass *instance;

public:
    static IntegerKlass *get_instance();
    void initialize();

    virtual void print(HiObject* obj);

    virtual HiObject* greater  (HiObject* x, HiObject* y);
    virtual HiObject* less     (HiObject* x, HiObject* y);
    virtual HiObject* equal    (HiObject* x, HiObject* y);
    virtual HiObject* not_equal(HiObject* x, HiObject* y);
    virtual HiObject* ge       (HiObject* x, HiObject* y);
    virtual HiObject* le       (HiObject* x, HiObject* y);

    virtual HiObject* add(HiObject* x, HiObject* y);
    virtual HiObject* sub(HiObject* x, HiObject* y);
    virtual HiObject* mul(HiObject* x, HiObject* y);
    virtual HiObject* div(HiObject* x, HiObject* y);
    virtual HiObject* mod(HiObject* x, HiObject* y);

    virtual HiObject *allocate_instance(HiObject* callable, ArrayList<HiObject *> *args);

    virtual size_t size();
    virtual void oops_do(OopClosure *f, HiObject *obj);
};

class HiInteger : public HiObject {
private:
    int _value;

public:
    HiInteger(int x);

    int value() {
        return _value;
    };
};

#endif //PYTHONVM_HIINTEGER_HPP
