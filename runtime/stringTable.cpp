//
// Created by Kosho on 2020/8/23.
//

#include "object/hiString.hpp"
#include "runtime/stringTable.hpp"

StringTable::StringTable() {
    next_str = new HiString("next");
}

StringTable* StringTable::instance = NULL;

StringTable *StringTable::get_instance() {
    if (instance == NULL)
        instance = new StringTable();

    return instance;
}
