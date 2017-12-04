/*
 * Copyright © 2012 Intel Corporation
 * Copyright © 2013 Collabora, Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "config.h"

#include <linux/input.h>

#include "weston-test-client-helper.h"

static void
check_pointer(struct client *client, int x, int y)
{
	int sx, sy;

	/* check that the client got the global pointer update */
	assert(client->test->pointer_x == x);
	assert(client->test->pointer_y == y);

	/* Does global pointer map onto the surface? */
	if (surface_contains(client->surface, x, y)) {
		/* check that the surface has the pointer focus */
		assert(client->input->pointer->focus == client->surface);

		/*
		 * check that the local surface pointer maps
		 * to the global pointer.
		 */
		sx = client->input->pointer->x + client->surface->x;
		sy = client->input->pointer->y + client->surface->y;
		assert(sx == x);
		assert(sy == y);
	} else {
		/*
		 * The global pointer does not map onto surface.  So
		 * check that it doesn't have the pointer focus.
		 */
		assert(client->input->pointer->focus == NULL);
	}
}

static void
check_pointer_move(struct client *client, int x, int y)
{
	weston_test_move_pointer(client->test->weston_test, x, y);
	client_roundtrip(client);
	check_pointer(client, x, y);
}

TEST(test_pointer_top_left)
{
	struct client *client;
	int x, y;

	client = create_client_and_test_surface(46, 76, 111, 134);
	assert(client);

	/* move pointer outside top left */
	x = client->surface->x - 1;
	y = client->surface->y - 1;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer on top left */
	x += 1; y += 1;
	assert(surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer outside top left */
	x -= 1; y -= 1;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);
}

TEST(test_pointer_bottom_left)
{
	struct client *client;
	int x, y;

	client = create_client_and_test_surface(99, 100, 100, 98);
	assert(client);

	/* move pointer outside bottom left */
	x = client->surface->x - 1;
	y = client->surface->y + client->surface->height;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer on bottom left */
	x += 1; y -= 1;
	assert(surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer outside bottom left */
	x -= 1; y += 1;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);
}

TEST(test_pointer_top_right)
{
	struct client *client;
	int x, y;

	client = create_client_and_test_surface(48, 100, 67, 100);
	assert(client);

	/* move pointer outside top right */
	x = client->surface->x + client->surface->width;
	y = client->surface->y - 1;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer on top right */
	x -= 1; y += 1;
	assert(surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer outside top right */
	x += 1; y -= 1;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);
}

TEST(test_pointer_bottom_right)
{
	struct client *client;
	int x, y;

	client = create_client_and_test_surface(100, 123, 100, 69);
	assert(client);

	/* move pointer outside bottom right */
	x = client->surface->x + client->surface->width;
	y = client->surface->y + client->surface->height;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer on bottom right */
	x -= 1; y -= 1;
	assert(surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer outside bottom right */
	x += 1; y += 1;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);
}

TEST(test_pointer_top_center)
{
	struct client *client;
	int x, y;

	client = create_client_and_test_surface(100, 201, 100, 50);
	assert(client);

	/* move pointer outside top center */
	x = client->surface->x + client->surface->width/2;
	y = client->surface->y - 1;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer on top center */
	y += 1;
	assert(surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer outside top center */
	y -= 1;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);
}

TEST(test_pointer_bottom_center)
{
	struct client *client;
	int x, y;

	client = create_client_and_test_surface(100, 45, 67, 100);
	assert(client);

	/* move pointer outside bottom center */
	x = client->surface->x + client->surface->width/2;
	y = client->surface->y + client->surface->height;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer on bottom center */
	y -= 1;
	assert(surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer outside bottom center */
	y += 1;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);
}

TEST(test_pointer_left_center)
{
	struct client *client;
	int x, y;

	client = create_client_and_test_surface(167, 45, 78, 100);
	assert(client);

	/* move pointer outside left center */
	x = client->surface->x - 1;
	y = client->surface->y + client->surface->height/2;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer on left center */
	x += 1;
	assert(surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer outside left center */
	x -= 1;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);
}

TEST(test_pointer_right_center)
{
	struct client *client;
	int x, y;

	client = create_client_and_test_surface(110, 37, 100, 46);
	assert(client);

	/* move pointer outside right center */
	x = client->surface->x + client->surface->width;
	y = client->surface->y + client->surface->height/2;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer on right center */
	x -= 1;
	assert(surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);

	/* move pointer outside right center */
	x += 1;
	assert(!surface_contains(client->surface, x, y));
	check_pointer_move(client, x, y);
}

TEST(test_pointer_surface_move)
{
	struct client *client;

	client = create_client_and_test_surface(100, 100, 100, 100);
	assert(client);

	/* move pointer outside of client */
	assert(!surface_contains(client->surface, 50, 50));
	check_pointer_move(client, 50, 50);

	/* move client center to pointer */
	move_client(client, 0, 0);
	assert(surface_contains(client->surface, 50, 50));
	check_pointer(client, 50, 50);
}

TEST(simple_pointer_button_test)
{
	struct client *client;
	struct pointer *pointer;

	client = create_client_and_test_surface(100, 100, 100, 100);
	assert(client);

	pointer = client->input->pointer;

	assert(pointer->button == 0);
	assert(pointer->state == 0);

	weston_test_move_pointer(client->test->weston_test, 150, 150);
	client_roundtrip(client);
	assert(pointer->x == 50);
	assert(pointer->y == 50);

	weston_test_send_button(client->test->weston_test, BTN_LEFT,
			    WL_POINTER_BUTTON_STATE_PRESSED);
	client_roundtrip(client);
	assert(pointer->button == BTN_LEFT);
	assert(pointer->state == WL_POINTER_BUTTON_STATE_PRESSED);

	weston_test_send_button(client->test->weston_test, BTN_LEFT,
			    WL_POINTER_BUTTON_STATE_RELEASED);
	client_roundtrip(client);
	assert(pointer->button == BTN_LEFT);
	assert(pointer->state == WL_POINTER_BUTTON_STATE_RELEASED);
}