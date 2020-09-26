//
// Created by Kosho on 2020/9/13.
//
#include <stdlib.h>
#include <runtime/interpreter.hpp>

#include "runtime/universe.hpp"
#include "memory/heap.hpp"
#include "memory/oopClosure.hpp"

Space::Space(size_t size) {
    _size = size;
    _base = (char *) malloc(size);
    _end = _base + size;
    _top = (char *) (((long long) (_base + 15)) & -16); // 16字节对齐
    _capacity = _end - _top;
}

Space::~Space() {
    if (_base) {
        free(_base);
        _base = 0;
    }

    _top = 0;
    _end = 0;
    _capacity = 0;
    _size = 0;
}

bool Space::can_alloc(size_t size) {
    return _capacity > size;
}

bool Space::has_obj(char *obj) {
    return obj >= _base && _end > obj;
}

void *Space::allocate(size_t size) {
    // 8字节对齐
    size = (size + 7) & -8;
    char *start = _top;
    _top += size;
    _capacity -= size;
    return start;
}

void Space::clear() {
    memset(_base, 0, _size);
    _top = (char *) (((long long) (_base + 15)) & -16);
    _capacity = _end - _top;
}

Heap *Heap::instance = NULL;
size_t Heap::MAX_CAP = 2 * 1024 * 1024;

Heap *Heap::get_instance() {
    if (instance == NULL)
        instance = new Heap(MAX_CAP);

    return instance;
}

Heap::Heap(size_t size) {
    mem_1 = new Space(size);
    mem_2 = new Space(size);
    metaspace = new Space(size / 16);

    mem_1->clear();
    mem_2->clear();
    metaspace->clear();

    eden = mem_1;
    survivor = mem_2;
}

Heap::~Heap() {
    if (mem_1) {
        delete mem_1;
        mem_1 = NULL;
    }

    if (mem_2) {
        delete mem_2;
        mem_2 = NULL;
    }

    if (metaspace) {
        delete metaspace;
        metaspace = NULL;
    }

    eden = NULL;
    survivor = NULL;
}

void *Heap::allocate(size_t size) {
    if (!eden->can_alloc(size)) {
        gc();
    }

    return eden->allocate(size);
}

void *Heap::allocate_meta(size_t size) {
    if (!metaspace->can_alloc(size)) {
        return NULL;
    }

    return metaspace->allocate(size);
}

void Heap::copy_live_objects() {
    ScavengeOopClosure* closure = new ScavengeOopClosure(eden, survivor, metaspace);
    closure->scavenge();
    delete closure;
}

void Heap::gc() {
    printf("gc starting...\n");
    printf("  befroe gc : \n");
    printf("  eden's capacity is %lu\n", eden->_capacity);
    copy_live_objects();

    Space *temp = eden;
    eden = survivor;
    survivor = temp;

    printf("  after gc : \n");
    printf("  eden's capacity is %lu\n", eden->_capacity);
    printf("gc end\n");

    survivor->clear();
}
