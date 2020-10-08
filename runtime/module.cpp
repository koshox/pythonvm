//
// Created by Kosho on 2020/9/26.
//

#include <unistd.h>
#include <string>
#include <sstream>

#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "runtime/module.hpp"
#include "runtime/universe.hpp"
#include "runtime/interpreter.hpp"
#include "runtime/stringTable.hpp"
#include "util/bufferedInputStream.hpp"
#include "code/binaryFileParser.hpp"
#include "memory/oopClosure.hpp"

#define ST(x) StringTable::get_instance()->STR(x)
#define STR(x) x##_str

ModuleKlass *ModuleKlass::_instance = NULL;

ModuleKlass *ModuleKlass::get_instance() {
    if (_instance == NULL)
        _instance = new ModuleKlass();

    return _instance;
}

ModuleKlass::ModuleKlass() {
}

void ModuleKlass::initialize() {
    HiDict *dict = new HiDict();
    set_klass_dict(dict);
    set_name(new HiString("module"));
    (new HiTypeObject())->set_own_klass(this);
    add_super(ObjectKlass::get_instance());
}

void ModuleKlass::oops_do(OopClosure *closure, HiObject *obj) {
    void *temp = &(((ModuleObject *) obj)->_mod_name);
    closure->do_oop((HiObject **) temp);
}

size_t ModuleKlass::size() {
    return sizeof(ModuleObject);
}

ModuleObject::ModuleObject(HiDict *x) {
    set_obj_dict(x);
    set_klass(ModuleKlass::get_instance());
}

ModuleObject *ModuleObject::import_module(HiString *cur_path, HiObject *x) {
    HiString *mod_name = (HiString *) x;
    HiString *file_name = (HiString *) (mod_name->add(ST(pyc_suf)));

    HiList* path_list = new HiList();
    path_list->append(cur_path);

        // TODO path
    std::stringstream ss;
    ss << "D:\\code\\mine\\cpp\\pythonvm\\test\\" <<  file_name->value();
    const char *file_path = ss.str().c_str();
    if (access(file_path, R_OK) == -1) {
        return NULL;
    }

    // BufferedInputStream stream(file_name->value());
    BufferedInputStream stream(file_path);
    BinaryFileParser parser(&stream);
    CodeObject *mod_code = parser.parse();
    HiDict *mod_dict = Interpreter::get_instance()->run_mod(mod_code, mod_name);
    return new ModuleObject(mod_dict);
}

ModuleObject *ModuleObject::import_so(HiString *mod_name) {
    // TODO
    return nullptr;
}

void ModuleObject::put(HiObject *x, HiObject *y) {
    obj_dict()->put(x, y);
}

HiObject *ModuleObject::get(HiObject *x) {
    return obj_dict()->get(x);
}

void ModuleObject::extend(ModuleObject *mo) {
    obj_dict()->update(mo->obj_dict());
}
