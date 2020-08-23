//
// Created by Kosho on 2020/8/15.
//

#include "runtime/universe.hpp"
#include "object/hiString.hpp"
#include "functionObject.hpp"

HiObject *Universe::HiTrue = NULL;
HiObject *Universe::HiFalse = NULL;

HiObject *Universe::HiNone = NULL;

void Universe::genesis() {
    HiTrue = new HiString("True");
    HiFalse = new HiString("False");
    HiNone = new HiString("None");

    Map<HiObject *, HiObject *> *klass_dict = new Map<HiObject *, HiObject *>();
    StringKlass::get_instance()->set_klass_dict(klass_dict);
    klass_dict->put(new HiString("upper"), new FunctionObject(string_upper));

    StringKlass::get_instance()->set_name(new HiString("string"));
}

void Universe::destory() {}