//
// Created by Kosho on 2020/8/23.
//

#include "object/hiString.hpp"
#include "runtime/stringTable.hpp"
#include "memory/oopClosure.hpp"

StringTable::StringTable() {
    next_str = new HiString("next");
    mod_str = new HiString("__module__");
    init_str = new HiString("__init__");
    add_str = new HiString("__add__");
    len_str = new HiString("__len__");
    call_str = new HiString("__call__");
    getitem_str = new HiString("__getitem__");
    setitem_str = new HiString("__setitem__");
    getattr_str = new HiString("__getattr__");
    setattr_str = new HiString("__setattr__");
}

StringTable *StringTable::instance = NULL;

StringTable *StringTable::get_instance() {
    if (instance == NULL)
        instance = new StringTable();

    return instance;
}

void StringTable::oops_do(OopClosure *f) {
    f->do_oop((HiObject **) &next_str);
    f->do_oop((HiObject **) &mod_str);
    f->do_oop((HiObject **) &init_str);
    f->do_oop((HiObject **) &add_str);
    f->do_oop((HiObject **) &len_str);
    f->do_oop((HiObject **) &call_str);
    f->do_oop((HiObject **) &getitem_str);
    f->do_oop((HiObject **) &setitem_str);
    f->do_oop((HiObject **) &setattr_str);
    f->do_oop((HiObject **) &getattr_str);
}
