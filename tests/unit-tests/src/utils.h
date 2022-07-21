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

#ifndef UNIT_TESTS_UTILS_H
#define UNIT_TESTS_UTILS_H

#include <doctest.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

extern "C" {
#include <commons/bitarray.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/config.h>
#include <commons/memory.h>
#include <commons/string.h>
#include "person.h"
#include "iterators.h"
}

#define REQUIRE_NULL(got) do { \
    REQUIRE((got) == NULL);    \
} while(0)

#define REQUIRE_NON_NULL(got) do { \
    REQUIRE((got) != NULL);        \
} while(0)

#define REQUIRE_STRING(got, expected) do {         \
    REQUIRE(strcmp((char*) (got), expected) == 0); \
} while(0)

#define REQUIRE_PERSON(got, expected_name, expected_age) do {  \
    REQUIRE_NON_NULL(got);                                     \
    REQUIRE_STRING(((t_person*) (got))->name, expected_name);  \
    REQUIRE(((t_person*) (got))->age == (expected_age));       \
} while(0)

#define REQUIRE_PERSON_IN_LIST(a_list, an_index, expected_name, expected_age) do { \
    REQUIRE_PERSON(list_get(a_list, an_index), expected_name, expected_age);       \
} while(0)

#endif //UNIT_TESTS_UTILS_H
