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

#ifndef vector_h__
#define vector_h__

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    void* elements;
    size_t element_size;
    size_t size;
    size_t allocated_size;
    void (*element_destroyer)(void*);
} t_vector;

/**
 * @NAME: vector_create
 * @DESC: Construye un nuevo vector in-place
 * element_size: tamaño de los elementos que guardara el vector normalmente sizeof(tipo-elemento)
 * element_destroyer: funcion llamada para destruir los elementos en sí, si es distinta a NULL 
 * initial_allocated_size: reserva inicial de memoria para guardar 'initialallocated_size' elementos (puede ser 0)
 */
void vector_create(t_vector* v, size_t element_size, void (*element_destroyer)(void*), size_t initial_allocated_size);

/**
 * @NAME: vector_destroy
 * @DESC: Libera la memoria reservada por las estructuras
 * destruyendo los elementos con la funcion pasada como parametro al construir, si no es NULL
 */
void vector_destroy(t_vector* v);

// C++ like interface

/**
 * @NAME: vector_size
 * @DESC: Devuelve tamaño del vector
 */
size_t vector_size(t_vector* v);

/**
 * @NAME: vector_resize_fill
 * @DESC: Cambia el tamaño del vector eliminando o insertando elementos
 * Los elementos insertados serán copias del elemento pasado por parámetro
 */
void vector_resize_fill(t_vector* v, size_t n, void* elem);

/**
 * @NAME: vector_resize_fill_zero
 * @DESC: Cambia el tamaño del vector eliminando o insertando elementos
 * Los elementos insertados serán inicializados en 0
 */
void vector_resize_fill_zero(t_vector* v, size_t n);

/**
 * @NAME: vector_allocated_size
 * @DESC: Devuelve capacidad actual de elementos
 */
size_t vector_allocated_size(t_vector* v);

/**
 * @NAME: vector_is_empty
 * @DESC: Devuelve true si el vector está vacio
 */
bool vector_is_empty(t_vector* v);

/**
 * @NAME: vector_reallocate
 * @DESC: Se asegura que la capacidad alocada sea de al menos n elementos
 * No cambia ningún elemento actual
 */
void vector_reallocate(t_vector* v, size_t n);

/**
 * @NAME: vector_reallocate_to_size
 * @DESC: Encoge la capacidad del vector de forma tal que sólo quepan
 * sus elementos actuales. A diferencia del estándar, siempre efectúa la operación
 * en caso de ser necesaria.
 */
void vector_reallocate_to_size(t_vector* v);

/**
 * @NAME: vector_get
 * @DESC: Devuelve el elemento en la posición indicada.
 */
void* vector_get(t_vector* v, size_t pos);

/**
 * @NAME: vector_first
 * @DESC: Devuelve el primer elemento
 */
void* vector_first(t_vector* v);

/**
 * @NAME: vector_last
 * @DESC: Devuelve el último elemento
 */
void* vector_last(t_vector* v);

/**
 * @NAME: vector_data
 * @DESC: Devuelve el arreglo de datos
 */
void* vector_data(t_vector* v);

/**
 * @NAME: vector_push_last
 * @DESC: Inserta un elemento al final
 */
void vector_push_last(t_vector* v, void* elem);

/**
 * @NAME: vector_pop_last
 * @DESC: Quita el último elemento, dejando una copia en dest.
 * Si dest es NULL, lo destruye llamando al element_destroyer.
 */
void vector_pop_last(t_vector* v, void* dest);

/**
 * @NAME; vector_add
 * @DESC: Inserta un único valor en la posición 'pos'
 */
void vector_add(t_vector* v, size_t pos, void* elem);

/**
 * @NAME: vector_add_fill
 * @DESC: Inserta n copias del elemento elem a partir de la posición 'pos'
 */
void vector_add_fill(t_vector* v, size_t pos, size_t n, void* elem);

/**
 * @NAME: vector_add_from_array
 * @DESC: Inserta valores desde un arreglo, entre la posicion begin, hasta la end exclusive
 * En otras palabras, rango cerrado abierto [begin, end)
 */
void vector_add_from_array(t_vector* v, size_t pos, void* begin, void* end);

/**
 * @NAME: vector_remove
 * @DESC: Quita el elemento ubicado en 'pos', dejando una copia en dest.
 * Si dest es NULL, lo destruye llamando al element_destroyer.
 */
void vector_remove(t_vector* v, size_t pos, void* dest);

/**
 * @NAME: vector_remove_range
 * @DESC: Quita los elementos en el rango de posiciones cerrado abierto [begin, end)
 * dejando una copia en dest. Si dest es NULL, lo destruye llamando al element_destroyer.
 */
void vector_remove_range(t_vector* v, size_t begin, size_t end, void* dest);

/**
 * @NAME: vector_swap_data
 * @DESC: Intercambia contenidos con otro vector
 */
void vector_swap_data(t_vector* v, t_vector* other);

/**
 * @NAME: vector_clean
 * @DESC: Limpia los elementos
 */
void vector_clean(t_vector* v);

// Non-C++ interface
/**
 * @NAME: vector_iterate
 * @DESC: Itera los elementos con una función
 */
void vector_iterate(t_vector* v, void (*closure)(void*));

#endif //vector_h__
