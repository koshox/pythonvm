//
// Created by Kosho on 2020/8/15.
//

#include "runtime/universe.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/module.hpp"
#include "object/hiInteger.hpp"
#include "object/hiObject.hpp"
#include "object/hiString.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "memory/heap.hpp"
#include "memory/oopClosure.hpp"

HiObject *Universe::HiTrue = NULL;
HiObject *Universe::HiFalse = NULL;

HiObject *Universe::HiNone = NULL;

Heap *Universe::heap = NULL;

CodeObject *Universe::main_code = NULL;

HiObject *Universe::stop_iteration = NULL;
ArrayList<Klass *> *Universe::klasses = NULL;

void Universe::genesis() {
    heap = Heap::get_instance();
    klasses = new ArrayList<Klass *>();

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
    ModuleKlass::get_instance()->initialize();

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
    ModuleKlass::get_instance()->order_supers();

    Interpreter::get_instance()->initialize();
}

void Universe::destory() {}

void Universe::oops_do(OopClosure *closure) {
    closure->do_oop((HiObject **) &HiTrue);
    closure->do_oop((HiObject **) &HiFalse);
    closure->do_oop((HiObject **) &HiNone);

    closure->do_oop((HiObject **) &stop_iteration);
    closure->do_oop((HiObject **) &main_code);
    closure->do_array_list(&klasses);
}
