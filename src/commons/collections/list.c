/*
 * Copyright (C) 2012 Sistemas Operativos - UTN FRBA. All rights reserved.
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

#include <stdlib.h>

#include "list.h"

static void list_link_element(t_list* self, t_link_element** indirect, t_link_element* element);
static t_link_element* list_unlink_element(t_list* self, t_link_element** indirect);
static t_link_element* list_create_element(void* data);
static t_link_element** list_get_last_element(t_list* self);
static t_link_element* list_find_element(t_list *self, bool(*cutting_condition)(t_link_element*, int));
static int list_add_element(t_list* self, t_link_element* element, bool(*cutting_condition)(t_link_element*, int));
static t_link_element* list_remove_element(t_list *self, bool(*cutting_condition)(t_link_element*, int));
static void list_append_to_sublist(t_list* sublist, t_list *self, bool(*condition)(void*, int), void* (*transformer)(void*));
static void* list_fold_elements(t_link_element* element, void* seed, void*(*operation)(void*, void*));

t_list *list_create() {
	t_list *list = malloc(sizeof(t_list));
	list->head = NULL;
	list->elements_count = 0;
	return list;
}

int list_add(t_list *self, void *data) {
	t_link_element *new_element = list_create_element(data);

	bool _add_as_last(t_link_element* element, int _) {
		return element == NULL;
	}
	return list_add_element(self, new_element, _add_as_last);
}

void list_add_all(t_list* self, t_list* other) {
	bool _add_all_elements(void* element_data, int index) {
		return true;
	}
	list_append_to_sublist(self, other, _add_all_elements, NULL);
}

void* list_get(t_list *self, int index) {
	t_link_element* element_in_index;
	bool _find_element_in_index(t_link_element* _, int i) {
		return i == index;
	}
	element_in_index = list_find_element(self, _find_element_in_index);

	return element_in_index->data;
}

void list_add_in_index(t_list *self, int index, void *data) {
	t_link_element* new_element = list_create_element(data);
	bool _add_element_at_index(t_link_element* _, int i) {
		return i == index;
	}
	list_add_element(self, new_element, _add_element_at_index);
}

void *list_replace(t_list *self, int index, void *data) {
	void *old_data = NULL;
	t_link_element* element;

	bool _find_element_at_index(t_link_element* _, int i) {
		return i == index;
	}
	element = list_find_element(self, _find_element_at_index);

	old_data = element->data;
	element->data = data;

	return old_data;
}

void list_replace_and_destroy_element(t_list *self, int index, void *data, void(*element_destroyer)(void*)) {
	void *old_data = list_replace(self, index, data);
	element_destroyer(old_data);
}

void* list_find(t_list *self, bool(*condition)(void*)) {
	t_link_element* element;
	bool _find_by_condition(t_link_element* element, int i) {
		return element == NULL || condition(element->data);
	}
	element = list_find_element(self, _find_by_condition);

	return element != NULL ? element->data : NULL;
}

void list_iterate(t_list* self, void(*closure)(void*)) {
	t_link_element *element = self->head;
	t_link_element *aux = NULL;
	while (element != NULL) {
		aux = element->next;
		closure(element->data);
		element = aux;
	}
}

void *list_remove(t_list *self, int index) {
	void *data = NULL;
	t_link_element *element = NULL;

	bool _remove_at_index(t_link_element* _, int i) {
		return i == index;
	}
	element = list_remove_element(self, _remove_at_index);

	data = element->data;
	free(element);

	return data;
}

void* list_remove_by_condition(t_list *self, bool(*condition)(void*)) {
	t_link_element *element = NULL;
	void *data = NULL;

	bool _remove_by_condition(t_link_element* element, int i) {
		return element == NULL || condition(element->data);
	}
	element = list_remove_element(self, _remove_by_condition);

	if(element != NULL) {
		data = element->data;
		free(element);
	}

	return data;
}

void list_remove_and_destroy_element(t_list *self, int index, void(*element_destroyer)(void*)) {
	void* data = list_remove(self, index);
	element_destroyer(data);
}

void list_remove_and_destroy_by_condition(t_list *self, bool(*condition)(void*), void(*element_destroyer)(void*)) {
	void* data = list_remove_by_condition(self, condition);
	if(data != NULL) {
		element_destroyer(data);
	}
}

void list_remove_and_destroy_all_by_condition(t_list *self, bool(*condition)(void*), void(*element_destroyer)(void*)) {
	t_link_element **aux = &self->head;

	while(*aux != NULL) {
		if(condition((*aux)->data)) {
			t_link_element *element = list_unlink_element(self, aux);
			element_destroyer(element->data);
			free(element);
		} else {
			aux = &(*aux)->next;
		}
	}
}

int list_size(t_list *list) {
	return list->elements_count;
}

bool list_is_empty(t_list *list) {
	return list_size(list) == 0;
}

void list_clean(t_list *self) {
	t_link_element* element;
	while (self->head != NULL) {
		element = self->head;
		self->head = self->head->next;
		free(element);
	}
	self->elements_count = 0;
}

void list_clean_and_destroy_elements(t_list *self, void(*element_destroyer)(void*)){
	list_iterate(self, element_destroyer);
	list_clean(self);
}

void list_destroy(t_list *self) {
	list_clean(self);
	free(self);
}

void list_destroy_and_destroy_elements(t_list *self, void(*element_destroyer)(void*)) {
	list_clean_and_destroy_elements(self, element_destroyer);
	free(self);
}

t_list* list_take(t_list* self, int count) {
	return list_slice(self, 0, count);
}

t_list* list_slice(t_list* self, int start, int count) {
	t_list* sublist = list_create();

	bool _take_count_elements(void* element_data, int index) {
		return start <= index && index < (start + count);
	}
	list_append_to_sublist(sublist, self, _take_count_elements, NULL);

	return sublist;
}

t_list* list_slice_and_remove(t_list* self, int start, int count) {
	t_link_element** aux = &self->head;
	for(int i = 0; i < start; i++) {
		aux = &(*aux)->next;
	}
	t_list* sublist = list_create();
	t_link_element** sublist_last_element = &sublist->head;
	for(int j = 0; j < count && (*aux) != NULL; j++) {
		list_link_element(sublist, sublist_last_element, list_unlink_element(self, aux));
		sublist_last_element = &(*sublist_last_element)->next;
	}
	return sublist;
}

t_list* list_take_and_remove(t_list* self, int count) {
	return list_slice_and_remove(self, 0, count);
}

t_list* list_filter(t_list* self, bool(*condition)(void*)){
	t_list* filtered = list_create();

	bool _add_if_condition(void* element_data, int index) {
		return condition(element_data);
	}
	list_append_to_sublist(filtered, self, _add_if_condition, NULL);

	return filtered;
}

t_list* list_map(t_list* self, void*(*transformer)(void*)){
	t_list* mapped = list_create();

	bool _transform_all(void* element_data, int index) {
			return true;
	}
	list_append_to_sublist(mapped, self, _transform_all, transformer);

	return mapped;
}

int list_add_sorted(t_list *self, void* data, bool (*comparator)(void*,void*)) {
	t_link_element* new_element = list_create_element(data);

	bool _insert_element_sorted(t_link_element* element, int i) {
		return element == NULL || !comparator(element->data, data);
	}
	return list_add_element(self, new_element, _insert_element_sorted);
}

void list_sort(t_list *self, bool (*comparator)(void *, void *)) {
	if(self->elements_count > 1) {
		t_list* sorted = list_create();
		t_link_element* removed = NULL;

		while(self->elements_count > 0) {
			bool _remove_first_element(t_link_element* element, int _) {
				return element != NULL;
			}
			removed = list_remove_element(self,_remove_first_element);

			bool _insert_element_sorted(t_link_element* element, int _) {
				return element == NULL || !comparator(element->data, removed->data);
			}
			list_add_element(sorted, removed, _insert_element_sorted);
		}

		self->head = sorted->head;
		self->elements_count = sorted->elements_count;
		free(sorted);
	}
}

t_list* list_sorted(t_list* self, bool (*comparator)(void *, void *)) {
	t_list* duplicated = list_duplicate(self);
	list_sort(duplicated, comparator);
	return duplicated;
}

int list_count_satisfying(t_list* self, bool(*condition)(void*)){
	int result = 0;
	void _count_satisfying(void* element_data) {
		if(condition(element_data)) {
			result++;
		}
	}
	list_iterate(self, _count_satisfying);
	return result;
}

bool list_any_satisfy(t_list* self, bool(*condition)(void*)){
	return list_count_satisfying(self, condition) > 0;
}

bool list_all_satisfy(t_list* self, bool(*condition)(void*)){
	return list_count_satisfying(self, condition) == self->elements_count;
}

t_list* list_duplicate(t_list* self) {
	t_list* duplicated = list_create();
	list_add_all(duplicated, self);
	return duplicated;
}

void* list_fold1(t_list* self, void* (*operation)(void*, void*)) {
	return list_fold_elements(self->head->next, self->head->data, operation);
}

void* list_fold(t_list* self, void* seed, void*(*operation)(void*, void*)) {
	return list_fold_elements(self->head, seed, operation);
}

void* list_get_minimum(t_list* self, void* (*minimum)(void*, void*)) {
	return list_fold1(self, minimum);
}

void* list_get_maximum(t_list* self, void* (*maximum)(void*, void*)) {
	return list_fold1(self, maximum);
}

t_list_iterator* list_iterator_create(t_list* list) {
	t_list_iterator* new = malloc(sizeof(t_list_iterator));
	new->list = list;
	new->actual = NULL;
	new->next = &list->head;
	new->index = -1;

	return new;
}

bool list_iterator_has_next(t_list_iterator* iterator) {
	return (*iterator->next) != NULL;
}

void* list_iterator_next(t_list_iterator* iterator) {
	iterator->actual = iterator->next;
	iterator->next = &(*iterator->next)->next;
	iterator->index++;
	return (*iterator->actual)->data;
}

void list_iterator_remove(t_list_iterator* iterator) {
	free(list_unlink_element(iterator->list, iterator->actual));
	iterator->next = iterator->actual;
	iterator->index--;
}

void list_iterator_destroy(t_list_iterator* iterator) {
	free(iterator);
}

/********* PRIVATE FUNCTIONS **************/

static void list_link_element(t_list* self, t_link_element** indirect, t_link_element* element) {
	element->next = *indirect;
	*indirect = element;
	self->elements_count++;
}

static t_link_element* list_unlink_element(t_list* self, t_link_element** indirect) {
	t_link_element* element = *indirect;
	*indirect = element->next;
	self->elements_count--;
	return element;
}

static t_link_element* list_create_element(void* data) {
	t_link_element* element = malloc(sizeof(t_link_element));
	element->data = data;
	element->next = NULL;
	return element;
}

static t_link_element** list_get_last_element(t_list* self) {
	t_link_element** last = &self->head;
	while ((*last) != NULL) {
		last = &(*last)->next;
	}

	return last;
}

static t_link_element* list_find_element(t_list *self, bool(*cutting_condition)(t_link_element*, int)) {
	t_link_element* element = self->head;
	int index = 0;

	while(!cutting_condition(element, index)) {
		element = element->next;
		index++;
	}

	return element;
}

static int list_add_element(t_list* self, t_link_element* element, bool(*cutting_condition)(t_link_element*, int)) {
	t_link_element **aux = &self->head;
	int index = 0;

	while(!cutting_condition(*aux, index)) {
		aux = &(*aux)->next;
		index++;
	}
	list_link_element(self, aux, element);

	return index;
}

static t_link_element* list_remove_element(t_list *self, bool(*cutting_condition)(t_link_element*, int)) {
	t_link_element **aux = &self->head;
	int index = 0;

	while(!cutting_condition(*aux, index)) {
		aux = &(*aux)->next;
		index++;
	}

	return list_unlink_element(self, aux);
}

static void list_append_to_sublist(t_list* sublist, t_list *self, bool(*condition)(void*, int), void* (*transformer)(void*)) {
	t_link_element **last_element = list_get_last_element(sublist);
	t_link_element **aux = &self->head;
	int index = 0;
	while((*aux) != NULL) {
		if(condition((*aux)->data, index)) {
			t_link_element* new_element = list_create_element(transformer ? transformer((*aux)->data) : (*aux)->data);
			list_link_element(sublist, last_element, new_element);
			last_element = &(*last_element)->next;
		}
		index++;
		aux = &(*aux)->next;
	}
}

static void* list_fold_elements(t_link_element* element, void* seed, void*(*operation)(void*, void*)) {
	void* result = seed;
	while(element != NULL) {
		result = operation(result, element->data);
		element = element->next;
	}

	return result;
}
