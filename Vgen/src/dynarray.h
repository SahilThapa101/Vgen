//
//  dynarray.h
//  Vgen
//
//  Created by Amar Thapa on 23/12/18.
//  Copyright © 2018 Amar Thapa. All rights reserved.
//

#ifndef dynarray_h
#define dynarray_h

struct dynarray {
    void ** buffer;
    unsigned int size;
    unsigned int count;
};

typedef struct dynarray dynarray;

typedef void (*dynarray_forfn)(void *);

dynarray * dynarray_create(unsigned int startsize);
void dynarray_empty(dynarray * array);
void dynarray_delete(dynarray * array);
void dynarray_add_tail(dynarray * array, void * data);
void dynarray_add_head(dynarray * array, void * data);
void * dynarray_remove_tail(dynarray * array);
void * dynarray_remove_head(dynarray * array);
void dynarray_insert(dynarray *array, unsigned int pos, void *data);
void * dynarray_remove(dynarray *array, unsigned int pos);
void * dynarray_get(const dynarray * array, unsigned int pos);
void * dynarray_set(dynarray * array, unsigned int pos, void * data);
void dynarray_for_each(const dynarray * array, dynarray_forfn fun);
unsigned int dynarray_get_count(const dynarray * array);
void dynarray_set_size(dynarray * array, unsigned int size);

#endif /* dynarray_h */