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

void _assert_names(char *name, const char *expected) {
    REQUIRE_NON_NULL(name);
    REQUIRE_EQ(name, expected);
}

TEST_SUITE("String") {
    TEST_CASE("itoa") {
        char *newString = string_itoa(15);
        REQUIRE_EQ(newString, "15");
        free(newString);
    }

    TEST_CASE("from_format") {
        char *newString = string_from_format("%s %s %d", "Hello", "world", 23);
        REQUIRE_EQ(newString, "Hello world 23");
        free(newString);
    }

    TEST_CASE("append") {
        char *string = string_new();
        string_append(&string, "Hello");
        string_append(&string, " ");
        string_append(&string, "world");

        REQUIRE_EQ(string, "Hello world");

        free(string);
    }

    TEST_CASE("append_with_format") {
        char *string = string_new();

        string_append_with_format(&string, "%s %s %d", "Hello", "world", 23);
        REQUIRE_EQ(string, "Hello world 23");

        free(string);
    }

    TEST_CASE("equals_ignore_case") {
        char *string = string_duplicate("Hello WorLd");

        REQUIRE(string_equals_ignore_case(string, "hello world"));

        free(string);
    }

    TEST_CASE("repeat") {
        char *string = string_repeat('a', 10);

        REQUIRE_EQ(string, "aaaaaaaaaa");
        REQUIRE_EQ(strlen(string), 10);

        free(string);
    }

    TEST_CASE("to_upper") {
        char *string = string_duplicate("Hello World");
        string_to_upper(string);

        REQUIRE_EQ(string, "HELLO WORLD");

        free(string);
    }

    TEST_CASE("to_lower") {
        char *string = string_duplicate("Hello World");
        string_to_lower(string);

        REQUIRE_EQ(string, "hello world");

        free(string);
    }

    TEST_CASE("capitalized") {
        char *string;

        string = string_duplicate("hello world");
        string_capitalized(string);
        REQUIRE_EQ(string, "Hello world");
        free(string);

        string = string_duplicate("HELLO WORLD");
        string_capitalized(string);
        REQUIRE_EQ(string, "Hello world");
        free(string);

        string = string_duplicate("hi");
        string_capitalized(string);
        REQUIRE_EQ(string, "Hi");
        free(string);

        string = string_duplicate("h");
        string_capitalized(string);
        REQUIRE_EQ(string, "H");
        free(string);
    }

    TEST_CASE("Trim") {
        SUBCASE("trim_left") {
            char *string;

            string = string_duplicate("Hola");
            string_trim_left(&string);
            REQUIRE_EQ(string, "Hola");
            free(string);

            string = string_duplicate("   Hola");
            string_trim_left(&string);
            REQUIRE_EQ(string, "Hola");
            free(string);
        }

        SUBCASE("trim_right") {
            char *string;

            string = string_duplicate("Hola");
            string_trim_right(&string);
            REQUIRE_EQ(string, "Hola");
            free(string);

            string = string_duplicate("Hola    ");
            string_trim_right(&string);
            REQUIRE_EQ(string, "Hola");
            free(string);
        }

        SUBCASE("trim") {
            char *string;

            string = string_duplicate("Hola");
            string_trim(&string);
            REQUIRE_EQ(string, "Hola");
            free(string);

            string = string_duplicate("    Hola");
            string_trim(&string);
            REQUIRE_EQ(string, "Hola");
            free(string);

            string = string_duplicate("Hola    ");
            string_trim(&string);
            REQUIRE_EQ(string, "Hola");
            free(string);

            string = string_duplicate("    Hola    ");
            string_trim(&string);
            REQUIRE_EQ(string, "Hola");
            free(string);
        }
    }

    TEST_CASE("is_empty") {
        char *string = string_new();
        REQUIRE(string_is_empty(string));
        free(string);
    }

    TEST_CASE("Split") {
        char **substrings;

        SUBCASE("split_with_separators") {
            char *line = "path//to//file";
            substrings = string_split(line, "//");

            char *expected[] = { "path", "to", "file", NULL };
            REQUIRE_STRING_ARRAY(substrings, expected, 3);
        }

        SUBCASE("split_with_empty_string_as_separator") {
            char *line = "hello";
            substrings = string_split(line, "");

            char *expected[] = { "h", "e", "l", "l", "o", NULL };
            REQUIRE_STRING_ARRAY(substrings, expected, 5);
        }

        SUBCASE("split_with_null_separator") {
            char *line = "path/to/file";
            substrings = string_split(line, NULL);

            char *expected[] = { "path/to/file", NULL };
            REQUIRE_STRING_ARRAY(substrings, expected, 1);
        }

        SUBCASE("split_starting_with_separator") {
            char *line = "/path/to/file";
            substrings = string_split(line, "/");

            char *expected[] = { "", "path", "to", "file", NULL };
            REQUIRE_STRING_ARRAY(substrings, expected, 4);
        }

        SUBCASE("split_ending_with_separator") {
            char *line = "path/to/file/";
            substrings = string_split(line, "/");

            char *expected[] = { "path", "to", "file", "", NULL };
            REQUIRE_STRING_ARRAY(substrings, expected, 4);
        }

        SUBCASE("split_having_separators_in_between") {
            char *line = "path/to//file";
            substrings = string_split(line, "/");

            char *expected[] = { "path", "to", "", "file", NULL };
            REQUIRE_STRING_ARRAY(substrings, expected, 4);
        }

        SUBCASE("split_is_empty") {
            char *line = "";
            substrings = string_split(line, "/");

            char *expected[] = { "", NULL };
            REQUIRE_STRING_ARRAY(substrings, expected, 1);
        }

        SUBCASE("split_is_empty_with_empty_separator") {
            char *line = "";
            substrings = string_split(line, "");

            char *expected[] = { "", NULL };
            REQUIRE_STRING_ARRAY(substrings, expected, 1);
        }

        SUBCASE("n_split_when_n_is_less_than_splitted_elements") {
            char *line = "Hola planeta tierra";
            substrings = string_n_split(line, 2, " ");

            char *expected[] = { "Hola", "planeta tierra", NULL };
            REQUIRE_STRING_ARRAY(substrings, expected, 2);
        }

        SUBCASE("n_split_when_n_is_equals_than_splitted_elements") {
            char *line = "Hola planeta tierra";
            substrings = string_n_split(line, 3, " ");

            char *expected[] = { "Hola", "planeta", "tierra", NULL };
            REQUIRE_STRING_ARRAY(substrings, expected, 3);
        }

        SUBCASE("n_split_when_separator_isnt_included") {
            char *line = "Hola planeta tierra";
            substrings = string_n_split(line, 5, ";");

            char *expected[] = { "Hola planeta tierra", NULL };
            REQUIRE_STRING_ARRAY(substrings, expected, 1);
        }

        SUBCASE("n_split_when_n_is_greater_than_splitted_elements") {
            char *line = "Hola planeta tierra";
            substrings = string_n_split(line, 10, " ");

            char *expected[] = { "Hola", "planeta", "tierra", NULL };
            REQUIRE_STRING_ARRAY(substrings, expected, 3);
        }

        SUBCASE("n_split_is_empty") {
            char *line = "";
            substrings = string_n_split(line, 10, ";");

            char *expected[] = { "", NULL };
            REQUIRE_STRING_ARRAY(substrings, expected, 1);
        }

        string_array_destroy(substrings);
    }

    TEST_CASE("starts_with") {
        REQUIRE(string_starts_with("#Comentario", "#"));
        REQUIRE(string_starts_with("Comentario", "Comen"));
        REQUIRE_FALSE(string_starts_with("Comentario", "comen"));
        REQUIRE_FALSE(string_starts_with("Comentario", "lala"));
        REQUIRE_FALSE(string_starts_with("", "#"));
    }

    TEST_CASE("ends_with") {
        REQUIRE(string_ends_with("MiArchivo.txt", "txt"));
        REQUIRE(string_ends_with("MiArchivo.txt", "MiArchivo.txt"));
        REQUIRE_FALSE(string_ends_with("MiArchivo.doc", "txt"));
        REQUIRE_FALSE(string_ends_with("MiDoc", "txt"));
        REQUIRE_FALSE(string_ends_with("", "txt"));
    }

    TEST_CASE("Substring") {
        SUBCASE("substring_empty") {
            char *original_word = "";
            char *substring = string_substring(original_word, 0, 3);
            REQUIRE_EQ(substring, original_word);
            free(substring);
        }

        SUBCASE("substring_with_short_string") {
            char *original_word = "hola";
            char *substring = string_substring(original_word, 0, 14);
            REQUIRE_EQ(substring, original_word);
            free(substring);
        }

        SUBCASE("substring_with_large_string") {
            char *original_word = "hola mundo c!";
            char *substring = string_substring(original_word, 0, 4);
            REQUIRE_EQ(substring, "hola");
            REQUIRE_NE(substring, original_word);
            free(substring);
        }

        SUBCASE("substring_from_other_start") {
            char *original_word = "hola mundo!";
            char *substring = string_substring(original_word, 5, strlen(original_word) - 5);
            REQUIRE_EQ(substring, "mundo!");
            REQUIRE_NE(substring, original_word);
            free(substring);
        }

        SUBCASE("substring_extract_internal_text") {
            char *original_word = "hola mundo";
            char *substring = string_substring(original_word, 2, 5);
            REQUIRE_EQ(substring, "la mu");
            REQUIRE_NE(substring, original_word);
            free(substring);
        }

        SUBCASE("substring_with_equal_large") {
            char *original_word = "hola";
            char *substring = string_substring(original_word, 0, 4);
            REQUIRE_EQ(substring, original_word);
            free(substring);
        }

        SUBCASE("substring_from_middle") {
            char *original = "hola mundo!";
            char *substring = string_substring_from(original, 5);
            REQUIRE_EQ(substring, "mundo!");
            free(substring);
        }

        SUBCASE("substring_from_start") {
            char *original = "hola mundo!";
            char *substring = string_substring_from(original, 0);
            REQUIRE_EQ(substring, original);
            free(substring);
        }

        SUBCASE("substring_until_middle") {
            char *original = "hola mundo!";
            char *substring = string_substring_until(original, 5);
            REQUIRE_EQ(substring, "hola ");
            free(substring);
        }

        SUBCASE("substring_until_end") {
            char *original = "hola mundo!";
            char *substring = string_substring_until(original, strlen(original));
            REQUIRE_EQ(substring, original);
            free(substring);
        }
    }

    TEST_CASE("String as array") {
        SUBCASE("get_string_as_array_empty") {
            char *string_empty_array = "[]";
            char **empty_array = string_get_string_as_array(string_empty_array);
            REQUIRE_NON_NULL(empty_array);
            REQUIRE_NULL(empty_array[0]);

            string_array_destroy(empty_array);
        }

        SUBCASE("get_string_full_array") {
            char *numbers = "[1, 2, 3, 4, 5]";
            char **numbers_array = string_get_string_as_array(numbers);
            REQUIRE_NON_NULL(numbers_array);
            REQUIRE_NULL(numbers_array[5]);

            int i;
            for (i = 1; i <= 5; ++i) {
                char *value = string_from_format("%d", i);
                REQUIRE_EQ(numbers_array[i - 1], value);
                free(value);
            }

            string_array_destroy(numbers_array);
        }
    }

    TEST_CASE("length") {
        REQUIRE_EQ(string_length("hola"), 4);
    }

    TEST_CASE("Reverse") {
        SUBCASE("reverse with length of a string is even") {
            char *word = "CASA";
            char *reverse_word = string_reverse(word);
            REQUIRE_EQ(reverse_word, "ASAC");
            free(reverse_word);
        }

        SUBCASE("reverse with length of a string is odd") {
            char *word = "FRUTA";
            char *reverse_word = string_reverse(word);
            REQUIRE_EQ(reverse_word, "ATURF");
            free(reverse_word);
        }

        SUBCASE("reverse with empty string") {
            char *word = "";
            char *reverse_word = string_reverse(word);
            REQUIRE_EQ(reverse_word, "");
            free(reverse_word);
        }
    }

    TEST_CASE("Contains") {
        REQUIRE(string_contains("Pablito clavo un clavito", "Pablito"));
        REQUIRE_FALSE(string_contains("Pablito clavo un clavito", "pablito"));
        REQUIRE(string_contains("Pablito clavo un clavito", "lito"));
        REQUIRE(string_contains("Pablito clavo un clavito", ""));
        REQUIRE_FALSE(string_contains("", "Pablito clavo un clavito"));
        REQUIRE(string_contains("", ""));
    }

    TEST_CASE("String array") {
        char **names = string_array_new();

        string_array_push(&names, "Gaston");
        string_array_push(&names, "Matias");
        string_array_push(&names, "Sebastian");
        string_array_push(&names, "Daniela");

        SUBCASE("add an element at the end") {
            string_array_push(&names, "Agustin");

            REQUIRE_EQ(string_array_size(names), 5);

            const char *expected[] = {"Gaston", "Matias", "Sebastian", "Daniela", "Agustin", NULL};
            REQUIRE_STRING_ARRAY(names, expected, 5);
        }

        SUBCASE("remove the last element") {
            char *name = string_array_pop(names);

            REQUIRE_EQ(name, "Daniela");

            REQUIRE_EQ(string_array_size(names), 3);

            const char *expected[] = {"Gaston", "Matias", "Sebastian", NULL};
            REQUIRE_STRING_ARRAY(names, expected, 3);
        }

        SUBCASE("replace an element") {
            char *name = string_array_replace(names, 2, "Damian");

            REQUIRE_EQ(name, "Sebastian");

            const char *expected[] = {"Gaston", "Matias", "Damian", "Daniela", NULL};
            REQUIRE_STRING_ARRAY(names, expected, 4);
        }

        free(names);
    }
}
