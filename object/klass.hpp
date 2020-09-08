//
// Created by Kosho on 2020/8/16.
//

#ifndef PYTHONVM_KLASS_HPP
#define PYTHONVM_KLASS_HPP

#include <stdio.h>
#include <assert.h>
#include "util/arrayList.hpp"

class HiTypeObject;
class HiObject;
class HiString;
class HiDict;
class HiList;

class Klass {
private:
    Klass *_super;
    HiTypeObject *_type_object;
    HiString * _name;
    HiDict* _klass_dict;

public:
    Klass();

    static int compare_klass(Klass* x, Klass* y);

    void set_type_object(HiTypeObject *x) { _type_object = x; }
    HiTypeObject *type_object() { return _type_object; }

    void set_super(Klass *x) { _super = x; }
    Klass *super() { return _super; }

    void set_name(HiString * x) { _name = x; }
    HiString *name() const { return _name; }

    void set_klass_dict(HiDict *dict) { _klass_dict = dict; }
    HiDict *klass_dict() { return _klass_dict; }

    virtual void print(HiObject *obj) {};

    virtual HiObject* greater  (HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* less     (HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* equal    (HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* not_equal(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* ge       (HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* le       (HiObject* x, HiObject* y) { return 0; }

    virtual HiObject* add(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* sub(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* mul(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* div(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* mod(HiObject* x, HiObject* y) { return 0; }

    virtual HiObject *subscr(HiObject *x, HiObject *y) { return 0; }
    virtual void store_subscr(HiObject *x, HiObject *y, HiObject *z) { return; }
    virtual void del_subscr(HiObject *x, HiObject *y) { return;}

    virtual HiObject *len(HiObject *x) { return 0; }

    virtual HiObject *contains(HiObject *x, HiObject *y) { return 0; }

    virtual HiObject *next     (HiObject* x) { return 0; }
    virtual HiObject *iter     (HiObject* x) { return 0; }

    virtual HiObject *allocate_instance(ArrayList<HiObject *> *args) { return 0;}
};

#endif //PYTHONVM_KLASS_HPP
