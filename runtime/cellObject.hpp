//
// Created by Kosho on 2020/8/28.
//

#ifndef PYTHONVM_CELLOBJECT_HPP
#define PYTHONVM_CELLOBJECT_HPP

#include "object/klass.hpp"
#include "object/hiObject.hpp"

class CellKlass : public Klass {
private:
    CellKlass();

    static CellKlass *_instance;

public:
    static CellKlass *get_instance();

    virtual size_t size();
    virtual void oops_do(OopClosure *f, HiObject *obj);
};

// 这种方式STORE_DEREF修改后仍然可见
class CellObject : public HiObject {
    friend class CellKlass;

private:
    HiList *_table;
    int _index;

public:
    CellObject(HiList *l, int i);

    HiObject *value();
};

#endif //PYTHONVM_CELLOBJECT_HPP
