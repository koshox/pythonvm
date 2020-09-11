//
// Created by Kosho on 2020/8/16.
//

#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"

FunctionKlass *FunctionKlass::instance = NULL;

FunctionKlass::FunctionKlass() {
    add_super(ObjectKlass::get_instance());
    set_name(new HiString("function"));
    HiTypeObject *tp_obj = new HiTypeObject();
    tp_obj->set_own_klass(this);
}

FunctionKlass *FunctionKlass::get_instance() {
    if (instance == NULL)
        instance = new FunctionKlass();

    return instance;
}

void FunctionKlass::print(HiObject *obj) {
    printf("<function : ");
    FunctionObject *fo = static_cast<FunctionObject *>(obj);

    assert(fo && fo->klass() == (Klass *) this);
    fo->func_name()->print();
    printf(">");
}

FunctionObject::FunctionObject(HiObject *code_object) {
    CodeObject *co = (CodeObject *) code_object;

    _func_code = co;
    _func_name = co->_co_name;
    _flags = co->_flag;
    _globals = NULL;
    _closure = NULL;

    set_klass(FunctionKlass::get_instance());
}

FunctionObject::FunctionObject(NativeFuncPointer nfp) {
    _func_code = NULL;
    _func_name = NULL;
    _flags = 0;
    _globals = NULL;
    _closure = NULL;
    _native_func = nfp;

    set_klass(NativeFunctionKlass::get_instance());
}

void FunctionObject::set_default(ArrayList<HiObject *> *defaults) {
    if (defaults == NULL) {
        _defaults = NULL;
        return;
    }

    _defaults = new ArrayList<HiObject *>(defaults->length());

    for (int i = 0; i < defaults->length(); i++) {
        _defaults->set(i, defaults->get(i));
    }
}

HiObject *FunctionObject::call(ObjList args) {
    return (*_native_func)(args);
}

NativeFunctionKlass *NativeFunctionKlass::instance = NULL;

NativeFunctionKlass *NativeFunctionKlass::get_instance() {
    if (instance == NULL)
        instance = new NativeFunctionKlass();

    return instance;
}

NativeFunctionKlass::NativeFunctionKlass() {
    add_super(FunctionKlass::get_instance());
    set_name(new HiString("native function"));
    HiTypeObject *tp_obj = new HiTypeObject();
    tp_obj->set_own_klass(this);
}

MethodKlass::MethodKlass() {
    add_super(FunctionKlass::get_instance());
    set_name(new HiString("method"));
    HiTypeObject *tp_obj = new HiTypeObject();
    tp_obj->set_own_klass(this);
}

MethodKlass *MethodKlass::instance = NULL;

MethodKlass *MethodKlass::get_instance() {
    if (instance == NULL)
        instance = new MethodKlass();

    return instance;
}

bool MethodObject::is_function(HiObject *x) {
    Klass *k = x->klass();
    if (k == (Klass *) FunctionKlass::get_instance()) {
        return true;
    }

    if (k->mro() == NULL) {
        return false;
    }

    for (int i = 0; i < k->mro()->size(); i++) {
        if (k->mro()->get(i) ==
            FunctionKlass::get_instance()->type_object()) {
            return true;
        }
    }

    return false;
}

HiObject *len(ObjList args) {
    return args->get(0)->len();
}

HiObject *isinstance(ObjList args) {
    HiObject *x = args->get(0);
    HiObject *y = args->get(1);

    assert(y && y->klass() == TypeKlass::get_instance());

    Klass *k = x->klass();
    if (k->type_object() == y)
        return Universe::HiTrue;

    for (int i = 0; i < k->mro()->size(); i++) {
        if (k->mro()->get(i) == y) {
            return Universe::HiTrue;
        }
    }

    return Universe::HiFalse;
}

HiObject *type_of(ObjList args) {
    HiObject *arg0 = args->get(0);
    return arg0->klass()->type_object();
}

HiObject *string_upper(ObjList args) {
    HiObject *arg0 = args->get(0);
    assert(arg0->klass() == StringKlass::get_instance());

    HiString *str_obj = (HiString *) arg0;

    int length = str_obj->length();
    if (length <= 0)
        return Universe::HiNone;

    char *v = new char[length];
    char c;
    for (int i = 0; i < length; i++) {
        c = str_obj->value()[i];
        if (c >= 'a' && c <= 'z')
            v[i] = c - 0x20;
        else
            v[i] = c;
    }

    HiString *s = new HiString(v, length);
    delete[]v;
    return s;
}