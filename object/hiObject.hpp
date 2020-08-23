//
// Created by Kosho on 2020/8/14.
//

#ifndef PYTHONVM_HIOBJECT_HPP
#define PYTHONVM_HIOBJECT_HPP

#include "klass.hpp"

class HiObject {
private:
    Klass *_klass;

public:
    Klass *klass() {
        assert(_klass != NULL);
        return _klass;
    }

    void set_klass(Klass *x) { _klass = x; }

    void print();

    HiObject* add(HiObject* x);
    HiObject* sub(HiObject* x);
    HiObject* mul(HiObject* x);
    HiObject* div(HiObject* x);
    HiObject* mod(HiObject* x);

    HiObject* greater  (HiObject* x);
    HiObject* less     (HiObject* x);
    HiObject* equal    (HiObject* x);
    HiObject* not_equal(HiObject* x);
    HiObject* ge       (HiObject* x);
    HiObject* le       (HiObject* x);

    HiObject *get_attr(HiObject *x);
    HiObject *set_attr(HiObject *x, HiObject *y);

    HiObject *subscr(HiObject *x);
    HiObject *contains(HiObject *x);
    void store_subscr(HiObject *x, HiObject *y);
    void del_subscr(HiObject *x);

    HiObject* len();
};

#endif //PYTHONVM_HIOBJECT_HPP
