/*
 * Copyright (C) 2012 Sistemas Operativos - UTN FRBA. All rights reserved.
 *
 * This program is free software: you can redistribute SUBCASE and/or modify
 * SUBCASE under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that SUBCASE will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "asserts.h"

void pop_assert_and_destroy(t_queue *queue, char *name, int age) {
    t_person *aux = (t_person *) queue_pop(queue);
    REQUIRE_PERSON(aux, name, age);
    persona_destroy(aux);
}

TEST_SUITE("Queue") {
    TEST_CASE("Read") {
        t_queue *queue = queue_create();

        SUBCASE("should return the value at top of queue without change the queue") {
            queue_push(queue, persona_create("Matias", 24));
            queue_push(queue, persona_create("Gaston", 25));
            REQUIRE_EQ(queue_size(queue), 2);

            REQUIRE_PERSON(queue_peek(queue), "Matias", 24);
            REQUIRE_EQ(queue_size(queue), 2);

            REQUIRE_PERSON(queue_peek(queue), "Matias", 24);
            REQUIRE_EQ(queue_size(queue), 2);
        }

        SUBCASE("should clean the queue and leave SUBCASE empty") {
            queue_push(queue, persona_create("Matias", 24));
            queue_push(queue, persona_create("Gaston", 25));

            REQUIRE_EQ(queue_size(queue), 2);

            queue_clean_and_destroy_elements(queue, (void (*)(void *)) persona_destroy);
            REQUIRE(queue_is_empty(queue));
        }

        queue_destroy_and_destroy_elements(queue, (void (*)(void *)) persona_destroy);
    }

    TEST_CASE("Push and pop") {
        t_queue *queue = queue_create();

        SUBCASE("should push some values") {
            REQUIRE_EQ(queue_size(queue), 0);
            queue_push(queue, persona_create("Matias", 24));
            REQUIRE_EQ(queue_size(queue), 1);
            queue_push(queue, persona_create("Gaston", 25));
            REQUIRE_EQ(queue_size(queue), 2);
        }

        SUBCASE("should pop some values") {
            queue_push(queue, persona_create("Matias", 24));
            queue_push(queue, persona_create("Gaston", 25));

            REQUIRE_EQ(queue_size(queue), 2);

            pop_assert_and_destroy(queue, "Matias", 24);
            REQUIRE_EQ(queue_size(queue), 1);

            pop_assert_and_destroy(queue, "Gaston", 25);
            REQUIRE_EQ(queue_size(queue), 0);
        }

        queue_destroy_and_destroy_elements(queue, (void (*)(void *)) persona_destroy);
    }
}
