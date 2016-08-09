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

#include "objectuser.h"

#include "json/cJSON.h"
// TODO: CAmbiar rolldie a roll dice!!!!!!!!

OBJECTUSER *objectuser_createInit(int id, float px, float py) {
	return objectuser_create(id, px, py, 0, 0);
}
OBJECTUSER *objectuser_create(int id, float px, float py, int m, int roll) {
	OBJECTUSER* objuser = malloc(sizeof(OBJECTUSER));

	objuser->id = id;
	objuser->posX = px;
	objuser->posY = py;
	objuser->map = m;
	objuser->rolldice = roll;

	return objuser;
}
void objectuser_destroy(OBJECTUSER *objuser)
{
	free(objuser);
}

int objectuser_id(OBJECTUSER *objuser)
{
	return objuser->id;
}
float objectuser_posX(OBJECTUSER *objuser)
{
	return objuser->posX;
}
float objectuser_posY(OBJECTUSER *objuser)
{
	return objuser->posY;
}
int objectuser_map(OBJECTUSER *objuser)
{
	return objuser->map;
}
int objectuser_rolldice(OBJECTUSER *objuser)
{
	return objuser->rolldice;
}

void objectuser_setposX(OBJECTUSER *objuser, float x)
{
	objuser->posX = x;
}

void objectuser_setposY(OBJECTUSER *objuser, float y)
{
	objuser->posY = y;
}

void objectuser_setposition(OBJECTUSER *objuser, float x, float y)
{
	objuser->posX = x;
	objuser->posY = y;
}

void objectuser_setmap(OBJECTUSER *objuser, int* map)
{
	objuser->map = map;
}

void objectuser_setrolldice(OBJECTUSER *objuser, int* rolldice)
{
	objuser->rolldice = rolldice;
}

cJSON* objectuser_tojson(OBJECTUSER * objuser) {
	cJSON * root = cJSON_CreateObject();

	cJSON_AddItemToObject(root, "Id", cJSON_CreateNumber(objectuser_id(objuser)));
	cJSON_AddItemToObject(root, "PosX", cJSON_CreateNumber(objectuser_posX(objuser)));
	cJSON_AddItemToObject(root, "PosY", cJSON_CreateNumber(objectuser_posY(objuser)));
	cJSON_AddItemToObject(root, "Map", cJSON_CreateNumber(objectuser_map(objuser)));
	cJSON_AddItemToObject(root, "RollDice", cJSON_CreateNumber(objectuser_rolldice(objuser)));

	return root;
}
