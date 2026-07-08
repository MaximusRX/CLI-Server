

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

#pragma once

//Protocol Version
#define VERSION 2

/*
    Default network Properties.
*/

#define MAX_CLIENTS 80
#define PORT 6767
#define FAMILY AF_INET
#define EXTERN_IP "192.168.10.211"
#define SERVER_IP INADDR_ANY
#define ADDR INADDR_ANY;

extern int active_clients;
extern int empty_slots;

/*
    The types of paylods to send and recv.
*/
enum COMMAND_TYPES{

    DEFAULT_COMMAND, BASH, MESSAGE, INFORM, UNKNOWN = 999

};

/*
    the default packet/payload the user uses.
*/
typedef struct{

    char* recv_payload;
    char* send_payload;

} DNP_PACKET;

/*
    default headder contai ing payload size and type
*/
typedef struct{

    unsigned int send_payload_size;
    unsigned int recv_payload_size;

    int recv_payload_type;
    int send_payload_type;

} DNP_HEADER;
/*
    The client struct represents a client, it will be used to interact with clients
*/
typedef struct{

    int socket;
    bool IS_ACTIVE;
    int index;

    DNP_PACKET packet;
    DNP_HEADER header;

} DNP_CLIENT;

extern DNP_CLIENT client[];

/*
    This is the logger struct, used to log events.
*/

typedef struct{

    FILE* file;

} DNP_Logger;

extern DNP_Logger DNP_logger;

/*
    DNP_recv takes cliet fd as its first arg, it whill then recv packet.payload_size 
    amount of data and store it in packet.payload.
*/
int DNP_recv(DNP_CLIENT *client);

/*
    DNP_send takes a client fd as its arg which it will then use to send data from 
    packet.payload and of size DNP_PACKET.payload_size to the client fd that was
    passed as thearg.
*/
int DNP_send(DNP_CLIENT *client, char* buff);

/*
    DNP_get_headder takes client fd arg which it will then recv a headder containing
    the size of the incomming payload to alloc and the payload type, returns -1 on error.
*/
int DNP_get_headder(DNP_CLIENT *client);

/*
    DNP_send_headder takes a client fd arg which it will then use to send a headder
    contaning the payload size and type the user wants to send, returns 1 on error.
*/
void DNP_send_hedder(DNP_CLIENT *client);

/*
    DNP_set_structure sets the family,port and IP which will be used to send data 
    to the given ip using the given port and family.
*/
void DNP_set_struct(int family, int port, const char* ip, struct sockaddr_in *addr);

/*
    This function returns the default socket for DNP.
*/
int DNP_socket();

/*
    This function iterates over the client array and when a unused index is found 
    it ill have a new clients fd assingsed and will be marked as in use.
*/
int DNP_assing_client(DNP_CLIENT *client,int client_fd);

/*
    This function sets all clients index's to inactive. Reason: when a client
    connects it needs a slot, if the array is not initilized it would not be
    possable to assing it to a slot/index
*/
int DNP_init_client(DNP_CLIENT *client);

/*
    This function splits a command into 2x parts, the command type and the payload, 
    it will then get the paylod size and put it all into a headdr, then send that
    headder containing paylod size and type to a client or server for communications 
*/
int DNP_craft_header(DNP_CLIENT *client,char* buff);

/*
    This function prints out data of the given client, it will print the fd, payload size,
    and send payload.
*/
void DNP_get_client_info(DNP_CLIENT *client);

/*
    This function takes a int value then prints the ENUM it is related to
*/
void DNP_get_cmd_type(DNP_CLIENT *client);

/*
    This function log's events to the log file.
*/

int DNP_log(const char* string_msg, const char* mode,...);

/*
    This function takes a addr, it sets the addr and returns a socket.
*/

int DNP_SETUP_SERVER(struct sockaddr_in *addr);

/*
    This function inits the client array, inits MAX_CLIENTS amount of clints.
*/

int DNP_init_client_arr();

/*
    This function tries to find a valid slot in the client array, if it fails it returns -1, else returns 
    valid slot.
*/

int DNP_get_valid_slot();

/*
    This function iterates through the client array to find how many active,inactive and unasigned slots and
    clients there are.
*/

void DNP_get_client_stats();