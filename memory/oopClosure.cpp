//
// Created by Kosho on 2020/9/18.
//

#include "runtime/universe.hpp"
#include "runtime/stringTable.hpp"
#include "runtime/interpreter.hpp"
#include "memory/oopClosure.hpp"
#include "memory/heap.hpp"
#include "util/map.hpp"
#include "util/stack.hpp"
#include "object/hiObject.hpp"

ScavengeOopClosure::ScavengeOopClosure(Space *from, Space *to, Space *meta) {
    _from = from;
    _to = to;
    _meta = meta;

    _oop_stack = new Stack<HiObject *>(1024);
}

ScavengeOopClosure::~ScavengeOopClosure() {
    _from = NULL;
    _to = NULL;

    delete _oop_stack;
    _oop_stack = NULL;
}

void ScavengeOopClosure::do_oop(HiObject **oop) {
    if (oop == NULL || *oop == NULL) {
        return;
    }

    // this oop has been handled, since it may be refered by Klass
    if (!_from->has_obj((char *) *oop)) {
        return;
    }

    (*oop) = copy_and_push(*oop);
}

void ScavengeOopClosure::do_array_list(ArrayList<Klass *> **alist) {
    if (alist == NULL || *alist == NULL) {
        return;
    }

    // no chance to visit list more than once.
    // ArrayList基本只作为HiObject内部属性使用，不会由多个对象同时引用
    assert(_from->has_obj((char *) *alist));

    size_t size = sizeof(ArrayList<Klass *>);
    char *target = (char *) _to->allocate(size);
    memcpy(target, (*alist), size);
    (*(char **) alist) = target;

    (*alist)->oops_do(this);
}

void ScavengeOopClosure::do_array_list(ArrayList<HiObject *> **alist) {
    if (alist == NULL || *alist == NULL) {
        return;
    }

    assert(_from->has_obj((char *) *alist));

    size_t size = sizeof(ArrayList<HiObject *>);
    char *target = (char *) _to->allocate(size);
    memcpy(target, (*alist), size);
    (*(char **) alist) = target;
    (*alist)->oops_do(this);
}

void ScavengeOopClosure::do_map(Map<HiObject *, HiObject *> **amap) {
    if (amap == NULL || *amap == NULL) {
        return;
    }

    assert(_from->has_obj((char *) *amap));

    size_t size = sizeof(Map<HiObject *, HiObject *>);
    char *target = (char *) _to->allocate(size);
    memcpy(target, (*amap), size);
    (*(char **) amap) = target;
    (*amap)->oops_do(this);
}

void ScavengeOopClosure::do_raw_mem(char **mem, int length) {
    if (*mem == NULL) {
        return;
    }

    char *target = (char *) _to->allocate(length);
    memcpy(target, (*mem), length);
    (*mem) = target;
}

void ScavengeOopClosure::do_klass(Klass **k) {
    if (k == NULL || *k == NULL) {
        return;
    }

    (*k)->oops_do(this);
}

HiObject *ScavengeOopClosure::copy_and_push(HiObject *obj) {
    char *target = obj->new_address();
    if (target) {
        // 如果已经move到了to空间, 那直接返回forwarding指针
        return (HiObject *) target;
    }

    // copy
    size_t size = obj->size();
    target = (char *) _to->allocate(size);
    memcpy(target, obj, size);
    obj->set_new_address(target);

    // push
    _oop_stack->push((HiObject *) target);

    return (HiObject *) target;
}

void ScavengeOopClosure::scavenge() {
    // step1, mark roots
    process_roots();

    // step2, process all objects
    while (!_oop_stack->empty()) {
        _oop_stack->pop()->oops_do(this);
    }
}

void ScavengeOopClosure::process_roots() {
    Universe::oops_do(this);
    Interpreter::get_instance()->oops_do(this);
    StringTable::get_instance()->oops_do(this);
}
