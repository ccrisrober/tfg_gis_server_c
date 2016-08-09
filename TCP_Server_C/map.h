// Copyright (c) 2015, maldicion069 (Cristian Rodríguez) <ccrisrober@gmail.con>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.package com.example

#ifndef MAP_INCLUDE_GUARD
#define MAP_INCLUDE_GUARD

#include "keyobject.h"

typedef struct map {
	int id;
	char* mapFields;
	int width;
	int height;
	KEYOBJECT** keyObjects;
//	KEYO
} MAP;

MAP *map_create(int i, char* map, int w, int h, KEYOBJECT** keys);
void map_destroy(MAP *map);

int map_id(MAP *map);
char* map_mapfields(MAP *map);
int map_width(MAP *map);
int map_height(MAP *map);
KEYOBJECT** map_keys(MAP *map);

char* map_tojson(MAP * map);

/*int keyobject_id(MAP *keyobject);
float keyobject_posX(MAP *keyobject);
float keyobject_posY(MAP *keyobject);
char* keyobject_color(MAP *keyobject);

void keyobject_setposX(MAP *keyobject, float x);
void keyobject_setposY(MAP *keyobject, float y);
void keyobject_setposition(MAP *keyobject, float x, float y);
void keyobject_setcolor(MAP *keyobject, char* color);*/

#endif