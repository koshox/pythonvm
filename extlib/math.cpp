#include "inc/koshox.hpp"

#include <math.h>

HiObject *add(ObjList args) {
    HiInteger *a = (HiInteger *) args->get(0);
    HiInteger *b = (HiInteger *) args->get(1);

    return new HiInteger(a->value() + b->value());
}

RGMethod math_methods[] = {
        {"add", add, 0, "add two integer",},
        {NULL, NULL, 0, NULL,},
};

#ifdef __cplusplus
extern "C" {
#endif

SO_PUBLIC RGMethod *init_libmath() {
    return math_methods;
}

#ifdef __cplusplus
}
#endif
