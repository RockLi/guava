#ifndef __GUAVA_LAZY_H__
#define __GUAVA_LAZY_H__

#include "guava.h"

guava_lazy_pyobj_t *guava_lazy_pyobj_new(PyObject **wrapped_obj, char *value, int flags);

void guava_lazy_pyobj_free(guava_lazy_pyobj_t *lazy_obj);

guava_bool_t guava_lazy_pyobj_parsed(guava_lazy_pyobj_t *lazy_obj);



#endif /* !__GUAVA_LAZY_H__ */
