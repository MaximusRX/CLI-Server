#include "DNP.h"
#include <stdarg.h>

DNP_Logger DNP_logger;
DNP_CLIENT client[MAX_CLIENTS];

int active_clients = 0;
int empty_slots = 0;


void DNP_set_struct(int family,int port, const char* ip, struct sockaddr_in *addr){

    addr->sin_family = family;
    addr->sin_port = htons(port);

    if(ip == NULL){addr->sin_addr.s_addr = INADDR_ANY; }
    else { inet_pton(family, ip, &addr->sin_addr.s_addr);}
    
}

int DNP_SETUP_SERVER(struct sockaddr_in *addr){

    addr->sin_family = FAMILY;
    addr->sin_port = htons(PORT);
    addr->sin_addr.s_addr = SERVER_IP;
    int sock = DNP_socket();

    if(sock <= 0){ return -1; }
    return sock;

}

int DNP_send(DNP_CLIENT *client, char* buff){

    if(client->socket <= 0){ printf("SOCKET ERROR <= 0, DNP_SEND, socket not valid\n"); return 1;}

    DNP_craft_header(client, buff);
    DNP_send_hedder(client);

    int send_bytes = 0;

    while(send_bytes != client->header.send_payload_size){
        
        int bytes = send(client->socket, client->packet.send_payload + send_bytes, client->header.send_payload_size - send_bytes, 0);

        if(bytes <= 0){ return -1; }

        send_bytes += bytes;   

    }

    return send_bytes;

}

void DNP_send_hedder(DNP_CLIENT *client){

    int send_bytes = 0;

    uint32_t headder_size = htonl ( client->header.send_payload_size);
    uint32_t payload_type = htonl (client->header.send_payload_type );

    char* size_ptr = (char*) &headder_size;
    char* type_ptr = (char*) & payload_type;

    while(send_bytes < 4){
        
        int bytes = send(client->socket, size_ptr + send_bytes, 4 - send_bytes, 0);

        if(bytes <= 0){ return; }

        send_bytes += bytes;   

    }

    send_bytes = 0;

    while(send_bytes < 4){
        
        int bytes = send(client->socket, type_ptr + send_bytes, 4 - send_bytes, 0);

        if(bytes <= 0){ return; }

        send_bytes += bytes;   

    }

}

int DNP_get_headder(DNP_CLIENT *client){

    int recvd_bytes = 0;
    uint32_t buff[2];

    char* ptr = (char*)buff;

    while(recvd_bytes < 8){
        
        int bytes = recv(client->socket, ptr + recvd_bytes, 8 - recvd_bytes, 0);

        if (bytes == 0){ return 0; }
        if(bytes == -1){ return -1; }

        recvd_bytes += bytes;   

    }

    client->header.recv_payload_size = ntohl( buff[0] );
    client->header.recv_payload_type = ntohl ( buff[1] );

    if(client->header.recv_payload_size >  5242880 ){ printf("RECV'd A INVALID SIZE, CLOSING CONNECTION\n"); close(client->socket);}

    else if(client->packet.recv_payload != NULL){
        
        free(client->packet.recv_payload); client->packet.recv_payload = NULL;
        
        printf("RECV Buff was free'd and SET to NULL\n");
        
        client->packet.recv_payload = malloc(client->header.recv_payload_size + 1);
        client->packet.recv_payload[client->header.recv_payload_size] = '\0';

        printf("MALLOCED\n");
    }



    else { client->packet.recv_payload = malloc(client->header.recv_payload_size + 1);
            client->packet.recv_payload[client->header.recv_payload_size] = '\0';

            printf("MALLOCED\n");
    }

    return recvd_bytes;

}

int DNP_recv(DNP_CLIENT *client){

    if(client->socket <= 0){ printf("SOCKET ERROR <= 0,DNP_recv, socket not valid\n"); return -1;}

    int header_status = DNP_get_headder(client);

    if (header_status == 0){ return 0; }
    if(header_status == -1){ return -1; }

    int recvd_bytes = 0;

    while(recvd_bytes < client->header.recv_payload_size){
        
        int bytes = recv(client->socket, client->packet.recv_payload + recvd_bytes, client->header.recv_payload_size - recvd_bytes, 0);
        
        if (bytes == 0){ return 0; }
        if(bytes == -1){ return -1; }

        recvd_bytes += bytes;   

    }

    return recvd_bytes;

}

int DNP_socket(){

    int sock_main = socket(FAMILY, SOCK_STREAM, 0);

    if(sock_main == -1){return -1; }

    return sock_main;
}

int DNP_init_client(DNP_CLIENT *client){

    client->IS_ACTIVE = false;
    client->socket = 0;

    client->packet.recv_payload = NULL;
    client->packet.send_payload = NULL;

    client->header.recv_payload_size = 0;
    client->header.send_payload_size = 0;
    client->header.recv_payload_type = 999;
    client->header.send_payload_type = 999;

    return 0;

}

int DNP_assing_client(DNP_CLIENT *client,int client_fd){

    for(int i = 0; i < MAX_CLIENTS; i++){

        if(!client[i].IS_ACTIVE){

            client[i].socket = client_fd;
            client[i].IS_ACTIVE = true;
            return i;

        }

    }

    return -1;

}

int DNP_craft_header(DNP_CLIENT *client,char* buff){

    int temp_size = strlen(buff);
    char tempt[temp_size +1];

    strcpy(tempt, buff);

    char* token_1 = strchr(tempt, ' ');

    if(token_1 == NULL){ printf("Failed crafting header payload\n"); return 1; }

    *token_1 = '\0';

if(client->packet.send_payload != NULL){
    free(client->packet.send_payload);
    client->packet.send_payload = NULL;
}

    client->packet.send_payload = strdup(token_1 + 1);
    client->header.send_payload_size = strlen(client->packet.send_payload);

    if(strcmp(tempt, "BASH") == 0){ client->header.send_payload_type = BASH; }
    else if(strcmp(tempt, "DEF") == 0){ client->header.send_payload_type = DEFAULT_COMMAND; }
    else if(strcmp(tempt, "MSG") == 0){ client->header.send_payload_type = MESSAGE; }
    else if(strcmp(tempt, "INFORM") == 0){ client->header.send_payload_type = INFORM; }

    else{

        client->header.send_payload_type = UNKNOWN;
        return 1;

    };

    return 0;
}

void DNP_get_client_info(DNP_CLIENT *client){

    printf("CLIENT SOCKET: %d\n", client->socket);
    printf("Last Send Payload Size: %d\n", client->header.send_payload_size);
    printf("Last Recv'd Payload Size: %d\n", client->header.recv_payload_size);

    DNP_get_cmd_type(client);

    printf("Recv'd payload: %s\n", client->packet.recv_payload);
    printf("Send Payload: %s\n", client->packet.send_payload);

}

void DNP_get_cmd_type(DNP_CLIENT *client){

    printf("RECV TYPE NUMBER: %d\n",client->header.recv_payload_type);
    printf("SEND TYPE NUMBER: %d\n",client->header.send_payload_type);

    switch(client->header.recv_payload_type){

        case DEFAULT_COMMAND:
            printf("RECV COMMAD TYPE: DEFAULT\n");
            break;

        case BASH:
            printf("RECV COMMAND TYPE: BASH\n");
            break;

        case MESSAGE:
            printf("RECV COMMAND TYPE: MESSAGE\n");
            break;

        case INFORM:
            printf("RECV COMMAND TYPE: INFORM\n");
            break;

        default: 
            printf("RECV COMMAND TYPE: UNKNOWN\n");
            break;
    }

    switch(client->header.send_payload_type){

        case DEFAULT_COMMAND:
            printf("SEND COMMAD TYPE: DEFAULT\n");
            break;

        case BASH:
            printf("SEND COMMAND TYPE: BASH\n");
            break;

        case MESSAGE:
            printf("SEND COMMAND TYPE: MESSAGE\n");
            break;

        case INFORM:
            printf("SEND COMMAND TYPE: INFORM\n");
            break;

        default: 
            printf("SEND COMMAND TYPE: UNKNOWN\n");
            break;
    }

}


int DNP_log(const char* string_msg, const char* mode,...){

    va_list args;

    va_start(args, mode);


    DNP_logger.file = fopen("DNP_LOG.txt", mode);

    if(DNP_logger.file == NULL){ return -1; }

    vfprintf(DNP_logger.file, string_msg, args);
    fputs("\n", DNP_logger.file);

    fclose(DNP_logger.file);

    va_end(args);
    return 0;
}

int DNP_init_client_arr(){

    for(int i = 0; i < MAX_CLIENTS; i++){

       DNP_init_client(&client[i]);
       client[i].index = i;

    }

    return 0;

}

int DNP_get_valid_slot(){

	for(int i = 0; i < MAX_CLIENTS; i++){

		if(!client->IS_ACTIVE){

			DNP_log(  "[DNP LOG] found a valid slot at index %d", "a", i);
			return i;

		}

	}
    DNP_log(  "[DNP LOG] Failed to get valid a slot", "a");
    return -1;
}

void DNP_get_client_stats(){

    active_clients = 0;
    empty_slots = 0;

    for(int i = 0; i < MAX_CLIENTS; i++){

        if(client[i].IS_ACTIVE){

            active_clients++;

        }

        else{

            empty_slots++;

        }

    }

}