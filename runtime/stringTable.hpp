//
// Created by Kosho on 2020/8/23.
//

#ifndef PYTHONVM_STRINGTABLE_HPP
#define PYTHONVM_STRINGTABLE_HPP

class HiString;


class StringTable {
private:
    static StringTable *instance;

    StringTable();

public:
    static StringTable *get_instance();

    HiString *next_str;
    HiString *mod_str;
    HiString* init_str;
};

#endif //PYTHONVM_STRINGTABLE_HPP
