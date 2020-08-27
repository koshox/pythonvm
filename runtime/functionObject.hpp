//
// Created by Kosho on 2020/8/16.
//

#ifndef PYTHONVM_FUNCTIONOBJECT_HPP
#define PYTHONVM_FUNCTIONOBJECT_HPP

#include "util/map.hpp"
#include "code/codeObject.hpp"
#include "object/klass.hpp"

class FunctionKlass : public Klass {
private:
    FunctionKlass();

    static FunctionKlass *instance;

public:
    static FunctionKlass *get_instance();

    virtual void print(HiObject *obj);
};

typedef HiObject *(*NativeFuncPointer)(ObjList args);

class FunctionObject : public HiObject {
    friend class FunctionKlass;
    friend class NativeFunctionKlass;
    friend class FrameObject;

private:
    CodeObject *_func_code;
    HiString *_func_name;
    HiDict *_globals;
    ObjList _defaults;

    NativeFuncPointer _native_func;

    unsigned int _flags;

public:
    enum CO_FLAGS {
        CO_VARARGS = 0x4,
        CO_VARKEYWORDS = 0x8,
        CO_GENERATOR = 0x20,
    };

    FunctionObject(HiObject *code_object);

    FunctionObject(NativeFuncPointer nfp);

    FunctionObject(Klass *klass) {
        _func_code = NULL;
        _func_name = NULL;
        _globals   = NULL;
        _defaults  = NULL;
        _flags = 0;

        set_klass(klass);
    }

    HiString *func_name() { return _func_name; }
    int flags() { return _flags; }

    HiDict *globals() { return _globals; }
    void set_globals(HiDict *x) { _globals = x; }

    void set_default(ObjList defaults);
    ObjList defaults() { return _defaults; }

    HiObject *call(ObjList args);
};

class NativeFunctionKlass : public Klass {
private:
    NativeFunctionKlass();

    static NativeFunctionKlass *instance;

public:
    static NativeFunctionKlass *get_instance();
};

class MethodKlass : public Klass {
private:
    MethodKlass();
    static MethodKlass* instance;

public:
    static MethodKlass* get_instance();
};

class MethodObject : public HiObject {
    friend class MethodKlass;

private:
    HiObject* _owner;
    FunctionObject *_func;

public:
    MethodObject(FunctionObject *func) : _owner(NULL), _func(func) {
        set_klass(MethodKlass::get_instance());
    }

    MethodObject(FunctionObject *func, HiObject *owner) : _owner(owner), _func(func) {
        set_klass(MethodKlass::get_instance());
    }

    void set_owner(HiObject *x) { _owner = x; }
    HiObject *owner() { return _owner; }
    FunctionObject *func() { return _func; }

    static bool is_function(HiObject *x);
};

HiObject* len(ObjList args);

#endif //PYTHONVM_FUNCTIONOBJECT_HPP
