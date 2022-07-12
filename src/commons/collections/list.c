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

static t_link_element *list_create_element(void* data);
static void list_link_element(t_list* self, t_link_element** indirect, t_link_element* element);
static t_link_element *list_unlink_element(t_list* self, t_link_element** indirect);
static t_link_element **list_get_indirect_in_tail(t_list *self);
static t_link_element **list_get_indirect_in_index(t_list *self, int index);
static t_link_element **list_get_indirect_by_condition(t_list *self, bool(*condition)(void*));
static void *list_replace_indirect(t_link_element **indirect, void *data);
static void *list_remove_indirect(t_list *self, t_link_element **indirect);
static void list_iterate_indirects(t_list* self, int start, int count, t_link_element **(*next)(t_link_element**));
static t_list *list_merge_sorted(t_list *left, t_list *right, bool (*comparator)(void*, void*));
static void* list_fold_elements(t_link_element* element, void* seed, void*(*operation)(void*, void*));

t_list *list_create() {
	t_list *list = malloc(sizeof(t_list));
	list->head = NULL;
	list->elements_count = 0;
	return list;
}

int list_add(t_list *self, void *data) {
	t_link_element **indirect = list_get_indirect_in_tail(self);
	list_link_element(self, indirect, list_create_element(data));
	return list_size(self) - 1;
}

void list_add_all(t_list* self, t_list* other) {
	t_link_element **indirect = list_get_indirect_in_tail(self);
	void _add_element(void *data) {
		list_link_element(self, indirect, list_create_element(data));
		indirect = &(*indirect)->next;
	}
	list_iterate(other, _add_element);
}

void* list_get(t_list *self, int index) {
	t_link_element **indirect = list_get_indirect_in_index(self, index);
	return (*indirect)->data;
}

void list_add_in_index(t_list *self, int index, void *data) {
	t_link_element **indirect = list_get_indirect_in_index(self, index);
	list_link_element(self, indirect, list_create_element(data));
}

void *list_replace(t_list *self, int index, void *data) {
	t_link_element **indirect = list_get_indirect_in_index(self, index);
	return list_replace_indirect(indirect, data);
}

void list_replace_and_destroy_element(t_list *self, int index, void *data, void(*element_destroyer)(void*)) {
	void *old_data = list_replace(self, index, data);
	element_destroyer(old_data);
}

void* list_find(t_list *self, bool(*condition)(void*)) {
	t_link_element **indirect = list_get_indirect_by_condition(self, condition);
	return (*indirect) != NULL ? (*indirect)->data : NULL;
}

void list_iterate(t_list* self, void(*closure)(void*)) {
	t_link_element **_get_next(t_link_element** indirect) {
		closure((*indirect)->data);
		return &(*indirect)->next;
	}
	list_iterate_indirects(self, 0, list_size(self), _get_next);
}

void *list_remove(t_list *self, int index) {
	t_link_element **indirect = list_get_indirect_in_index(self, index);
	return list_remove_indirect(self, indirect);
}

void* list_remove_by_condition(t_list *self, bool(*condition)(void*)) {
	t_link_element **indirect = list_get_indirect_by_condition(self, condition);
	return (*indirect) != NULL ? list_remove_indirect(self, indirect) : NULL;
}

void list_remove_and_destroy_element(t_list *self, int index, void(*element_destroyer)(void*)) {
	void* data = list_remove(self, index);
	element_destroyer(data);
}

void list_remove_and_destroy_by_condition(t_list *self, bool(*condition)(void*), void(*element_destroyer)(void*)) {
	void* data = list_remove_by_condition(self, condition);
	if (data != NULL) {
		element_destroyer(data);
	}
}

void list_remove_and_destroy_all_by_condition(t_list *self, bool(*condition)(void*), void(*element_destroyer)(void*)) {
	t_link_element **indirect = &self->head;
	while ((*indirect) != NULL) {
		if (condition((*indirect)->data)) {
			void* data = list_remove_indirect(self, indirect);
			element_destroyer(data);
		} else {
			indirect = &(*indirect)->next;
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
	t_link_element **indirect = &self->head;
	while ((*indirect) != NULL) {
		list_remove_indirect(self, indirect);
	}
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
	t_link_element **sublist_indirect = &sublist->head;

	t_link_element **_slice_data(t_link_element **indirect) {
		t_link_element *element = list_create_element((*indirect)->data);
		list_link_element(sublist, sublist_indirect, element);
		sublist_indirect = &(*sublist_indirect)->next;
		return &(*indirect)->next;
	}
	list_iterate_indirects(self, start, count, _slice_data);

	return sublist;
}

t_list* list_slice_and_remove(t_list* self, int start, int count) {
	t_list* sublist = list_create();
	t_link_element **sublist_indirect = &sublist->head;

	t_link_element **_slice_and_remove_data(t_link_element **indirect) {
		t_link_element *element = list_unlink_element(self, indirect);
		list_link_element(sublist, sublist_indirect, element);
		sublist_indirect = &(*sublist_indirect)->next;
		return indirect;
	}
	list_iterate_indirects(self, start, count, _slice_and_remove_data);

	return sublist;
}

t_list* list_take_and_remove(t_list* self, int count) {
	return list_slice_and_remove(self, 0, count);
}

t_list* list_filter(t_list* self, bool(*condition)(void*)){
	t_list *sublist = list_create();
	t_link_element **indirect = &sublist->head;

	void _filter_data(void* data) {
		if (condition(data)) {
			list_link_element(sublist, indirect, list_create_element(data));
			indirect = &(*indirect)->next;
		}
	}
	list_iterate(self, _filter_data);

	return sublist;
}

t_list* list_map(t_list* self, void*(*transformer)(void*)){
	t_list *sublist = list_create();
	t_link_element **indirect = &sublist->head;

	void _map_data(void* data) {
		t_link_element *element = list_create_element(transformer(data));
		list_link_element(sublist, indirect, element);
		indirect = &(*indirect)->next;
	}
	list_iterate(self, _map_data);

	return sublist;
}

int list_add_sorted(t_list *self, void* data, bool (*comparator)(void*,void*)) {
	t_link_element **indirect = &self->head;
	int index = 0;
	while ((*indirect) != NULL && comparator((*indirect)->data, data)) {
		indirect = &(*indirect)->next;
		index++;
	}
	list_link_element(self, indirect, list_create_element(data));

	return index;
}

void list_sort(t_list *self, bool (*comparator)(void *, void *)) {
	if (list_size(self) <= 1) {
		return;
	}
	t_list *left = list_take_and_remove(self, list_size(self) / 2);
	t_list *right = list_take_and_remove(self, list_size(self));

	list_sort(left, comparator);
	list_sort(right, comparator);

	t_list *result = list_merge_sorted(left, right, comparator);
	*self = *result;
	free(result);
}

t_list* list_sorted(t_list* self, bool (*comparator)(void *, void *)) {
	t_list *other = list_duplicate(self);
	list_sort(other, comparator);
	return other;
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
	list_remove_indirect(iterator->list, iterator->actual);
	iterator->next = iterator->actual;
	iterator->index--;
}

void list_iterator_destroy(t_list_iterator* iterator) {
	free(iterator);
}

/********* PRIVATE FUNCTIONS **************/

static t_link_element* list_create_element(void* data) {
	t_link_element* element = malloc(sizeof(t_link_element));
	element->data = data;
	element->next = NULL;
	return element;
}

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

static t_link_element **list_get_indirect_in_tail(t_list *self) {
	t_link_element **indirect = &self->head;
	while ((*indirect) != NULL) {
		indirect = &(*indirect)->next;
	}
	return indirect;
}

static t_link_element **list_get_indirect_in_index(t_list *self, int index) {
	t_link_element **indirect = &self->head;
	for (int i = 0; i < index; ++i) {
		indirect = &(*indirect)->next;
	}
	return indirect;
}

static t_link_element **list_get_indirect_by_condition(t_list *self, bool(*condition)(void*)) {
	t_link_element** indirect = &self->head;
	while ((*indirect) != NULL && !condition((*indirect)->data)) {
		indirect = &(*indirect)->next;
	}
	return indirect;
}

static void *list_replace_indirect(t_link_element **indirect, void *data) {
	void *old_data = (*indirect)->data;
	(*indirect)->data = data;
	return old_data;
}

static void *list_remove_indirect(t_list *self, t_link_element **indirect) {
	t_link_element *element = list_unlink_element(self, indirect);
	void *data = element->data;
	free(element);
	return data;
}

static t_list *list_merge_sorted(t_list *left, t_list *right, bool (*comparator)(void*, void*)) {
	if (list_is_empty(left)) {
		list_destroy(left);
		return right;
	}
	if (list_is_empty(right)) {
		list_destroy(right);
		return left;
	}
	t_link_element *element = comparator(left->head->data, right->head->data)
							  ? list_unlink_element(left, &left->head)
							  : list_unlink_element(right, &right->head);
	t_list *sorted = list_merge_sorted(left, right, comparator);
	list_link_element(sorted, &sorted->head, element);

	return sorted;
}

static void list_iterate_indirects(t_list *self, int start, int count, t_link_element **(*next)(t_link_element**)) {
	t_link_element **indirect = list_get_indirect_in_index(self, start);
	int i = 0;
	while ((i < count) && (*indirect) != NULL) {
		indirect = next(indirect);
		++i;
	}
}

static void* list_fold_elements(t_link_element* element, void* seed, void*(*operation)(void*, void*)) {
	void* result = seed;
	while (element != NULL) {
		result = operation(result, element->data);
		element = element->next;
	}

	return result;
}