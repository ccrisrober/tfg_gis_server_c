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

#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

#include "hashtbl.h"

#include "json\cJSON.h"

#include "keyobject.h"
#include "objectuser.h"
#include "map.h"
#include <string.h>

#define MAX 10
#define PORT 8089


HASHTBL *positions, *keys;
SOCKET client_socket[MAX];

char* inttochar(int id) {
	char id_user[6];	// 5 + "/0"
	sprintf(id_user, "%d", id);
	return id_user;
}

int RandomValueInRange(int min, int max)
{
	return min + rand() % max;
}

void sendFightToAnotherClient(int emisor, int receiver)
{
	cJSON * retOthers, *ret;
	int port, i;
	char* id_user, *id_receiver;
	OBJECTUSER* obj_user;

	retOthers = cJSON_CreateObject();
	cJSON_AddStringToObject(retOthers, "Action", cJSON_CreateString("hide"));
	cJSON * arr = cJSON_CreateArray();
	cJSON_AddItemReferenceToArray(arr, cJSON_CreateNumber(emisor));
	cJSON_AddItemReferenceToArray(arr, cJSON_CreateNumber(receiver));
	cJSON_AddItemToObject(retOthers, "Ids", arr);

	ret = cJSON_CreateObject();
	cJSON_AddStringToObject(ret, "Action", cJSON_CreateString("fight"));
	cJSON_AddNumberToObject(ret, "Id_enemy", emisor);

	id_user = inttochar(emisor);
	obj_user = (OBJECTUSER*)hashtbl_get(positions, id_user);
	objectuser_setrolldie(obj_user, RandomValueInRange(1, 6));

	for (i = 0; i < MAX; i++)
	{
		// TODO port
		if (port != receiver)
		{
			id_receiver = inttochar(receiver);
			obj_user = (OBJECTUSER*)hashtbl_get(positions, id_receiver);
			objectuser_setrolldie(obj_user, RandomValueInRange(1, 6));

			//socket.Send(cJSON_PrintUnformatted(ret));
		}
		else if (port != emisor)
		{
			//socket.Send(cJSON_PrintUnformatted(retOthers));
		}

	}


	cJSON_Delete(ret);
	cJSON_Delete(retOthers);
	free(ret);
	free(retOthers);
}

void sendDiePlayerAndWinnerToShow(int num_socket, int emisor, int receiver)
{
	char * msg;
	int winner, valueC, valueE;
	char *emisor_, *receiver_;
	int roll_emisor, roll_receiver;
	cJSON *j_emisor, *j_receiver, *root;
	
	winner = -1;
	valueC = -1;
	valueE = -1;
	roll_emisor = -1;
	roll_receiver = -1;

	emisor_ = inttochar(emisor);
	receiver_ = inttochar(receiver);

	j_emisor = hashtbl_get(positions, emisor_);
	if (j_emisor != NULL)
	{
		roll_receiver = objectuser_rolldie((OBJECTUSER*)hashtbl_get(positions, emisor));
	}

	j_receiver = hashtbl_get(positions, receiver_);

	if (j_receiver != NULL)
	{
		roll_emisor = objectuser_rolldie((OBJECTUSER*)hashtbl_get(positions, receiver));
	}

	if (j_emisor == NULL)
	{
		winner = receiver;
		valueC = roll_emisor;
	}
	else if (receiver == NULL)
	{
		winner = emisor;
		valueE = roll_receiver;
	}
	else if (roll_emisor > roll_receiver)
	{
		winner = emisor;
		valueE = roll_receiver;
		valueC = roll_emisor;
	}
	else if (roll_receiver > roll_emisor)
	{
		winner = receiver;
		valueE = roll_receiver;
		valueC = roll_emisor;
	}

	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "Action", "finish");
	cJSON_AddNumberToObject(root, "ValueClient", valueC);
	cJSON_AddNumberToObject(root, "ValueEnemy", valueE);
	cJSON_AddNumberToObject(root, "Winner", winner);


	msg = cJSON_PrintUnformatted(root);

	if (send(root, msg, strlen(msg), 0) != strlen(msg))
	{
		perror("send failed");
	}

	cJSON_Delete(send);
	free(send);
}
/*

int socket_comparator(void *new_obj, void *next_obj)
{
	SOCKET * new_val = *((SOCKET*)new_obj);
	SOCKET * next_val = *((SOCKET*)next_obj);
	

}
*/

int main(int argc, char *argv[])
{
	WSADATA wsa;
	int max_clients = 3, activity, addrlen, i, valread, j;
	SOCKET master, new_socket, s, ss;
	struct sockaddr_in server, address;

	KEYOBJECT* keys_map0[4];
	int size;

	char message[5120];
	char* out;
	cJSON *json, *jaux;
	MAP* maps[1];

	//size of our receive buffer, this is string length.
	int MAXRECV = 1024;
	//set of socket descriptors
	fd_set readfds;
	//1 extra for null character, string termination
	char *buffer;
	buffer = (char*)malloc((MAXRECV + 1) * sizeof(char));

	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	printf("Initialised.\n");

	//Create a socket
	if ((master = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind
	if (bind(master, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	puts("Bind done");

	//Listen to incoming connections
	listen(master, 3);

	srand(time(0)); // seed the random numbers ONCE -- use the current time 

	if (!(positions = hashtbl_create(16, NULL))) {
		fprintf(stderr, "ERROR: hashtbl_create() failed\n");
		exit(EXIT_FAILURE);
	}

	if (!(keys = hashtbl_create(8, NULL))) {
		fprintf(stderr, "ERROR: hashtbl_create() failed\n");
		exit(EXIT_FAILURE);
	}

	hashtbl_insert(keys, "Red", keyobject_create(1, 5 * 64, 5 * 64, "Red"));
	hashtbl_insert(keys, "Blue", keyobject_create(2, 6 * 64, 5 * 64, "Blue"));
	hashtbl_insert(keys, "Yellow", keyobject_create(3, 7 * 64, 5 * 64, "Yellow"));
	hashtbl_insert(keys, "Green", keyobject_create(4, 8 * 64, 5 * 64, "Green"));
	
	keys_map0[0] = hashtbl_get(keys, "Red");
	keys_map0[1] = hashtbl_get(keys, "Blue");
	keys_map0[2] = hashtbl_get(keys, "Yellow");
	keys_map0[3] = hashtbl_get(keys, "Green");


	maps[0] = map_create(1, 
		"1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 4, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 5, 5, 7, 5, 5, 5, 5, 1, 1, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 1, 1, 1, 0, 0, 4, 6, 5, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 4, 0, 5, 5, 5, 0, 8, 8, 8, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 4, 0, 0, 0, 1, 1, 0, 5, 5, 5, 0, 0, 0, 8, 8, 8, 4, 0, 1, 1, 0, 1, 0, 0, 0, 0, 4, 0, 0, 1, 1, 1, 1, 4, 0, 0, 0, 1, 0, 5, 0, 4, 4, 0, 0, 8, 8, 8, 1, 4, 1, 4, 0, 1, 0, 0, 0, 0, 4, 4, 0, 1, 1, 1, 1, 1, 1, 4, 0, 1, 0, 5, 0, 4, 4, 4, 0, 8, 8, 8, 1, 1, 1, 1, 0, 1, 0, 0, 4, 4, 4, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 5, 4, 4, 4, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 4, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 0, 0, 4, 0, 5, 5, 5, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 4, 0, 5, 5, 5, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 5, 5, 5, 0, 0, 0, 1, 1, 1, 4, 0, 0, 0, 1, 1, 0, 5, 5, 5, 0, 0, 0, 1, 1, 1, 4, 0, 1, 1, 0, 1, 0, 5, 0, 4, 4, 0, 0, 1, 1, 1, 1, 4, 0, 0, 0, 1, 0, 5, 0, 4, 4, 0, 0, 1, 1, 1, 1, 4, 1, 4, 0, 1, 0, 5, 0, 4, 4, 4, 0, 1, 1, 1, 1, 1, 1, 4, 0, 1, 0, 5, 0, 4, 4, 4, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,", 
		32, 25, keys_map0);


	//Accept and incoming connection
	puts("Waiting for incoming connections...");

	addrlen = sizeof(struct sockaddr_in);

	while (TRUE)
	{
		//clear the socket fd set
		FD_ZERO(&readfds);

		//add master socket to fd set
		FD_SET(master, &readfds);

		//add child sockets to fd set
		for (i = 0; i < max_clients; i++)
		{
			s = client_socket[i];
			if (s > 0)
			{
				FD_SET(s, &readfds);
			}
		}

		//wait for an activity on any of the sockets, timeout is NULL , so wait indefinitely
		activity = select(0, &readfds, NULL, NULL, NULL);

		if (activity == SOCKET_ERROR)
		{
			printf("select call failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		//If something happened on the master socket , then its an incoming connection
		if (FD_ISSET(master, &readfds))
		{
			if ((new_socket = accept(master, (struct sockaddr *)&address, (int *)&addrlen))<0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}

			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d \n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

			// Add user to positions
			OBJECTUSER* user = objectuser_createInit(ntohs(address.sin_port), 5 * 64, 5 * 64);

			//send new connection greeting message
			cJSON * root = cJSON_CreateObject();
			cJSON_AddItemToObject(root, "Action", cJSON_CreateString("sendMap"));
			cJSON_AddItemToObject(root, "X", cJSON_CreateNumber(objectuser_posX(user)));
			cJSON_AddItemToObject(root, "Y", cJSON_CreateNumber(objectuser_posY(user)));
			cJSON_AddItemToObject(root, "Id", cJSON_CreateNumber(objectuser_id(user)));
			cJSON * map = cJSON_CreateObject();
			cJSON_AddNumberToObject(map, "Width", map_width(maps[0]));
			cJSON_AddNumberToObject(map, "Height", map_height(maps[0]));
			cJSON_AddNumberToObject(map, "Id", map_id(maps[0]));
			cJSON_AddStringToObject(map, "MapFields", map_mapfields(maps[0]));
			cJSON *keys = cJSON_CreateArray();

			KEYOBJECT** k = map_keys(maps[0]);

			int s1 = sizeof(map_keys(maps[0]));
			int s2 = sizeof(KEYOBJECT);

			int numObj = s2 / s1;
			int i = 0;

			for (i = 0; i < numObj; i++) {
				cJSON_AddItemReferenceToArray(keys, keyobject_tojson(k[i]));
			}

			hashtbl_insert(positions, inttochar(ntohs(address.sin_port)), user);

			// TODO: Falta enviar a todos los usuarios!!
			size = 0;
			char** users = hashtabl_keys(positions, &size);

			cJSON * pos_users = cJSON_CreateArray();
			for (i = 0; i < size; i++) {
				printf("%s\n", users[i]);
				//cJSON *u = objectuser_tojson((OBJECTUSER*) hashtbl_get(positions, users[i]));
			}
			cJSON_AddItemToObject(root, "Users", pos_users);
			// Erase array!
			for (i = 0; i < size; i++) {
				free(users[i]);
			}
			free(user);

			cJSON_AddItemToObject(map, "KeyObjects", keys);
			cJSON_AddItemToObject(root, "Map", map);

			char * msg = cJSON_PrintUnformatted(root);

			cJSON_Delete(root);			

			if (send(new_socket, msg, strlen(msg), 0) != strlen(msg))
			{
				perror("send failed");
			}

			puts("Welcome message sent successfully");

			//add new socket to array of sockets
			for (i = 0; i < max_clients; i++)
			{
				if (client_socket[i] == 0)
				{
					client_socket[i] = new_socket;
					printf("Adding to list of sockets at index %d \n", i);
					break;
				}
			}
		}

		//else its some IO operation on some other socket :)
		for (i = 0; i < max_clients; i++)
		{
			s = client_socket[i];
			//if client presend in read sockets             
			if (FD_ISSET(s, &readfds))
			{
				//get details of the client
				getpeername(s, (struct sockaddr*)&address, (int*)&addrlen);

				//Check if it was for closing , and also read the incoming message
				//recv does not place a null terminator at the end of the string (whilst printf %s assumes there is one).
				valread = recv(s, buffer, MAXRECV, 0);

				if (valread == SOCKET_ERROR)
				{
					int error_code = WSAGetLastError();
					if (error_code == WSAECONNRESET)
					{
						//Somebody disconnected , get his details and print
						printf("Host disconnected unexpectedly , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

						//Close the socket and mark as 0 in list for reuse
						closesocket(s);
						client_socket[i] = 0;
					}
					else
					{
						printf("recv failed with error code : %d", error_code);
					}
				}
				if (valread == 0)
				{
					//Somebody disconnected , get his details and print
					printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

					//Close the socket and mark as 0 in list for reuse
					closesocket(s);
					client_socket[i] = 0;
					hashtbl_remove(positions, inttochar(ntohs(address.sin_port)));
				}

				//Echo back the message that came in
				else
				{
					//add null character, if you want to use with printf/puts or other string handling functions
					buffer[valread] = '\0';
					printf("%s:%d - %s \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port), buffer);
					
					for (j = 0; j < max_clients; j++)
					{
						if (j != i) {
							ss = client_socket[j];

							send(ss, buffer, valread, 0);
						}
						else {
							json = cJSON_Parse(buffer);
							if (!json) { printf("Error before: [%s]\n", cJSON_GetErrorPtr()); }
							else
							{
								char* action = cJSON_GetObjectItem(json, "Action")->valuestring;
								if (strcmp(action, "move") == 0) {
									OBJECTUSER* obj_user = (OBJECTUSER*)hashtbl_get(positions, inttochar(ntohs(address.sin_port)));
									double PosX = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "Pos"), "X")->valuedouble;
									double PosY = cJSON_GetObjectItem(cJSON_GetObjectItem(json, "Pos"), "Y")->valuedouble;
									objectuser_setposition(obj_user, PosX, PosY);
									hashtbl_insert(positions, inttochar(ntohs(address.sin_port)), obj_user);
								}
								else if (strcmp(action, "position") == 0) {
									// NOT TODO
									cJSON* root = cJSON_CreateObject();
									OBJECTUSER* obj_user = (OBJECTUSER*)hashtbl_get(positions, inttochar(ntohs(address.sin_port)));
									cJSON_AddItemReferenceToObject(root, "PosX", cJSON_CreateNumber(objectuser_posX(obj_user)));
									cJSON_AddItemReferenceToObject(root, "PosY", cJSON_CreateNumber(objectuser_posY(obj_user)));
									
									char * msg = cJSON_PrintUnformatted(root);

									cJSON_Delete(root);

									if (send(s, msg, strlen(msg), 0) != strlen(msg))
									{
										perror("send failed");
									}

									free(root);
									continue;
								}
								else if (strcmp(action, "fight") == 0) {
									sendFightToAnotherClient(ntohs(address.sin_port), cJSON_GetObjectItem(json, "Id_enemy")->valueint);
									continue;
								}
								else if (strcmp(action, "finishBattle") == 0) {
									sendDiePlayerAndWinnerToShow(j, ntohs(address.sin_port), cJSON_GetObjectItem(json, "Id_enemy")->valueint);
									continue;
								}
								else if (strcmp(action, "exit") == 0) {
									closesocket(s);
									client_socket[i] = 0;
									printf("Cortando\n");

									// Delete socket from positions!!
									hashtbl_remove(positions, inttochar(ntohs(address.sin_port)));
									continue;
								}
								out = cJSON_Print(json);
								cJSON_Delete(json);
								printf("%s\n", out);
								free(out);
							}
						}
					}
				}
			}
		}
	}

	hashtbl_destroy(keys);
	hashtbl_destroy(positions);
	map_destroy(maps[0]);

	closesocket(s);
	WSACleanup();

	printf("Hello World");
	int sss = 0;
	scanf("%d", &sss);
}