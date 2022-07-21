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

#include "person.h"

t_person *persona_create(char *name, unsigned char age) {
    t_person *new = malloc(sizeof(t_person));
    new->name = string_duplicate(name);
    new->age = age;

    return new;
}

void persona_destroy(t_person *person) {
    free(person->name);
    free(person);
}


char *get_name(t_person *person) {
    return person->name;
}

bool is_gaston(t_person *p) {
    return string_equals_ignore_case(p->name, "Gaston");
}

bool is_25_years_old(t_person *p) {
    return p->age == 25;
}

bool is_young(t_person *person) {
    return person->age <= 24;
}

bool is_adult(t_person *person) {
    return person->age >= 18;
}

bool ayudantes_menor(t_person *joven, t_person *menos_joven) {
    return joven->age <= menos_joven->age;
}

t_person *ayudantes_minimo_edad(t_person *person1, t_person *person2) {
    return person1->age <= person2->age ? person1 : person2;
}

t_person *ayudantes_maximo_edad(t_person *person1, t_person *person2) {
    return person1->age >= person2->age ? person1 : person2;
}

t_person *find_someone_by_name(t_list *list, char *name) {
    bool _is_the_one(t_person *p) {
        return string_equals_ignore_case(p->name, name);
    }

    return list_find(list, (void *) _is_the_one);
}

intptr_t add_age(intptr_t accum, t_person *person) {
    return accum + person->age;
}
