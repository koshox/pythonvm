//
// Created by Kosho on 2020/8/14.
//

#ifndef PYTHONVM_HIOBJECT_HPP
#define PYTHONVM_HIOBJECT_HPP

#include "klass.hpp"

class HiString;

class ObjectKlass : public Klass {
private:
    ObjectKlass();
    static ObjectKlass *instance;

public:
    static ObjectKlass *get_instance();
    // TODO oops?
};

class HiObject {
private:
    long _mark_word;
    Klass *_klass;
    HiDict *_obj_dict = NULL;

public:
    Klass *klass() {
        assert(_klass != NULL);
        return _klass;
    }

    void set_klass(Klass *x) { _klass = x; }

    void print();

    HiObject *add(HiObject *x);
    HiObject *sub(HiObject *x);
    HiObject *mul(HiObject *x);
    HiObject *div(HiObject *x);
    HiObject *mod(HiObject *x);

    HiObject *greater(HiObject *x);
    HiObject *less(HiObject *x);
    HiObject *equal(HiObject *x);
    HiObject *not_equal(HiObject *x);
    HiObject *ge(HiObject *x);
    HiObject *le(HiObject *x);

    HiObject *getattr(HiObject *x);
    HiObject *setattr(HiObject *x, HiObject *y);

    HiObject *subscr(HiObject *x);
    HiObject *contains(HiObject *x);
    void store_subscr(HiObject *x, HiObject *y);
    void del_subscr(HiObject *x);

    HiObject *iter();
    HiObject *next();
    HiObject *len();

    HiDict *obj_dict() { return _obj_dict; }
    void set_obj_dict(HiDict *x) { _obj_dict = x; }
    void init_dict();

    void *operator new(size_t size);

    // interfaces for GC.
    void oops_do(OopClosure *closure);
    size_t size();
    char *new_address();
    void set_new_address(char *addr);
};

/**
 * Meta klass for object system
 */
class TypeKlass : public Klass {
private:
    TypeKlass() {}

    static TypeKlass *instance;

public:
    static TypeKlass *get_instance();

    virtual void print(HiObject *obj);

    virtual HiObject *setattr(HiObject *x, HiObject *y, HiObject *z);

    virtual size_t size();

    virtual void oops_do(OopClosure *f, HiObject *obj);
};

class HiTypeObject : public HiObject {
private:
    Klass *_own_klass;

public:
    HiTypeObject();

    void set_own_klass(Klass *k);

    Klass *own_klass() { return _own_klass; }
};

#endif //PYTHONVM_HIOBJECT_HPP
