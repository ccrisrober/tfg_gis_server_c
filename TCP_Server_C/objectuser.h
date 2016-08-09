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

#ifndef OBJECT_USER_INCLUDE_GUARD
#define OBJECT_USER_INCLUDE_GUARD

#include "json/cJSON.h"

typedef struct objectuser {
	int id;
	float posX;
	float posY;
	int map;
	int rolldice;
} OBJECTUSER;

OBJECTUSER *objectuser_createInit(int id, float px, float py);
OBJECTUSER *objectuser_create(int id, float px, float py, int m, int roll);
void objectuser_destroy(OBJECTUSER *objuser);

int objectuser_id(OBJECTUSER *objuser);
float objectuser_posX(OBJECTUSER *objuser);
float objectuser_posY(OBJECTUSER *objuser);
int objectuser_map(OBJECTUSER *objuser);
int objectuser_rolldice(OBJECTUSER *objuser);

void objectuser_setposX(OBJECTUSER *objuser, float x);
void objectuser_setposY(OBJECTUSER *objuser, float y);
void objectuser_setposition(OBJECTUSER *objuser, float x, float y);
void objectuser_setmap(OBJECTUSER *objuser, int* map);
void objectuser_setrolldice(OBJECTUSER *objuser, int* rolldice);

cJSON* objectuser_tojson(OBJECTUSER * objuser);
#endif
