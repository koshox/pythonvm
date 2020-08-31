//
// Created by Kosho on 2020/8/15.
//

#include "runtime/universe.hpp"
#include "object/hiString.hpp"
#include "object/hiDict.hpp"
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
    type_klass->set_super(object_klass);

    HiTypeObject *obj_obj = new HiTypeObject();
    obj_obj->set_own_klass(object_klass);
    object_klass->set_super(NULL);

    DictKlass::get_instance()->initialize();
    StringKlass::get_instance()->initialize();

    type_klass->set_name(new HiString("type"));
    object_klass->set_name(new HiString("object"));
}

void Universe::destory() {}