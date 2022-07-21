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


#include "iterators.h"

int iterate_a_dictionary(t_dictionary *self, const char **expected_keys,
                         void (*assert_function)(char *got, const char *expected)) {
    int iterations = 0;
    void _iterator(char *key, void *_) {
        assert_function(key, expected_keys[iterations]);
        ++iterations;
    }
    dictionary_iterator(self, _iterator);

    return iterations;
}

int iterate_a_list(t_list *self, const char **expected_names,
                   void (*assert_function)(t_person *got, const char *expected)) {
    int iterations = 0;
    void _iterator(t_person *person) {
        assert_function(person, expected_names[iterations]);
        ++iterations;
    }
    list_iterate(self, (void *) _iterator);

    return iterations;
}

int iterate_a_string_array(char **self, const char **expected_lines,
                           void (*assert_function)(char *got, const char *expected)) {
    int iterations = 0;
    void _iterator(char *got) {
        assert_function(got, expected_lines[iterations]);
        ++iterations;
    }
    string_iterate_lines(self, _iterator);

    return iterations;
}
