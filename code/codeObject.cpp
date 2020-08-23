//
// Created by Kosho on 2020/8/14.
//

#include "codeObject.hpp"

CodeObject::CodeObject(int argcount, int nlocals, int stacksize, int flag, HiString *bytecodes,
                       ArrayList<HiObject *> *consts, ArrayList<HiObject *> *names, ArrayList<HiObject *> *varnames,
                       ArrayList<HiObject *> *freevars, ArrayList<HiObject *> *cellvars,
                       HiString *file_name, HiString *co_name, int lineno, HiString *notable) :
        _argcount(argcount),
        _nlocals(nlocals),
        _stack_size(stacksize),
        _flag(flag),
        _bytecodes(bytecodes),
        _names(names),
        _consts(consts),
        _var_names(varnames),
        _free_vars(freevars),
        _cell_vars(cellvars),
        _co_name(co_name),
        _file_name(file_name),
        _lineno(lineno),
        _notable(notable) {
}