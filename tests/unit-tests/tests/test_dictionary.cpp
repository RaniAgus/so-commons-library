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

#include "asserts.h"

void _assert_key(char *got, const char *expected) {
    REQUIRE_NON_NULL(got);
    REQUIRE_STRING(got, expected);
}

TEST_SUITE("Dictionary") {
    TEST_CASE("Put and get") {
        t_dictionary *dictionary = dictionary_create();

        SUBCASE("should put a value at a key") {
            REQUIRE(dictionary_is_empty(dictionary));
            dictionary_put(dictionary, "Matias", persona_create("Matias", 24));
            REQUIRE_EQ(dictionary_size(dictionary), 1);
            dictionary_put(dictionary, "Gaston", persona_create("Gaston", 25));
            REQUIRE_EQ(dictionary_size(dictionary), 2);
        }

        SUBCASE("should get a value from a key") {
            dictionary_put(dictionary, "Matias", persona_create("Matias", 24));
            dictionary_put(dictionary, "Gaston", persona_create("Gaston", 25));
            REQUIRE_EQ(dictionary_size(dictionary), 2);
            REQUIRE_PERSON(dictionary_get(dictionary, "Matias"), "Matias", 24);
            REQUIRE_PERSON(dictionary_get(dictionary, "Gaston"), "Gaston", 25);
            REQUIRE_EQ(dictionary_size(dictionary), 2);
        }

        SUBCASE("should update a value from a key") {
            dictionary_put(dictionary, "Gaston", persona_create("Gaston", 23));
            dictionary_put(dictionary, "Fernando", persona_create("Fernando", 24));
            dictionary_put(dictionary, "Matias", persona_create("Matias", 26));

            t_person *old_fernando = (t_person *) dictionary_get(dictionary, "Fernando");
            dictionary_put(dictionary, "Fernando", persona_create("Velcic", 25));
            REQUIRE_EQ(dictionary_size(dictionary), 3);
            REQUIRE_PERSON(dictionary_get(dictionary, "Fernando"), "Velcic", 25);

            persona_destroy(old_fernando);
        }

        dictionary_destroy_and_destroy_elements(dictionary, (void (*)(void *)) persona_destroy);
    }

    TEST_CASE("Remove, destroy and clean") {
        t_dictionary *dictionary = dictionary_create();

        dictionary_put(dictionary, "Matias", persona_create("Matias", 24));
        dictionary_put(dictionary, "Gaston", persona_create("Gaston", 25));

        SUBCASE("should remove a value from a key") {
            REQUIRE_EQ(dictionary_size(dictionary), 2);

            t_person *aux = (t_person *) dictionary_remove(dictionary, "Matias");
            REQUIRE_PERSON(aux, "Matias", 24);
            persona_destroy(aux);

            REQUIRE_EQ(dictionary_size(dictionary), 1);

            aux = (t_person *) dictionary_remove(dictionary, "Gaston");
            REQUIRE_PERSON(aux, "Gaston", 25);
            persona_destroy(aux);

            REQUIRE(dictionary_is_empty(dictionary));
        }

        SUBCASE("should remove and destroy a value from a key") {
            REQUIRE_EQ(dictionary_size(dictionary), 2);

            dictionary_remove_and_destroy(dictionary, "Matias", (void (*)(void *)) persona_destroy);
            REQUIRE_EQ(dictionary_size(dictionary), 1);

            dictionary_remove_and_destroy(dictionary, "Gaston", (void (*)(void *)) persona_destroy);
            REQUIRE(dictionary_is_empty(dictionary));

            REQUIRE_NULL(dictionary_get(dictionary, "Matias"));
            REQUIRE_NULL(dictionary_get(dictionary, "Gaston"));
        }

        SUBCASE("should clean the dictionary") {
            REQUIRE_EQ(dictionary_size(dictionary), 2);
            dictionary_clean_and_destroy_elements(dictionary, (void (*)(void *)) persona_destroy);
            REQUIRE(dictionary_is_empty(dictionary));
        }

        dictionary_destroy_and_destroy_elements(dictionary, (void (*)(void *)) persona_destroy);
    }

    TEST_CASE("Iterate") {
        t_dictionary *dictionary = dictionary_create();
        const char *names[] = {"Gaston", "Marco", "Daniela", "Matias"};
        int iterator_count;

        SUBCASE("should iterate all entries") {
            dictionary_put(dictionary, "Matias", persona_create("Matias", 24));
            dictionary_put(dictionary, "Gaston", persona_create("Gaston", 25));
            dictionary_put(dictionary, "Daniela", persona_create("Daniela", 20));
            dictionary_put(dictionary, "Marco", persona_create("Marco", 21));

            iterator_count = iterate_a_dictionary(dictionary, names, _assert_key);

            REQUIRE_EQ(iterator_count, 4);
        }

        dictionary_destroy_and_destroy_elements(dictionary, (void (*)(void *)) persona_destroy);
    }
}
