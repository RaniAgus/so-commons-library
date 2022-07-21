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

/**
 * TODO: This tests are 8-BIT dependant. We should think of a way to abstract them.
 * TODO: Can we abstract the LSB and MSB tests so they get unified?
 */
TEST_SUITE("Bitarray") {
    TEST_CASE("LSB Bitarray") {
        t_bitarray *bitarray;

        SUBCASE("should get the bit value at position") {
            char data[] = {(char) 0b10000000, 0, 0b00000001};
            bitarray = bitarray_create_with_mode(data, sizeof(data), LSB_FIRST);

            REQUIRE(bitarray_test_bit(bitarray, 7) == true);
            REQUIRE(bitarray_test_bit(bitarray, 1) == false);
            REQUIRE(bitarray_test_bit(bitarray, 8 + 8 + 0) == true);
        }

        SUBCASE("should clean the bit value at position") {
            char BASE_ARRAY[] = {0, 0, 0};
            char data[] = {0, 0, 0b00000001};
            bitarray = bitarray_create_with_mode(data, sizeof(data), LSB_FIRST);

            bitarray_clean_bit(bitarray, 8 + 8 + 0);

            REQUIRE(memcmp(BASE_ARRAY, data, sizeof(data)) == 0);
        }

        SUBCASE("should set the bit value at position") {
            char BASE_ARRAY[] = {(char) 0b10000000, 0, 0};
            char data[] = {0, 0, 0};
            bitarray = bitarray_create_with_mode(data, sizeof(data), LSB_FIRST);

            bitarray_set_bit(bitarray, 7);

            REQUIRE(memcmp(BASE_ARRAY, data, sizeof(data)) == 0);
        }

        SUBCASE("should get the bits count") {
            char data[] = {0, 0, 0};
            bitarray = bitarray_create_with_mode(data, sizeof(data), LSB_FIRST);

            REQUIRE(bitarray_get_max_bit(bitarray) == (8 * 3));
        }

        bitarray_destroy(bitarray);
    }

    TEST_CASE("MSB Bitarray") {
        t_bitarray *bitarray;

        SUBCASE("should get the bit value at position") {
            char data[] = {(char) 0b10000000, 0, (char) 0b10000000};
            bitarray = bitarray_create_with_mode(data, sizeof(data), MSB_FIRST);

            REQUIRE(bitarray_test_bit(bitarray, 0) == true);
            REQUIRE(bitarray_test_bit(bitarray, 7) == false);
            REQUIRE(bitarray_test_bit(bitarray, 8 + 8 + 0) == true);
        }

        SUBCASE("should clean the bit value at position") {
            char BASE_ARRAY[] = {0, 0, 0};
            char data[] = {0, 0, (char) 0b10000000};
            bitarray = bitarray_create_with_mode(data, sizeof(data), MSB_FIRST);

            bitarray_clean_bit(bitarray, 8 + 8 + 0);

            REQUIRE(memcmp(BASE_ARRAY, data, sizeof(data)) == 0);
        }

        SUBCASE("should set the bit value at position") {
            char BASE_ARRAY[] = {(char) 0b10000000, 0, 0};
            char data[] = {0, 0, 0};
            bitarray = bitarray_create_with_mode(data, sizeof(data), MSB_FIRST);

            bitarray_set_bit(bitarray, 0);

            REQUIRE(memcmp(BASE_ARRAY, data, sizeof(data)) == 0);
        }

        SUBCASE("should get the bits count") {
            char data[] = {0, 0, 0};
            bitarray = bitarray_create_with_mode(data, sizeof(data), MSB_FIRST);

            REQUIRE(bitarray_get_max_bit(bitarray) == (8 * 3));
        }

        bitarray_destroy(bitarray);
    }
}
