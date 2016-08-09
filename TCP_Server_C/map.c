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

#include "map.h"
#include <stdio.h>
#include <string.h>

MAP *map_create(int i, char* m, int w, int h, KEYOBJECT** keys)
{
	MAP* map = malloc(sizeof(MAP));

	map->id = i;
	map->mapFields = m;
	map->width = w;
	map->height = h;
	map->keyObjects = keys;

	return map;

}

void map_destroy(MAP *map)
{
	free(map);
}

int map_id(MAP *map)
{
	return map->id;
}

char* map_mapfields(MAP *map)
{
	return map->mapFields;
}

int map_width(MAP *map)
{
	return map->width;
}

int map_height(MAP *map)
{
	return map->height;
}

KEYOBJECT** map_keys(MAP *map)
{
	return map->keyObjects;
}

char* map_tojson(MAP * map)
{
	char ret[2560];
	sprintf(ret, "{\"Id\":%d,\"Width\":%d,\"Height\":%d,\"MapFields\":\"%s", map->id, map->width, map->height, map->mapFields);
	printf("%d", strlen(ret));
	return ret;
}