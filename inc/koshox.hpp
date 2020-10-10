//
// Created by Kosho on 2020/10/8.
//

#ifndef PYTHONVM_KOSHOX_HPP
#define PYTHONVM_KOSHOX_HPP

#include "runtime/functionObject.hpp"
#include "object/hiInteger.hpp"

#define SO_PUBLIC __attribute__((visibility("default")))

struct RGMethod {
    const char *method_name;
    NativeFuncPointer method;
    int method_info;
    const char *method_doc;
};

typedef RGMethod* (*INIT_FUNC)();

#endif //PYTHONVM_KOSHOX_HPP
