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

TEST_SUITE("Config") {
    TEST_CASE("Inexistent file") {
        SUBCASE("should return null when try to open a non-existent config file") {
            t_config *config = config_create("this_doesnt_exist.really.dont.cfg");
            REQUIRE_NULL(config);
        }
    }

    TEST_CASE("Existent file") {
        t_config *config = config_create("resources/config.cfg");

        REQUIRE_NON_NULL(config);

        SUBCASE("should return true if has property") {
            REQUIRE(config_has_property(config, "NUMBERS"));
        }

        SUBCASE("should return false if has not got property") {
            REQUIRE_FALSE(config_has_property(config, "PROPERY_MISSING"));
        }

        SUBCASE("should return the keys count") {
            REQUIRE_EQ(config_keys_amount(config), 8);
        }

        SUBCASE("Get") {
            SUBCASE("should get int value") {
                REQUIRE_EQ(config_get_int_value(config, "PORT"), 8080);
            }

            SUBCASE("should get string value") {
                REQUIRE_STRING(config_get_string_value(config, "IP"), "127.0.0.1");
            }

            SUBCASE("should get string value with equals sign in value") {
                REQUIRE_STRING(config_get_string_value(config, "WITH_EQUALS"), "this=value");
            }

            SUBCASE("should get double value") {
                REQUIRE_EQ(config_get_double_value(config, "LOAD"), 0.5);
            }

            SUBCASE("should get an empty array value") {
                REQUIRE_STRING(config_get_string_value(config, "EMPTY_ARRAY"), "[]");
                char **empty_array = config_get_array_value(config, "EMPTY_ARRAY");

                char *empty_array_expected[] = {NULL};
                REQUIRE_STRING_ARRAY(empty_array_expected, empty_array, 0);

                string_array_destroy(empty_array);
            }

            SUBCASE("should get an array with values") {
                char *numbers_expected[] = {"1", "2", "3", "4", "5", NULL};
                REQUIRE_STRING(config_get_string_value(config, "NUMBERS"), "[1, 2, 3, 4, 5]");

                char **numbers = config_get_array_value(config, "NUMBERS");
                REQUIRE_STRING_ARRAY(numbers_expected, numbers, 5);

                string_array_destroy(numbers);
            }

            SUBCASE("should get an array with values without spaces between entries") {
                char *strings_expected[] = {"One", "String", "Next", "to", "another", NULL};
                REQUIRE_STRING(config_get_string_value(config, "NO_SPACES"), "[One,String,Next,to,another]");

                char **strings = config_get_array_value(config, "NO_SPACES");
                REQUIRE_STRING_ARRAY(strings_expected, strings, 5);

                string_array_destroy(strings);
            }
        }

        config_destroy(config);
    }

    TEST_CASE("Double newline") {
        SUBCASE("should not fail when the config file ends with two newlines") {
            t_config *config = config_create("resources/config-double-newline.cfg");
            config_destroy(config);
        }
    }

    TEST_CASE("Set value") {
        t_config *config = config_create("resources/config.cfg");

        REQUIRE_NON_NULL(config);

        SUBCASE("should override the value given an existing key") {
            char *key = "PORT";
            char *expected = "3000";
            config_set_value(config, key, expected);
            REQUIRE_STRING(config_get_string_value(config, key), expected);
        }

        SUBCASE("should add an non existing value to the config") {
            char *key = "ANEWKEY";
            char *expected = "lorem ipsum";
            config_set_value(config, key, expected);
            REQUIRE_STRING(config_get_string_value(config, key), expected);
        }

        config_destroy(config);
    }

    TEST_CASE("Write config file") {
        t_config *config = config_create("resources/config.cfg");
        char *new_file_path = "resources/test_save_config.cfg";

        REQUIRE_NON_NULL(config);

        SUBCASE("should create a file with the specified config") {
            int result = config_save_in_file(config, new_file_path);
            REQUIRE_EQ(result, 1);
            REQUIRE_EQ(access(new_file_path, F_OK), 0);
        }

        SUBCASE("should override a config") {
            char *key = "PORT";
            char *expected = "3000";
            config_set_value(config, key, expected);
            config_save_in_file(config, new_file_path);
            t_config *new_config = config_create(new_file_path);
            REQUIRE_STRING(config_get_string_value(new_config, key), expected);
            config_destroy(new_config);
        }

        SUBCASE("should create a config file without the specified key") {
            char *key = "PORT";

            config_remove_key(config, key);
            config_save_in_file(config, new_file_path);
            t_config *new_config = config_create(new_file_path);

            REQUIRE_FALSE(config_has_property(new_config, key));
            REQUIRE(config_has_property(new_config, "WITH_EQUALS"));
            REQUIRE(config_has_property(new_config, "NUMBERS"));

            config_destroy(new_config);
        }

        remove(new_file_path);
        config_destroy(config);
    }
}
