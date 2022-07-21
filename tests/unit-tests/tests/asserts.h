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

#ifndef UNIT_TESTS_ASSERTS_H
#define UNIT_TESTS_ASSERTS_H

#include <doctest.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
#include <commons/bitarray.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/config.h>
#include <commons/memory.h>
#include <commons/string.h>
#include "utils/person.h"
#include "utils/iterators.h"
}

#define REQUIRE_NULL(actual) do { \
    REQUIRE_EQ((intptr_t) actual, (intptr_t) NULL); \
} while(0)

#define REQUIRE_NON_NULL(actual) do { \
    REQUIRE_NE((intptr_t) actual, (intptr_t) NULL); \
} while(0)

#define REQUIRE_STRING(actual, expected) do { \
    REQUIRE_NON_NULL(actual);                 \
    std::string actual_str((char*) (actual)); \
    std::string expected_str(expected);       \
    REQUIRE_EQ(actual_str, expected_str);     \
} while(0)

#define REQUIRE_STRING_ARRAY(actual, expected, size) do { \
    REQUIRE_NON_NULL(actual);                             \
    int i;                                                \
    for (i = 0; i < size; i++) {                          \
        REQUIRE_STRING(actual[i], expected[i]);           \
    }                                                     \
    REQUIRE_NULL(actual[i]);                              \
} while(0)

#define REQUIRE_PERSON(actual, expected_name, expected_age) do {  \
    REQUIRE_NON_NULL(actual);                                     \
    REQUIRE_STRING(((t_person*) (actual))->name, expected_name);  \
    REQUIRE_EQ(((t_person*) (actual))->age, expected_age);        \
} while(0)

#define REQUIRE_PERSON_IN_LIST(actual_list, actual_index, expected_name, expected_age) do { \
    REQUIRE_PERSON(list_get(actual_list, actual_index), expected_name, expected_age);       \
} while(0)

#endif //UNIT_TESTS_ASSERTS_H
