//
// Created by Kosho on 2020/8/28.
//

#include "runtime/cellObject.hpp"
#include "object/hiList.hpp"
#include "object/hiDict.hpp"
#include "object/hiString.hpp"
#include "memory/oopClosure.hpp"

CellKlass *CellKlass::_instance = NULL;

CellKlass *CellKlass::get_instance() {
    if (_instance == NULL) {
        _instance = new CellKlass();
    }

    return _instance;
}

CellKlass::CellKlass() {
    set_klass_dict(new HiDict());
    set_name(new HiString("cell"));
}

CellObject::CellObject(HiList *t, int i) :
        _table(t),
        _index(i) {
    set_klass(CellKlass::get_instance());
}

HiObject *CellObject::value() {
    return _table->get(_index);
}

size_t CellKlass::size() {
    return sizeof(CellKlass);
}

void CellKlass::oops_do(OopClosure *f, HiObject *obj) {
    CellObject *co = (CellObject *) obj;
    f->do_oop((HiObject **) &co->_table);
}
