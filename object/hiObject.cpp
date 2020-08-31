//
// Created by Kosho on 2020/8/15.
//

#include "runtime/universe.hpp"
#include "runtime/stringTable.hpp"
#include "hiObject.hpp"
#include "object/hiDict.hpp"
#include "object/hiString.hpp"
#include "runtime/universe.hpp"
#include "runtime/functionObject.hpp"

ObjectKlass *ObjectKlass::instance = NULL;

ObjectKlass::ObjectKlass() {
}

ObjectKlass *ObjectKlass::get_instance() {
    if (instance == NULL)
        instance = new ObjectKlass();

    return instance;
}

void HiObject::print() {
    klass()->print(this);
}

HiObject *HiObject::greater(HiObject *rhs) {
    return klass()->greater(this, rhs);
}

HiObject *HiObject::add(HiObject *rhs) {
    return klass()->add(this, rhs);
}

HiObject *HiObject::sub(HiObject *rhs) {
    return klass()->sub(this, rhs);
}

HiObject *HiObject::mul(HiObject *rhs) {
    return klass()->mul(this, rhs);
}

HiObject *HiObject::div(HiObject *rhs) {
    return klass()->div(this, rhs);
}

HiObject *HiObject::mod(HiObject *rhs) {
    return klass()->mod(this, rhs);
}

HiObject *HiObject::less(HiObject *rhs) {
    return klass()->less(this, rhs);
}

HiObject *HiObject::equal(HiObject *rhs) {
    return klass()->equal(this, rhs);
}

HiObject *HiObject::not_equal(HiObject *rhs) {
    return klass()->not_equal(this, rhs);
}

HiObject *HiObject::ge(HiObject *rhs) {
    return klass()->ge(this, rhs);
}

HiObject *HiObject::le(HiObject *rhs) {
    return klass()->le(this, rhs);
}

HiObject *HiObject::get_attr(HiObject *x) {
    HiObject *result = Universe::HiNone;
    result = klass()->klass_dict()->get(x);

    if (result == Universe::HiNone) {
        return result;
    }

    if (MethodObject::is_function(result)) {
        result = new MethodObject((FunctionObject *) result, this);
    }

    return result;
}

// TODO
HiObject *HiObject::set_attr(HiObject *x, HiObject *y) {
    // return klass()->klass_dict()->put(x, y);
    return NULL;
}

HiObject *HiObject::subscr(HiObject *x) {
    return klass()->subscr(this, x);
}

HiObject *HiObject::contains(HiObject *x) {
    return klass()->contains(this, x);
}

void HiObject::store_subscr(HiObject *x, HiObject *y) {
    return klass()->store_subscr(this, x, y);
}

void HiObject::del_subscr(HiObject *x) {
    return klass()->del_subscr(this, x);
}

HiObject *HiObject::iter() {
    return klass()->iter(this);
}

HiObject *HiObject::next() {
    return klass()->next(this);
}

HiObject *HiObject::len() {
    return klass()->len(this);
}

TypeKlass *TypeKlass::instance = NULL;

TypeKlass *TypeKlass::get_instance() {
    if (instance == NULL)
        instance = new TypeKlass();

    return instance;
}

void TypeKlass::print(HiObject *obj) {
    assert(obj->klass() == (Klass *) this);
    printf("<type '");
    Klass *own_klass = ((HiTypeObject *) obj)->own_klass();

    HiDict *attr_dict = own_klass->klass_dict();
    if (attr_dict) {
        HiObject *mod = attr_dict->get((HiObject *) StringTable::get_instance()->mod_str);
        if (mod != Universe::HiNone) {
            mod->print();
            printf(".");
        }
    }

    own_klass->name()->print();
    printf("'>");
}

HiTypeObject::HiTypeObject() {
    set_klass(TypeKlass::get_instance());
}

void HiTypeObject::set_own_klass(Klass *k) {
    _own_klass = k;
    k->set_type_object(this);
}
