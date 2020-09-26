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
class OopClosure;

class Klass {
private:
    HiList *_super;
    HiList *_mro;
    HiTypeObject *_type_object;
    HiString * _name;
    HiDict* _klass_dict;

    HiObject *find_and_call(HiObject *x, ObjList args, HiObject *func_name);
    HiObject *find_in_parents(HiObject *x, HiObject *y);
public:
    Klass();

    static HiObject *create_klass(HiObject *x, HiObject *supers, HiObject *name);

    static int compare_klass(Klass* x, Klass* y);

    void set_type_object(HiTypeObject *x) { _type_object = x; }
    HiTypeObject *type_object() { return _type_object; }

    void add_super(Klass *x);
    HiTypeObject *super();
    void order_supers();

    void set_super_list(HiList *x) { _super = x; }
    HiList *mro() { return _mro; }

    void set_name(HiString * x) { _name = x; }
    HiString *name() const { return _name; }

    void set_klass_dict(HiDict *dict) { _klass_dict = dict; }
    HiDict *klass_dict() { return _klass_dict; }

    // TODO
    virtual void print(HiObject *obj) {};

    virtual HiObject* greater  (HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* less     (HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* equal    (HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* not_equal(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* ge       (HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* le       (HiObject* x, HiObject* y) { return 0; }

    virtual HiObject* add(HiObject* x, HiObject* y);
    // TODO operator overload
    virtual HiObject* sub(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* mul(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* div(HiObject* x, HiObject* y) { return 0; }
    virtual HiObject* mod(HiObject* x, HiObject* y) { return 0; }

    virtual HiObject *subscr(HiObject *x, HiObject *y);
    virtual void store_subscr(HiObject *x, HiObject *y, HiObject *z);
    virtual void del_subscr(HiObject *x, HiObject *y) { return;}

    virtual HiObject *len(HiObject *x);

    virtual HiObject *contains(HiObject *x, HiObject *y) { return 0; }

    virtual HiObject *getattr(HiObject *x, HiObject *y);
    virtual HiObject *setattr(HiObject *x, HiObject *y, HiObject *z);

    virtual HiObject *next     (HiObject* x) { return 0; }
    virtual HiObject *iter     (HiObject* x) { return 0; }

    virtual HiObject *allocate_instance(HiObject* callable, ArrayList<HiObject *> *args);

    void* operator new(size_t size);

    // gc interfaces
    // this is for objects of this type.
    virtual void oops_do(OopClosure* closure, HiObject* obj);
    // for klass itself only.
    virtual void oops_do(OopClosure* closure);

    virtual size_t size();
};

#endif //PYTHONVM_KLASS_HPP
