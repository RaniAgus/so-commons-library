/*
 * Copyright (C) 2019 Sistemas Operativos - UTN FRBA. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "vector.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

inline static void* _calculate_offset(void* array, size_t pos, size_t element_size)
{
    return ((char*) array) + pos * element_size;
}

inline static size_t _calculate_growth(t_vector* v, size_t new_size)
{
    size_t geometric = v->allocated_size + v->allocated_size / 2;
    if (geometric < new_size)
        return new_size;
    return geometric;
}

void vector_create(t_vector* v, size_t element_size, void (*element_destroyer)(void*), size_t initial_allocated_size)
{
    v->elements = NULL;
    v->element_size = element_size;
    v->size = 0;
    v->allocated_size = initial_allocated_size;
    if (initial_allocated_size)
        v->elements = malloc(initial_allocated_size * element_size);
    v->element_destroyer = element_destroyer;
}

size_t vector_size(t_vector* v)
{
    return v->size;
}

void vector_resize_fill(t_vector* v, size_t n, void* elem)
{
    size_t oldsize = v->size;

    if (n < oldsize)
        vector_remove_range(v, n, v->size, NULL);
    else if (n > oldsize)
    {
        // vector_reallocate(v, n);

        // fill rest with elem
        vector_add_fill(v, oldsize, n - oldsize, elem);
    }
}

void vector_resize_fill_zero(t_vector* v, size_t n)
{
    size_t oldsize = v->size;
    if (n < oldsize)
        vector_remove_range(v, n, v->size, NULL);
    else
    {
        vector_reallocate(v, n);

        memset(_calculate_offset(v->elements, oldsize, v->element_size), 0, (n - oldsize) * v->element_size);
        v->size = n;
    }
}

size_t vector_allocated_size(t_vector* v)
{
    return v->allocated_size;
}

bool vector_is_empty(t_vector* v)
{
    return !v->size;
}

void vector_reallocate(t_vector* v, size_t n)
{
    if (n <= v->allocated_size)
        return;

    v->elements = realloc(v->elements, n * v->element_size);
    v->allocated_size = n;
}

void vector_reallocate_to_size(t_vector* v)
{
    if (v->allocated_size == v->size)
        return;

    if (!v->size)
    {
        free(v->elements);
        v->elements = NULL;
        v->allocated_size = 0;
    }
    else
    {
        v->elements = realloc(v->elements, v->size * v->element_size);
        v->allocated_size = v->size;
    }
}

void* vector_get(t_vector* v, size_t i)
{
    assert(v->elements && i < v->size);
    return _calculate_offset(v->elements, i, v->element_size);
}

void* vector_first(t_vector* v)
{
    assert(v->elements);
    return _calculate_offset(v->elements, 0, v->element_size);
}

void* vector_last(t_vector* v)
{
    assert(v->elements && v->size);
    return vector_get(v, v->size - 1);
}

void* vector_data(t_vector* v)
{
    return v->elements;
}

void vector_push_last(t_vector* v, void* elem)
{
    if (v->allocated_size == v->size)
        vector_reallocate(v, _calculate_growth(v, v->size + 1));

    memcpy(_calculate_offset(v->elements, v->size, v->element_size), elem, v->element_size);
    ++v->size;
}

void vector_pop_last(t_vector* v, void* dest)
{
    assert(v->size);
    vector_remove(v, v->size - 1, dest);
}

void vector_add(t_vector* v, size_t pos, void* elem)
{
    vector_add_fill(v, pos, 1, elem);
}

void vector_add_fill(t_vector* v, size_t pos, size_t n, void* elem)
{
    assert(pos <= v->size);
    if (n == 1 && pos == v->size) // special case
    {
        vector_push_last(v, elem);
        return;
    }

    if (n > (v->allocated_size - v->size))
        vector_reallocate(v, _calculate_growth(v, v->size + n));

    void* start = _calculate_offset(v->elements, pos, v->element_size);
    void* end = _calculate_offset(v->elements, pos + n, v->element_size);
    size_t blocksize = (v->size - pos) * v->element_size;

    memmove(end, start, blocksize);
    for (size_t i = 0; i < n; ++i)
        memcpy(_calculate_offset(start, i, v->element_size), elem, v->element_size);
    v->size += n;
}

void vector_add_from_array(t_vector* v, size_t pos, void* begin, void* end)
{
    assert(pos <= v->size);
    if (begin == end)
        return;

    size_t n = ((char*)end - (char*)begin) / v->element_size;
    if (n == 1 && pos == v->size) // special case
    {
        vector_push_last(v, begin);
        return;
    }

    if (n > (v->allocated_size - v->size))
        vector_reallocate(v, _calculate_growth(v, v->size + n));

    void* shiftSrc = _calculate_offset(v->elements, pos, v->element_size);
    void* shiftDst = _calculate_offset(v->elements, pos + n, v->element_size);
    size_t blocksize = (v->size - pos) * v->element_size;

    if (blocksize)
        memmove(shiftDst, shiftSrc, blocksize);

    memcpy(shiftSrc, begin, n * v->element_size);
    v->size += n;
}

void vector_remove(t_vector* v, size_t pos, void* dest)
{
    assert(pos < v->size);
    vector_remove_range(v, pos, pos + 1, dest);
}

void vector_remove_range(t_vector* v, size_t begin, size_t end, void* dest)
{
    assert(begin < v->size);
    assert(end <= v->size);

    size_t n = end - begin;
    if (dest)
        memcpy(dest, _calculate_offset(v->elements, begin, v->element_size), n * v->element_size);
    else if (v->element_destroyer)
    {
        for (size_t i = begin; i < end; ++i)
            v->element_destroyer(_calculate_offset(v->elements, i, v->element_size));
    }

    // shift needed
    if (end < v->size)
    {
        void* shiftSrc = _calculate_offset(v->elements, end, v->element_size);
        void* shiftDst = _calculate_offset(v->elements, begin, v->element_size);
        size_t blocksize = (v->size - end) * v->element_size;
        memmove(shiftDst, shiftSrc, blocksize);
    }

    v->size -= n;
}

void vector_swap_data(t_vector* v, t_vector* other)
{
    //POD should copy
    t_vector me = *v;
    *v = *other;
    *other = me;
}

void vector_clean(t_vector* v)
{
    if (v->element_destroyer)
        vector_iterate(v, v->element_destroyer);
    v->size = 0;
}

void vector_iterate(t_vector* v, void (*closureFn)(void*))
{
    for (size_t i = 0; i < v->size; ++i)
        closureFn(_calculate_offset(v->elements, i, v->element_size));
}

void vector_destroy(t_vector* v)
{
    if (v->element_destroyer)
        vector_iterate(v, v->element_destroyer);
    free(v->elements);
}
