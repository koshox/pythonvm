//
// Created by Kosho on 2020/8/23.
//

#ifndef PYTHONVM_HIDICT_HPP
#define PYTHONVM_HIDICT_HPP

#include "object/hiObject.hpp"
#include "util/map.hpp"

class DictKlass : public Klass {
private:
    DictKlass();
    static DictKlass* instance;

public:
    static DictKlass* get_instance();
    void initialize();

    virtual HiObject* subscr (HiObject* x, HiObject* y);
    virtual HiObject* iter(HiObject* x);
    virtual void print(HiObject* obj);
    virtual void store_subscr(HiObject* x, HiObject* y, HiObject* z);
    virtual void del_subscr (HiObject* x, HiObject* y);

    virtual HiObject *allocate_instance(ArrayList<HiObject *> *args);

    virtual size_t size();
};

class HiDict : public HiObject {
    friend class DictKlass;
private:
    Map<HiObject*, HiObject*>* _map;

public:
    HiDict();
    HiDict(Map<HiObject*, HiObject*>* map);

    Map<HiObject*, HiObject*>* map()   { return _map; }
    void put(HiObject* k, HiObject* v) { _map->put(k, v); }
    HiObject* get(HiObject* k)         { return _map->get(k); }
    bool      has_key(HiObject* k)     { return _map->has_key(k); }
    int       size()                   { return _map->size(); }
    HiObject* remove(HiObject* k)      { return _map->remove(k); }
};

HiObject* dict_set_default(ObjList args);
HiObject* dict_remove(ObjList args);
HiObject* dict_pop(ObjList args);
HiObject* dict_keys(ObjList args);
HiObject* dict_values(ObjList args);
HiObject* dict_items(ObjList args);
HiObject* dict_iterkeys(ObjList args);
HiObject* dict_itervalues(ObjList args);
HiObject* dict_iteritems(ObjList args);

enum ITER_TYPE {
    ITER_KEY = 0,
    ITER_VALUE,
    ITER_ITEM
};

template<ITER_TYPE n>
class DictIteratorKlass : public Klass {
private:
    static DictIteratorKlass *instance;
    DictIteratorKlass();

public:
    static DictIteratorKlass *get_instance();
    virtual HiObject *iter(HiObject *x) { return x; }
    virtual HiObject *next(HiObject *x);
};

class DictIterator : public HiObject {
private:
    HiDict *_owner;
    int _iter_cnt;
public:
    DictIterator(HiDict *owner);

    HiDict *owner() { return _owner; }
    int iter_cnt() { return _iter_cnt; }
    void inc_cnt() { _iter_cnt++; }
};

// TODO remove
HiObject* dictiterator_next(ObjList args);

#endif //PYTHONVM_HIDICT_HPP
