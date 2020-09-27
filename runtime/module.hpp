//
// Created by Kosho on 2020/9/26.
//

#ifndef PYTHONVM_MODULE_HPP
#define PYTHONVM_MODULE_HPP

#include "object/hiObject.hpp"

class HiDict;

class OopClosure;

class ModuleKlass : public Klass {
private:
    static ModuleKlass *_instance;

    ModuleKlass();

public:
    static ModuleKlass *get_instance();

    void initialize();

    virtual void oops_do(OopClosure *closure, HiObject *obj);

    virtual size_t size();
};

class ModuleObject : public HiObject {
    friend class ModuleKlass;

private:
    HiString *_mod_name;

public:
    ModuleObject(HiDict *x);

    static ModuleObject *import_module(HiObject *mod_name);

    static ModuleObject *import_so(HiString *mod_name);

    void put(HiObject *x, HiObject *y);

    HiObject *get(HiObject *x);

    void extend(ModuleObject *mo);
};

#endif //PYTHONVM_MODULE_HPP
