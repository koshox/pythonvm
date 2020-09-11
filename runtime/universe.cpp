//
// Created by Kosho on 2020/8/15.
//

#include "runtime/universe.hpp"
#include "object/hiString.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "functionObject.hpp"

HiObject *Universe::HiTrue = NULL;
HiObject *Universe::HiFalse = NULL;

HiObject *Universe::HiNone = NULL;

void Universe::genesis() {
    HiTrue = new HiString("True");
    HiFalse = new HiString("False");
    HiNone = new HiString("None");

    Klass *object_klass = ObjectKlass::get_instance();
    Klass *type_klass = TypeKlass::get_instance();

    HiTypeObject *tp_obj = new HiTypeObject();
    tp_obj->set_own_klass(type_klass);

    HiTypeObject *obj_obj = new HiTypeObject();
    obj_obj->set_own_klass(object_klass);

    type_klass->add_super(object_klass);
    // do nothing for object klass
    // object_klass->add_super(NULL);

    IntegerKlass::get_instance()->initialize();
    StringKlass::get_instance()->initialize();
    DictKlass::get_instance()->initialize();
    ListKlass::get_instance()->initialize();

    type_klass->set_klass_dict(new HiDict());
    object_klass->set_klass_dict(new HiDict());

    type_klass->set_name(new HiString("type"));
    object_klass->set_name(new HiString("object"));

    IntegerKlass::get_instance()->order_supers();
    StringKlass::get_instance()->order_supers();
    DictKlass::get_instance()->order_supers();
    ListKlass::get_instance()->order_supers();
    type_klass->order_supers();

    FunctionKlass::get_instance()->order_supers();
    NativeFunctionKlass::get_instance()->order_supers();
    MethodKlass::get_instance()->order_supers();
}

void Universe::destory() {}