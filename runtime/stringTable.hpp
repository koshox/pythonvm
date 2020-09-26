//
// Created by Kosho on 2020/8/23.
//

#ifndef PYTHONVM_STRINGTABLE_HPP
#define PYTHONVM_STRINGTABLE_HPP

class HiString;

class OopClosure;

class StringTable {
private:
    static StringTable *instance;

    StringTable();

public:
    static StringTable *get_instance();

    HiString *next_str;
    HiString *mod_str;
    HiString *init_str;
    HiString *add_str;
    HiString *len_str;
    HiString *call_str;
    HiString *getitem_str;
    HiString *setitem_str;
    HiString *getattr_str;
    HiString *setattr_str;

    void oops_do(OopClosure *f);
};

#endif //PYTHONVM_STRINGTABLE_HPP
