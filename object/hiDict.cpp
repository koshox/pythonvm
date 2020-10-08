//
// Created by Kosho on 2020/8/23.
//

#include "object/hiDict.hpp"
#include "object/hiList.hpp"
#include "object/hiInteger.hpp"
#include "object/hiString.hpp"
#include "runtime/functionObject.hpp"
#include "runtime/universe.hpp"
#include "runtime/stringTable.hpp"
#include "memory/oopClosure.hpp"
#include <assert.h>

DictKlass *DictKlass::instance = NULL;

DictKlass::DictKlass() {
}

DictKlass *DictKlass::get_instance() {
    if (instance == NULL) {
        instance = new DictKlass();
    }

    return instance;
}

void DictKlass::initialize() {
    HiDict *klass_dict = new HiDict();
    klass_dict->put(new HiString("setdefault"), new FunctionObject(dict_set_default));
    klass_dict->put(new HiString("remove"),
                    new FunctionObject(dict_remove));
    klass_dict->put(new HiString("pop"),
                    new FunctionObject(dict_pop));
    klass_dict->put(new HiString("keys"),
                    new FunctionObject(dict_keys));
    klass_dict->put(new HiString("values"),
                    new FunctionObject(dict_values));
    klass_dict->put(new HiString("items"),
                    new FunctionObject(dict_items));
    klass_dict->put(new HiString("iterkeys"),
                    new FunctionObject(dict_iterkeys));
    klass_dict->put(new HiString("itervalues"),
                    new FunctionObject(dict_itervalues));
    klass_dict->put(new HiString("iteritems"),
                    new FunctionObject(dict_iteritems));
    set_klass_dict(klass_dict);
    set_name(new HiString("dict"));
    (new HiTypeObject())->set_own_klass(this);
    add_super(ObjectKlass::get_instance());
}

/*
 * Iterations for dict object
 */
template<ITER_TYPE n>
DictIteratorKlass<n> *DictIteratorKlass<n>::instance = NULL;

template<ITER_TYPE n>
DictIteratorKlass<n> *DictIteratorKlass<n>::get_instance() {
    if (instance == NULL) {
        instance = new DictIteratorKlass<n>();
    }

    return instance;
}

template<ITER_TYPE iter_type>
DictIteratorKlass<iter_type>::DictIteratorKlass() {
    const char *klass_names[] = {
            "dictionary-keyiterator",
            "dictionary-valueiterator",
            "dictionary-itemiterator",
    };
    HiDict *klass_dict = new HiDict();
    klass_dict->put(new HiString("next"),
                    new FunctionObject(dictiterator_next));
    set_klass_dict(klass_dict);
    set_name(new HiString(klass_names[iter_type]));
}

DictIterator::DictIterator(HiDict *dict) {
    _owner = dict;
    _iter_cnt = 0;
}

HiObject *DictKlass::subscr(HiObject *x, HiObject *y) {
    assert(x && x->klass() == (Klass *) this);
    return ((HiDict *) x)->map()->get(y);
}

HiObject *DictKlass::iter(HiObject *x) {
    HiObject *it = new DictIterator((HiDict *) x);
    it->set_klass(DictIteratorKlass<ITER_KEY>::get_instance());
    return it;
}

void DictKlass::print(HiObject *obj) {
    HiDict *dict_obj = (HiDict *) obj;
    assert(dict_obj && dict_obj->klass() == (Klass *) this);

    printf("{");
    int size = dict_obj->_map->size();
    if (size >= 1) {
        dict_obj->_map->entries()[0]._k->print();
        printf(":");
        dict_obj->_map->entries()[0]._v->print();
    }

    for (int i = 1; i < size; i++) {
        printf(", ");
        dict_obj->_map->entries()[i]._k->print();
        printf(":");
        dict_obj->_map->entries()[i]._v->print();
    }

    printf("}");
}

void DictKlass::store_subscr(HiObject *x, HiObject *y, HiObject *z) {
    assert(x && x->klass() == (Klass *) this);
    ((HiDict *) x)->put(y, z);
}

void DictKlass::del_subscr(HiObject *x, HiObject *y) {
    assert(x && x->klass() == (Klass *) this);
    ((HiDict *) x)->remove(y);
}

HiObject *DictKlass::allocate_instance(HiObject *callable, ArrayList<HiObject *> *args) {
    if (!args || args->length() == 0) {
        return new HiDict();
    } else {
        return NULL;
    }
}

size_t DictKlass::size() {
    return sizeof(HiDict);
}

void DictKlass::oops_do(OopClosure *f, HiObject *obj) {
    assert(obj->klass() == (Klass *) this);

    f->do_map(&((HiDict *) obj)->_map);
}

HiDict::HiDict() {
    _map = new Map<HiObject *, HiObject *>();
    set_klass(DictKlass::get_instance());
}

HiDict::HiDict(Map<HiObject *, HiObject *> *x) {
    _map = x;
    set_klass(DictKlass::get_instance());
}

void HiDict::update(HiDict *dict) {
    for (int i = 0; i < dict->size(); i++) {
        put(dict->map()->get_key(i), dict->map()->get_value(i));
    }
}

HiObject *dict_set_default(ObjList args) {
    HiDict *dict = (HiDict *) (args->get(0));
    HiObject *key = args->get(1);
    HiObject *value = args->get(2);

    if (!dict->has_key(key)) {
        dict->put(key, value);
    }

    return Universe::HiNone;
}

HiObject *dict_remove(ObjList args) {
    HiObject *x = args->get(0);
    HiObject *y = args->get(1);

    return ((HiDict *) x)->remove(y);
}

HiObject *dict_pop(ObjList args) {
    HiObject *x = args->get(0);
    HiObject *y = args->get(1);

    ((HiDict *) x)->remove(y);

    return Universe::HiNone;
}

HiObject *dict_keys(ObjList args) {
    HiDict *x = (HiDict *) (args->get(0));
    HiList *keys = new HiList();

    for (int i = 0; i < x->size(); i++) {
        keys->append(x->map()->get_key(i));
    }

    return keys;
}

HiObject *dict_values(ObjList args) {
    HiDict *x = (HiDict *) (args->get(0));
    HiList *values = new HiList();

    for (int i = 0; i < x->size(); i++) {
        values->append(x->map()->get_value(i));
    }

    return values;
}

HiObject *dict_items(ObjList args) {
    HiDict *x = (HiDict *) (args->get(0));
    HiList *items = new HiList();

    for (int i = 0; i < x->size(); i++) {
        HiList *item = new HiList();
        item->append(x->map()->get_key(i));
        item->append(x->map()->get_value(i));
        items->append(item);
    }

    return items;
}

HiObject *dict_iterkeys(ObjList args) {
    HiDict *x = (HiDict *) (args->get(0));
    HiObject *it = new DictIterator(x);
    it->set_klass(DictIteratorKlass<ITER_KEY>::get_instance());
    return it;
}

HiObject *dict_itervalues(ObjList args) {
    HiDict *x = (HiDict *) (args->get(0));
    HiObject *it = new DictIterator(x);
    it->set_klass(DictIteratorKlass<ITER_VALUE>::get_instance());
    return it;
}

HiObject *dict_iteritems(ObjList args) {
    HiDict *x = (HiDict *) (args->get(0));
    HiObject *it = new DictIterator(x);
    it->set_klass(DictIteratorKlass<ITER_ITEM>::get_instance());
    return it;
}

template<ITER_TYPE iter_type>
HiObject *DictIteratorKlass<iter_type>::next(HiObject *x) {
    DictIterator *iter = (DictIterator *) x;

    HiDict *adict = iter->owner();
    int iter_cnt = iter->iter_cnt();
    if (iter_cnt < adict->map()->size()) {
        HiObject *obj;
        if (iter_type == ITER_KEY)
            obj = adict->map()->get_key(iter_cnt);
        else if (iter_type == ITER_VALUE) {
            obj = adict->map()->get_value(iter_cnt);
        } else if (iter_type == ITER_ITEM) {
            HiList *lobj = new HiList();
            lobj->append(adict->map()->get_key(iter_cnt));
            lobj->append(adict->map()->get_value(iter_cnt));
            obj = lobj;
        }
        iter->inc_cnt();
        return obj;
    } else {
        // TODO : we need Traceback here to mark iteration end
        return NULL;
    }
}

// TODO remove
HiObject *dictiterator_next(ObjList args) {
    DictIterator *iter = (DictIterator *) (args->get(0));
    return iter->klass()->next(iter);
}
