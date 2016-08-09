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

#include "keyobject.h"

#include "json/cJSON.h"


KEYOBJECT *keyobject_create(int i, float px, float py, char* c)
{
	KEYOBJECT* keyobject = malloc(sizeof(KEYOBJECT));

	keyobject->id = i;
	keyobject->posX = px;
	keyobject->posY = py;
	keyobject->color = c;

	return keyobject;
}

void keyobject_destroy(KEYOBJECT *keyobject)
{
	free(keyobject);
}

int keyobject_id(KEYOBJECT *keyobject)
{
	return keyobject->id;
}

float keyobject_posX(KEYOBJECT *keyobject)
{
	return keyobject->posX;
}

float keyobject_posY(KEYOBJECT *keyobject)
{
	return keyobject->posY;
}

char* keyobject_color(KEYOBJECT *keyobject)
{
	return keyobject->color;
}

void keyobject_setposX(KEYOBJECT *keyobject, float x)
{
	keyobject->posX = x;
}

void keyobject_setposY(KEYOBJECT *keyobject, float y)
{
	keyobject->posY = y;
}

void keyobject_setposition(KEYOBJECT *keyobject, float x, float y)
{
	keyobject->posX = x;
	keyobject->posY = y;
}

void keyobject_setcolor(KEYOBJECT *keyobject, char* color)
{
	keyobject->color = color;
}

cJSON* keyobject_tojson(KEYOBJECT* keyobject)
{
	cJSON *key = cJSON_CreateObject();

	cJSON_AddNumberToObject(key, "Id", keyobject_id(keyobject));
	cJSON_AddNumberToObject(key, "PosX", keyobject_posX(keyobject));
	cJSON_AddNumberToObject(key, "PosY", keyobject_posY(keyobject));
	cJSON_AddStringToObject(key, "Color", keyobject_color(keyobject));

	return key;
}