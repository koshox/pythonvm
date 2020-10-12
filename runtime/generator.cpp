//
// Created by Kosho on 2020/10/13.
//

#include "runtime/generator.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/frameObject.hpp"
#include "runtime/interpreter.hpp"
#include "code/codeObject.hpp"
#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "object/hiString.hpp"
#include "memory/oopClosure.hpp"

GeneratorKlass *GeneratorKlass::instance = NULL;

GeneratorKlass::GeneratorKlass() {
    HiDict *klass_dict = new HiDict();
    klass_dict->put(new HiString("next"), new FunctionObject(generator_next));
    set_klass_dict(klass_dict);
}

GeneratorKlass *GeneratorKlass::get_instance() {
    if (instance == NULL)
        instance = new GeneratorKlass();

    return instance;
}

HiObject *GeneratorKlass::iter(HiObject *obj) {
    return obj;
}

HiObject *GeneratorKlass::next(HiObject *obj) {
    assert(obj->klass() == (Klass *) this);
    Generator *g = (Generator *) obj;
    return Interpreter::get_instance()->eval_generator(g);
}

size_t GeneratorKlass::size() {
    return sizeof(Generator);
}

void GeneratorKlass::oops_do(OopClosure *f, HiObject *obj) {
    Generator *g = (Generator *) obj;
    assert(g->klass() == (Klass *) this);

    if (g->frame())
        g->frame()->oops_do(f);
}

Generator::Generator(FunctionObject *func, ArrayList<HiObject *> *args, int arg_cnt) {
    _frame = new FrameObject(func, args, arg_cnt);
    set_klass(GeneratorKlass::get_instance());
}

HiObject *generator_next(ObjList args) {
    Generator *generator = (Generator *) (args->get(0));
    return generator->next();
}
