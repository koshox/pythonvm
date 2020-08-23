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

    virtual HiObject* subscr (HiObject* x, HiObject* y);
    virtual HiObject* iter(HiObject* x);
    virtual void print(HiObject* obj);
    virtual void store_subscr(HiObject* x, HiObject* y, HiObject* z);
    virtual void del_subscr (HiObject* x, HiObject* y);

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

#endif //PYTHONVM_HIDICT_HPP
