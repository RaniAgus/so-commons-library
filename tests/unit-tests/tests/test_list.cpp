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

#include "../src/utils.h"

void _assert_person_has_name(t_person *p, const char *expected) {
    REQUIRE_NON_NULL(p);
    REQUIRE_STRING(p->name, expected);
}

t_list *_sort_actual_list(t_list *list) {
    list_sort(list, (bool (*)(void *, void *)) ayudantes_menor);
    return list;
}

t_list *_generate_sorted_list(t_list *list) {
    return list_sorted(list, (bool (*)(void *, void *)) ayudantes_menor);
}

t_list *_verify_a_sort_without_duplicates(t_list *(*sorted_list_generator)(t_list *list), t_list *list) {
    t_list *a_sorted_list = sorted_list_generator(list);

    REQUIRE(list_size(a_sorted_list) == 4);
    REQUIRE_PERSON_IN_LIST(a_sorted_list, 0, "Daniela", 19);
    REQUIRE_PERSON_IN_LIST(a_sorted_list, 1, "Sebastian", 21);
    REQUIRE_PERSON_IN_LIST(a_sorted_list, 2, "Matias", 24);
    REQUIRE_PERSON_IN_LIST(a_sorted_list, 3, "Gaston", 25);

    return a_sorted_list;
}

t_list *_verify_a_sort_with_duplicates(t_list *(*sorted_list_generator)(t_list *), t_list *list) {
    list_add(list, persona_create("Ezequiel", 25));

    t_list *a_sorted_list = sorted_list_generator(list);

    REQUIRE(list_size(a_sorted_list) == 5);
    REQUIRE_PERSON_IN_LIST(a_sorted_list, 0, "Daniela", 19);
    REQUIRE_PERSON_IN_LIST(a_sorted_list, 1, "Sebastian", 21);
    REQUIRE_PERSON_IN_LIST(a_sorted_list, 2, "Matias", 24);
    REQUIRE_PERSON_IN_LIST(a_sorted_list, 3, "Gaston", 25);
    REQUIRE_PERSON_IN_LIST(a_sorted_list, 4, "Ezequiel", 25);

    return a_sorted_list;
}

TEST_SUITE("List") {
    t_person *ayudantes[5];

    TEST_CASE("Add") {
        t_list *list = list_create();

        SUBCASE("should add a value") {
            list_add(list, persona_create("Matias", 24));
            REQUIRE_PERSON_IN_LIST(list, 0, "Matias", 24);

            list_add(list, persona_create("Gaston", 25));
            REQUIRE_PERSON_IN_LIST(list, 1, "Gaston", 25);

            REQUIRE(list_size(list) == 2);
        }

        SUBCASE("should add a value at index") {
            list_add(list, persona_create("Matias", 24));
            REQUIRE_PERSON_IN_LIST(list, 0, "Matias", 24);
            REQUIRE(list_size(list) == 1);

            list_add_in_index(list, 0, persona_create("Gaston", 25));
            REQUIRE_PERSON_IN_LIST(list, 0, "Gaston", 25);
            REQUIRE_PERSON_IN_LIST(list, 1, "Matias", 24);
        }

        SUBCASE("should add a value in sorted list") {
            list_add(list, persona_create("Sebastian", 21));
            list_add(list, persona_create("Matias", 24));
            list_add(list, persona_create("Gaston", 25));

            REQUIRE(list_size(list) == 3);
            list_add_sorted(list, persona_create("Daniela", 19), (bool (*)(void *, void *)) ayudantes_menor);
            list_add_sorted(list, persona_create("Agustin", 26), (bool (*)(void *, void *)) ayudantes_menor);
            list_add_sorted(list, persona_create("Ezequiel", 25), (bool (*)(void *, void *)) ayudantes_menor);
            REQUIRE(list_size(list) == 6);

            REQUIRE_PERSON_IN_LIST(list, 0, "Daniela", 19);
            REQUIRE_PERSON_IN_LIST(list, 1, "Sebastian", 21);
            REQUIRE_PERSON_IN_LIST(list, 2, "Matias", 24);
            REQUIRE_PERSON_IN_LIST(list, 3, "Gaston", 25);
            REQUIRE_PERSON_IN_LIST(list, 4, "Ezequiel", 25);
            REQUIRE_PERSON_IN_LIST(list, 5, "Agustin", 26);
        }

        SUBCASE("should add all list into other list") {
            t_list *other = list_create();

            list_add(list, persona_create("Matias", 24));
            list_add(list, persona_create("Gaston", 25));
            list_add(list, persona_create("Sebastian", 21));
            list_add(other, persona_create("Daniela", 19));
            list_add(other, persona_create("Facundo", 25));

            REQUIRE(list_size(list) == 3);
            list_add_all(list, other);
            REQUIRE(list_size(list) == 5);

            REQUIRE_PERSON_IN_LIST(list, 0, "Matias", 24);
            REQUIRE_PERSON_IN_LIST(list, 1, "Gaston", 25);
            REQUIRE_PERSON_IN_LIST(list, 2, "Sebastian", 21);
            REQUIRE_PERSON_IN_LIST(list, 3, "Daniela", 19);
            REQUIRE_PERSON_IN_LIST(list, 4, "Facundo", 25);

            list_destroy(other);
        }

        list_destroy_and_destroy_elements(list, (void (*)(void *)) persona_destroy);
    }

    TEST_CASE("Duplicate") {
        t_list *list = list_create();

        SUBCASE("should add all elements in a new list") {
            list_add(list, persona_create("Juan", 22));
            list_add(list, persona_create("Diana", 22));

            t_list *duplicated = list_duplicate(list);
            REQUIRE(list_size(duplicated) == 2);

            REQUIRE(duplicated != list);
            REQUIRE_PERSON_IN_LIST(duplicated, 0, "Juan", 22);
            REQUIRE_PERSON_IN_LIST(duplicated, 1, "Diana", 22);

            list_destroy(duplicated);
        }

        list_destroy_and_destroy_elements(list, (void (*)(void *)) persona_destroy);
    }

    TEST_CASE("Replace, remove and destroy") {
        t_list *list = list_create();

        list_add(list, persona_create("Matias", 25));
        list_add(list, persona_create("Gaston", 24));
        list_add(list, persona_create("Sebastian", 21));
        list_add(list, persona_create("Ezequiel", 25));
        list_add(list, persona_create("Facundo", 25));

        SUBCASE("should replace a value at index with other value") {
            t_person *aux = (t_person *) list_replace(list, 3, persona_create("Daniela", 19));
            REQUIRE_PERSON(aux, "Ezequiel", 25);

            persona_destroy(aux);

            aux = (t_person *) list_get(list, 3);
            REQUIRE_PERSON(aux, "Daniela", 19);
        }

        SUBCASE("should remove a value at index") {
            REQUIRE(list_size(list) == 5);

            t_person *aux = (t_person *) list_remove(list, 0);
            REQUIRE_PERSON(aux, "Matias", 25);
            persona_destroy(aux);

            REQUIRE_PERSON_IN_LIST(list, 0, "Gaston", 24);
            REQUIRE(list_size(list) == 4);
        }

        SUBCASE("should remove and destroy a value at index") {
            REQUIRE_PERSON_IN_LIST(list, 0, "Matias", 25);
            REQUIRE(list_size(list) == 5);

            list_remove_and_destroy_element(list, 0, (void (*)(void *)) persona_destroy);

            REQUIRE_PERSON_IN_LIST(list, 0, "Gaston", 24);
            REQUIRE(list_size(list) == 4);
        }

        SUBCASE("should remove the first value that satisfies a condition") {
            REQUIRE(list_size(list) == 5);

            t_person *aux = (t_person *) list_remove_by_condition(list, (bool (*)(void *)) is_gaston);
            REQUIRE_PERSON(aux, "Gaston", 24);
            persona_destroy(aux);

            REQUIRE(list_size(list) == 4);
        }

        SUBCASE("should remove all values which satisfy a condition") {
            REQUIRE_PERSON_IN_LIST(list, 0, "Matias", 25);
            REQUIRE_PERSON_IN_LIST(list, 1, "Gaston", 24);
            REQUIRE_PERSON_IN_LIST(list, 2, "Sebastian", 21);
            REQUIRE_PERSON_IN_LIST(list, 3, "Ezequiel", 25);
            REQUIRE_PERSON_IN_LIST(list, 4, "Facundo", 25);
            REQUIRE(list_size(list) == 5);

            list_remove_and_destroy_all_by_condition(list, (bool (*)(void *)) is_25_years_old,
                                                     (void (*)(void *)) persona_destroy);

            REQUIRE_PERSON_IN_LIST(list, 0, "Gaston", 24);
            REQUIRE_PERSON_IN_LIST(list, 1, "Sebastian", 21);
            REQUIRE(list_size(list) == 2);
        }

        SUBCASE("should clean a list and leave it empty") {
            REQUIRE(list_size(list) == 5);
            list_clean_and_destroy_elements(list, (void (*)(void *)) persona_destroy);
            REQUIRE(list_is_empty(list));
        }

        list_destroy_and_destroy_elements(list, (void (*)(void *)) persona_destroy);
    }

    TEST_CASE("Higher order functions") {
        t_list *list = list_create();

        list_add(list, persona_create("Matias", 24));
        list_add(list, persona_create("Gaston", 25));
        list_add(list, persona_create("Sebastian", 21));
        list_add(list, persona_create("Ezequiel", 25));
        list_add(list, persona_create("Facundo", 25));

        SUBCASE("should find the first value that satisfies a condition") {
            REQUIRE_PERSON(find_someone_by_name(list, "Ezequiel"), "Ezequiel", 25);
            REQUIRE_PERSON(find_someone_by_name(list, "Sebastian"), "Sebastian", 21);
            REQUIRE(find_someone_by_name(list, "Chuck Norris") == NULL);
            REQUIRE(list_size(list) == 5);
        }

        SUBCASE("should filter a list with the values that satisfies a condition") {
            REQUIRE(list_size(list) == 5);

            t_list *filtered = list_filter(list, (bool (*)(void *)) is_young);
            REQUIRE(list_size(filtered) == 2);
            REQUIRE_PERSON_IN_LIST(filtered, 0, "Matias", 24);
            REQUIRE_PERSON_IN_LIST(filtered, 1, "Sebastian", 21);
            list_destroy(filtered);
        }

        SUBCASE("should map a list with the function result") {
            const char *names_array[] = {"Matias", "Gaston", "Sebastian", "Ezequiel", "Facundo"};
            t_list *names = list_map(list, (void *(*)(void *)) get_name);
            int i;
            for (i = 0; i < 5; i++) {
                REQUIRE_STRING(list_get(names, i), names_array[i]);
            }
            list_destroy(names);
        }

        list_destroy_and_destroy_elements(list, (void (*)(void *)) persona_destroy);
    }

    TEST_CASE("Take") {
        t_list *list = list_create();

        list_add(list, persona_create("Matias", 24));
        list_add(list, persona_create("Gaston", 25));
        list_add(list, persona_create("Sebastian", 21));
        list_add(list, persona_create("Ezequiel", 25));
        list_add(list, persona_create("Facundo", 25));

        SUBCASE("should return a new list with the first \"N\" elements of a list") {
            t_list *sublist = list_take(list, 3);
            REQUIRE(list_size(list) == 5);
            REQUIRE(list_size(sublist) == 3);

            REQUIRE_PERSON_IN_LIST(sublist, 0, "Matias", 24);
            REQUIRE_PERSON_IN_LIST(sublist, 1, "Gaston", 25);
            REQUIRE_PERSON_IN_LIST(sublist, 2, "Sebastian", 21);

            list_destroy(sublist);
        }

        SUBCASE("should return a new list with the first \"N\" elements starting at a given index of a list") {
            t_list *sublist = list_slice(list, 1, 3);
            REQUIRE(list_size(list) == 5);
            REQUIRE(list_size(sublist) == 3);

            REQUIRE_PERSON_IN_LIST(sublist, 0, "Gaston", 25);
            REQUIRE_PERSON_IN_LIST(sublist, 1, "Sebastian", 21);
            REQUIRE_PERSON_IN_LIST(sublist, 2, "Ezequiel", 25);

            list_destroy(sublist);
        }

        SUBCASE("should return a new list with the first \"N\" elements of a list and remove them from original list") {
            t_list *sublist = list_take_and_remove(list, 3);
            REQUIRE(list_size(list) == 2);
            REQUIRE(list_size(sublist) == 3);

            REQUIRE_PERSON_IN_LIST(sublist, 0, "Matias", 24);
            REQUIRE_PERSON_IN_LIST(sublist, 1, "Gaston", 25);
            REQUIRE_PERSON_IN_LIST(sublist, 2, "Sebastian", 21);

            list_destroy_and_destroy_elements(sublist, (void (*)(void *)) persona_destroy);
        }

        SUBCASE("should return a new list with the first \"N\" elements starting at a given index and remove them from the original list") {
            t_list *sublist = list_slice_and_remove(list, 1, 3);
            REQUIRE(list_size(list) == 2);
            REQUIRE(list_size(sublist) == 3);

            REQUIRE_PERSON_IN_LIST(sublist, 0, "Gaston", 25);
            REQUIRE_PERSON_IN_LIST(sublist, 1, "Sebastian", 21);
            REQUIRE_PERSON_IN_LIST(sublist, 2, "Ezequiel", 25);

            list_destroy_and_destroy_elements(sublist, (void (*)(void *)) persona_destroy);
        }

        SUBCASE("should return a new list with the remaining elements starting at a given index and remove them from the original list when \"N\" is too big") {
            t_list *sublist = list_slice_and_remove(list, 2, 10);
            REQUIRE(list_size(list) == 2);
            REQUIRE(list_size(sublist) == 3);

            REQUIRE_PERSON_IN_LIST(sublist, 0, "Sebastian", 21);
            REQUIRE_PERSON_IN_LIST(sublist, 1, "Ezequiel", 25);
            REQUIRE_PERSON_IN_LIST(sublist, 2, "Facundo", 25);

            list_destroy_and_destroy_elements(sublist, (void (*)(void *)) persona_destroy);
        }

        list_destroy_and_destroy_elements(list, (void (*)(void *)) persona_destroy);
    }

    TEST_CASE("Sort a list") {
        t_list *list = list_create();

        SUBCASE("Sorting a empty list") {
            SUBCASE("should sort and empty list") {
                list_sort(list, (bool (*)(void *, void *)) ayudantes_menor);
                REQUIRE(list_is_empty(list));
            }
        }

        SUBCASE("Sorting a list with elements") {
            list_add(list, persona_create("Matias", 24));
            list_add(list, persona_create("Gaston", 25));
            list_add(list, persona_create("Sebastian", 21));
            list_add(list, persona_create("Daniela", 19));

            SUBCASE("Sort - with side effect") {
                SUBCASE("should sort a list without duplicated values") {
                    _verify_a_sort_without_duplicates(_sort_actual_list, list);
                }

                SUBCASE("should sort a list with duplicated values") {
                    _verify_a_sort_with_duplicates(_sort_actual_list, list);
                }
            }

            SUBCASE("Sorted - without side effect") {
                t_list *new_list;

                SUBCASE("should sort a list without duplicated values") {
                    new_list = _verify_a_sort_without_duplicates(_generate_sorted_list, list);
                }

                SUBCASE("should sort a list with duplicated values") {
                    new_list = _verify_a_sort_with_duplicates(_generate_sorted_list, list);
                }

                list_destroy(new_list);
            }
        }

        list_destroy_and_destroy_elements(list, (void (*)(void *)) persona_destroy);
    }

    TEST_CASE("Satisfying") {
        t_list *list = list_create();

        list_add(list, persona_create("Matias", 24));
        list_add(list, persona_create("Gaston", 26));
        list_add(list, persona_create("Sebastian", 21));
        list_add(list, persona_create("Daniela", 19));

        SUBCASE("Count") {
            SUBCASE("should count the values that satisfies a condition") {
                REQUIRE(list_count_satisfying(list, (bool (*)(void *)) is_young) == 3);
            }
        }

        SUBCASE("Any") {
            SUBCASE("should not satisfy an empty list") {
                t_list *list = list_create();

                REQUIRE_FALSE(list_any_satisfy(list, (bool (*)(void *)) is_young));

                list_destroy(list);
            }

            SUBCASE("should satisfy a list with values if any of them satisfies the condition") {
                REQUIRE(list_any_satisfy(list, (bool (*)(void *)) is_young));
            }

            SUBCASE("should not satisfy a list with values if none of them satisfies the condition") {
                REQUIRE_FALSE(list_any_satisfy(list, (bool (*)(void *)) is_25_years_old));
            }
        }

        SUBCASE("All") {
            SUBCASE("should satisfy an empty list") {
                t_list *list = list_create();

                REQUIRE(list_all_satisfy(list, (bool (*)(void *)) is_young));

                list_destroy(list);
            }

            SUBCASE("should satisfy a list with values if each of them satisfies the condition") {
                REQUIRE(list_all_satisfy(list, (bool (*)(void *)) is_adult));
            }

            SUBCASE("should not satisfy a list with values if any of them not satisfy the condition") {
                REQUIRE_FALSE(list_all_satisfy(list, (bool (*)(void *)) is_young));
            }
        }

        list_destroy_and_destroy_elements(list, (void (*)(void *)) persona_destroy);
    }

    TEST_CASE("Iterator") {
        t_list *list = list_create();

        SUBCASE("should iterate all list values") {
            list_add(list, persona_create("Matias", 24));
            list_add(list, persona_create("Gaston", 25));
            list_add(list, persona_create("Sebastian", 21));
            list_add(list, persona_create("Daniela", 19));

            const char *names_array[] = {"Matias", "Gaston", "Sebastian", "Daniela"};

            int index = iterate_a_list(list, names_array, _assert_person_has_name);
            REQUIRE(index == 4);
        }

        list_destroy_and_destroy_elements(list, (void (*)(void *)) persona_destroy);
    }

    TEST_CASE("Fold1") {
        t_list *list = list_create();

        list_add(list, persona_create("Nicolas", 6));
        list_add(list, persona_create("Matias", 70));
        list_add(list, persona_create("Juan", 124));
        list_add(list, persona_create("Juan Manuel", 1));
        list_add(list, persona_create("Sebastian", 8));
        list_add(list, persona_create("Rodrigo", 40));

        SUBCASE("should fold all values into a single one, starting with first element") {
            t_person *oldestPerson = (t_person *) list_fold1(list, (void *(*)(void *, void *)) ayudantes_maximo_edad);

            REQUIRE_PERSON(oldestPerson, "Juan", 124);
        }

        SUBCASE("should get minimum") {
            t_person *youngestPerson = (t_person *) list_get_minimum(list,
                                                                     (void *(*)(void *, void *)) ayudantes_minimo_edad);

            REQUIRE_PERSON(youngestPerson, "Juan Manuel", 1);
        }

        SUBCASE("should get maximum") {
            t_person *oldestPerson = (t_person *) list_get_maximum(list,
                                                                   (void *(*)(void *, void *)) ayudantes_maximo_edad);

            REQUIRE_PERSON(oldestPerson, "Juan", 124);
        }

        list_destroy_and_destroy_elements(list, (void (*)(void *)) persona_destroy);
    }

    TEST_CASE("Fold") {
        t_list *list = list_create();

        list_add(list, persona_create("Jorge", 24));
        list_add(list, persona_create("Matias", 70));
        list_add(list, persona_create("Juan", 124));
        list_add(list, persona_create("Nicolas", 6));
        list_add(list, persona_create("Juan Manuel", 1));
        list_add(list, persona_create("Sebastian", 8));
        list_add(list, persona_create("Rodrigo", 40));

        SUBCASE("should fold all list values into a single one, depending on what the given function does") {
            t_person *seedPerson = persona_create("Maximiliano", 150);

            t_person *oldestPerson = (t_person *) list_fold(list, seedPerson,
                                                            (void *(*)(void *, void *)) ayudantes_maximo_edad);

            REQUIRE(oldestPerson->age == 150);

            persona_destroy(seedPerson);
        }

        SUBCASE("should fold all values using a intial value with different type") {
            intptr_t sum = (intptr_t) list_fold(list, (void *) 0, (void *(*)(void *, void *)) add_age);
            REQUIRE(sum == 273);
        }

        list_destroy_and_destroy_elements(list, (void (*)(void *)) persona_destroy);
    }

    TEST_CASE("Iterator") {
        t_list *list = list_create();

        list_add(list, persona_create("Juan", 124));
        list_add(list, persona_create("Nicolas", 6));
        list_add(list, persona_create("Matias", 70));
        list_add(list, persona_create("Juan Manuel", 1));
        list_add(list, persona_create("Sebastian", 8));
        list_add(list, persona_create("Rodrigo", 40));
        list_add(list, persona_create("Agustin", 21));
        list_add(list, persona_create("Juan Pablo", 33));

        SUBCASE("Should iterate all elements") {
            const char *names[] = {"Juan", "Nicolas", "Matias", "Juan Manuel", "Sebastian", "Rodrigo", "Agustin",
                                   "Juan Pablo"};

            t_list_iterator *list_iterator = list_iterator_create(list);
            while (list_iterator_has_next(list_iterator)) {
                t_person *person = (t_person *) list_iterator_next(list_iterator);
                REQUIRE_STRING(person->name, names[list_iterator->index]);
            }
            list_iterator_destroy(list_iterator);
        }

        SUBCASE("Should iterate an empty list") {
            t_list *empty_list = list_create();
            t_list_iterator *list_iterator = list_iterator_create(empty_list);
            while (list_iterator_has_next(list_iterator)) {
                t_person *person = (t_person *) list_iterator_next(list_iterator);
                REQUIRE(false);
            }
            list_iterator_destroy(list_iterator);
            list_destroy(empty_list);
        }

        SUBCASE("Should remove an element") {
            const char *names[] = {"Nicolas", "Matias", "Sebastian", "Rodrigo", "Agustin"};
            t_list_iterator *list_iterator = list_iterator_create(list);
            while (list_iterator_has_next(list_iterator)) {
                t_person *person = (t_person *) list_iterator_next(list_iterator);
                if (string_contains(person->name, "Juan")) {
                    list_iterator_remove(list_iterator);
                    persona_destroy(person);
                    continue;
                }
                REQUIRE_STRING(person->name, names[list_iterator->index]);
            }
            list_iterator_destroy(list_iterator);
            REQUIRE(list->elements_count == 5);
            REQUIRE_PERSON_IN_LIST(list, 0, "Nicolas", 6);
            REQUIRE_PERSON_IN_LIST(list, 1, "Matias", 70);
            REQUIRE_PERSON_IN_LIST(list, 2, "Sebastian", 8);
            REQUIRE_PERSON_IN_LIST(list, 3, "Rodrigo", 40);
            REQUIRE_PERSON_IN_LIST(list, 4, "Agustin", 21);
        }

        list_destroy_and_destroy_elements(list, (void (*)(void *)) persona_destroy);
    }
}
