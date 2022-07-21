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

#ifndef SO_COMMONS_LIBRARY_PERSON_H
#define SO_COMMONS_LIBRARY_PERSON_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/list.h>

typedef struct {
    char *name;
    unsigned char age;
} t_person;

t_person *persona_create(char *name, unsigned char age);

void persona_destroy(t_person *person);

char *get_name(t_person *person);

bool is_gaston(t_person *p);

bool is_25_years_old(t_person *p);

bool is_young(t_person *person);

bool is_adult(t_person *person);

bool ayudantes_menor(t_person *joven, t_person *menos_joven);

t_person *ayudantes_minimo_edad(t_person *person1, t_person *person2);

t_person *ayudantes_maximo_edad(t_person *person1, t_person *person2);

t_person *find_someone_by_name(t_list *list, char *name);

intptr_t add_age(intptr_t accum, t_person *person);

#endif //SO_COMMONS_LIBRARY_PERSON_H
