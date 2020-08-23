//
// Created by Kosho on 2020/8/23.
//

#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/universe.hpp"
#include "runtime/stringTable.hpp"
#include <assert.h>

DictKlass* DictKlass::instance = NULL;

DictKlass::DictKlass() {
    Map<HiObject *, HiObject *> *dict = new Map<HiObject *, HiObject*>();
    set_name(new HiString("dict"));
}

DictKlass *DictKlass::get_instance() {
    if (instance == NULL) {
        instance = new DictKlass();
    }

    return instance;
}

HiObject *DictKlass::subscr(HiObject *x, HiObject *y) {
    assert(x && x->klass() == (Klass*) this);
    return ((HiDict*)x)->map()->get(y);
}

HiObject *DictKlass::iter(HiObject *x) {
    return Klass::iter(x);
}

void DictKlass::print(HiObject *obj) {
    HiDict* dict_obj = (HiDict*) obj;
    assert(dict_obj && dict_obj->klass() == (Klass*) this);

    printf("{");
    int size = dict_obj->_map->size();
    if (size >= 1) {
        dict_obj->_map->entries()[0]._k->print();
        printf(":");
        dict_obj->_map->entries()[0]._v->print();
    }

    for (int i = 1; i < size; i++) {
        printf(", ");
        dict_obj->_map->entries()[i]._k->print();
        printf(":");
        dict_obj->_map->entries()[i]._v->print();
    }

    printf("}");
}

void DictKlass::store_subscr(HiObject *x, HiObject *y, HiObject *z) {
    assert(x && x->klass() == (Klass*) this);
    ((HiDict*)x)->put(y, z);
}

void DictKlass::del_subscr(HiObject *x, HiObject *y) {
    assert(x && x->klass() == (Klass*) this);
    ((HiDict*)x)->remove(y);
}

size_t DictKlass::size() {
    return sizeof(HiDict);
}

HiDict::HiDict() {
    _map = new Map<HiObject*, HiObject*>();
    set_klass(DictKlass::get_instance());
}

HiDict::HiDict(Map<HiObject *, HiObject *> *x) {
    _map = x;
    set_klass(DictKlass::get_instance());
}
