//
// Created by Kosho on 2020/8/16.
//

#include <runtime/interpreter.hpp>
#include "runtime/stringTable.hpp"
#include "runtime/functionObject.hpp"
#include "klass.hpp"
#include "object/hiObject.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "runtime/universe.hpp"

Klass::Klass() {
    _name = NULL;
}

int Klass::compare_klass(Klass *x, Klass *y) {
    if (x == y) {
        return 0;
    }

    if (x == IntegerKlass::get_instance()) {
        return -1;
    } else if (y == IntegerKlass::get_instance()) {
        return 1;
    }

    if (x->name()->less(y->name()) == (HiObject *) Universe::HiTrue) {
        return -1;
    } else {
        return 1;
    }
}

HiObject *Klass::create_klass(HiObject *x, HiObject *supers, HiObject *name) {
    assert(x->klass() == (Klass *) DictKlass::get_instance());
    assert(supers->klass() == (Klass *) ListKlass::get_instance());
    assert(name->klass() == (Klass *) StringKlass::get_instance());

    Klass *new_klass = new Klass();
    HiDict *klass_dict = (HiDict *) x;
    HiList *supers_list = (HiList *) supers;

    new_klass->set_klass_dict(klass_dict);
    new_klass->set_name((HiString *) name);
    // TODO 暂时先处理单继承
    if (supers_list->inner_list()->length() > 0) {
        HiTypeObject *super = (HiTypeObject *) supers_list->inner_list()->get(0);
        new_klass->set_super(super->own_klass());
    }

    HiTypeObject *type_obj = new HiTypeObject();
    type_obj->set_own_klass(new_klass);

    return type_obj;
}

HiObject *Klass::allocate_instance(HiObject *callable, ArrayList<HiObject *> *args) {
    HiObject *instance = new HiObject();
    instance->set_klass(((HiTypeObject *) callable)->own_klass());
    HiObject *constructor = instance->get_attr(StringTable::get_instance()->init_str);
    if (constructor != Universe::HiNone) {
        Interpreter::get_instance()->call_virtual(constructor, args);
    }

    return instance;
}

HiObject *Klass::getattr(HiObject *x, HiObject *y) {
    HiObject *result = Universe::HiNone;

    if (x->obj_dict() != NULL) {
        result = x->obj_dict()->get(y);
        if (result != Universe::HiNone) {
            return result;
        }
    }

    result = x->klass()->klass_dict()->get(y);

    if (result == Universe::HiNone) {
        return result;
    }

    if (MethodObject::is_function(result)) {
        result = new MethodObject((FunctionObject *)result, x);
    }

    return result;
}

HiObject *Klass::setattr(HiObject *x, HiObject *y, HiObject *z) {
    if (x->obj_dict() == NULL) {
        x->init_dict();
    }

    x->obj_dict()->put(y, z);
    return Universe::HiNone;
}
