//
// Created by Kosho on 2020/9/26.
//

#include <unistd.h>
#include <string>
#include <dlfcn.h>

#include "inc/koshox.hpp"
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

    HiList *so_list = new HiList();
    so_list->append(ST(libdir_pre));
    so_list->append(mod_name);
    so_list->append(ST(so_suf));
    HiString *file_name = ST(empty)->join(so_list);

    // 查找so
    if (access(file_name->value(), R_OK) == 0) {
        return import_so(mod_name);
    }

    file_name = (HiString *) (mod_name->add(ST(pyc_suf)));

    // 从当前code的路径下查找
    std::string cur_path_str = cur_path->value();
    std::string mod_path_str;
    const size_t last_slash_idx = cur_path_str.rfind('/');
    if (std::string::npos != last_slash_idx) {
        mod_path_str = cur_path_str.substr(0, last_slash_idx);
    }
    mod_path_str = mod_path_str + "/" + file_name->value();
    HiString *file_path = new HiString(mod_path_str.c_str());

    if (access(file_path->value(), R_OK) == -1) {
        // 从/lib下查找
        HiList *pyc_list = new HiList();
        pyc_list->append(ST(libdir_pre));
        pyc_list->append(mod_name);
        pyc_list->append(ST(pyc_suf));
        file_path = ST(empty)->join(pyc_list);
    }

    assert(access(file_path->value(), R_OK) == 0);

    BufferedInputStream stream(file_path->value());
    BinaryFileParser parser(&stream);
    CodeObject *mod_code = parser.parse();
    HiDict *mod_dict = Interpreter::get_instance()->run_mod(mod_code, mod_name);
    return new ModuleObject(mod_dict);
}

ModuleObject *ModuleObject::import_so(HiString *mod_name) {
    char *error_msg = NULL;

    HiString *prefix = ST(libdir_pre);
    HiString *so_suffix = ST(so_suf);

    HiString *file_name = (HiString *) (prefix->add(mod_name)->add(so_suffix));
    void *handle = dlopen(file_name->value(), RTLD_NOW);
    if (handle == NULL) {
        printf("error to open file: %s\n", dlerror());
        return NULL;
    }

    HiString *method_prefix = new HiString("init_");
    HiString *init_meth = (HiString *) (method_prefix->add(mod_name));
    INIT_FUNC init_func = (INIT_FUNC) dlsym(handle, init_meth->value());
    if ((error_msg = dlerror()) != NULL) {
        printf("Symbol init_methods not found: %s\n", error_msg);
        dlclose(handle);
        return NULL;
    }

    RGMethod *methods = init_func();
    ModuleObject *mod = new ModuleObject(new HiDict());
    for (; methods->method_name != NULL; methods++) {
        mod->put(new HiString(methods->method_name), new FunctionObject(methods->method));
    }

    return mod;
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
