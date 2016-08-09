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

#ifndef KEY_OBJECT_INCLUDE_GUARD
#define KEY_OBJECT_INCLUDE_GUARD

#include "json/cJSON.h"

typedef struct keyobject {
	int id;
	float posX;
	float posY;
	char* color;
} KEYOBJECT;

KEYOBJECT *keyobject_create(int i, float px, float py, char* c);
void keyobject_destroy(KEYOBJECT *keyobject);

int keyobject_id(KEYOBJECT *keyobject);
float keyobject_posX(KEYOBJECT *keyobject);
float keyobject_posY(KEYOBJECT *keyobject);
char* keyobject_color(KEYOBJECT *keyobject);

void keyobject_setposX(KEYOBJECT *keyobject, float x);
void keyobject_setposY(KEYOBJECT *keyobject, float y);
void keyobject_setposition(KEYOBJECT *keyobject, float x, float y);
void keyobject_setcolor(KEYOBJECT *keyobject, char* color);

cJSON* keyobject_tojson(KEYOBJECT* keyobject);


#endif