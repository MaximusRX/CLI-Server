#include "../include/server_defaults.h"
#include <stdio.h>
#include <stdlib.h>

server_settings settings;
struct sockaddr_in addr;

Token tokens[MAX_TOKENS];
int LastTokIndex = 0;
int LastRedyIndex = 0;
int ready_clients[MAX_CLIENTS];

int IsReady = 0;
socklen_t addr_size = sizeof(addr);
int max_fd = 0;
int selected_fd = -1;
int selected_fd_index = -1;
int sock = 0;
int tempt_num = 0;
bool show_clients = false;

void input_handler(char* input_buff){

    memset(input_buff, 0, INPUT_BUFF_SIZE);
    LastTokIndex = 0;
    set_tokens();

    const char delim[] = " ";

	int read_bytes = read(STDIN, input_buff,   INPUT_BUFF_SIZE) -1;

	input_buff[ read_bytes ] = '\0';

    char* token = strtok(input_buff, delim);

    while(token != NULL){

        if(strcmp(token, "exit") == 0){

            tokens[LastTokIndex].type = TOK_EXIT;
            LastTokIndex++;
            token = strtok(NULL, delim);

        }

        else if (strcmp(token, "help") == 0) {

            tokens[LastTokIndex].type = TOK_HELP;
            LastTokIndex++;
            token = strtok(NULL, delim);
        }

        else if (strcmp(token, "clear") == 0) {

            tokens[LastTokIndex].type = TOK_CLEAR;
            token = strtok(NULL, delim);
        }

        else if (strcmp(token, "enable") == 0) {

            tokens[LastTokIndex].type = TOK_ENABLE;
            LastTokIndex++;
            token = strtok(NULL, delim);

        }

        else if (strcmp(token, "disable") == 0) {

            tokens[LastTokIndex].type = TOK_DISABLE;
            LastTokIndex++;
            token = strtok(NULL, delim);

        }
        
        else if (strcmp(token, "select") == 0) {

            tokens[LastTokIndex].type = TOK_SELECT;
            LastTokIndex++;

            token = strtok(NULL, delim);

            if(token == NULL){printf("Incomplete usage of select\n"); return;}

            tokens[LastTokIndex].type = TOK_INT;
            selected_fd_index = atoi(tokens[LastTokIndex].value);
            strcpy(tokens[LastTokIndex].value, token);

        }

        else if (strcmp(token, "send") == 0) {

            tokens[LastTokIndex].type = TOK_SEND;
            LastTokIndex++;

            char test_delim[] = "|";

            token = strtok(NULL, test_delim);

            strcpy(tokens[LastTokIndex].value, token);
            tokens[LastTokIndex].type = TOK_STRING;
            LastTokIndex++;      

            token = NULL;
        }

        else if (strcmp(token, "show-connected-clients") == 0) {
            tokens[LastTokIndex].type = TOK_SHOW_CONNECTED_CLIENTS;
            LastTokIndex++;
            token = strtok(NULL, delim);
        }   

        else if (strcmp(token, "show-unavalable-slots") == 0) {
            tokens[LastTokIndex].type = TOK_SHOW_UNAVLABLE_SLOTS;
            LastTokIndex++;
            token = strtok(NULL, delim);
        }           

        else if (strcmp(token, "server-info") == 0) {
            tokens[LastTokIndex].type = TOK_SERVER_INFO;
            LastTokIndex++;
            token = strtok(NULL, delim);
        }   

        else if (strcmp(token, "info") == 0) {
            tokens[LastTokIndex].type = TOK_INFO;
            LastTokIndex++;
            token = strtok(NULL, delim);
        } 

        else if (strcmp(token, "show-max-fd") == 0) {
            tokens[LastTokIndex].type = TOK_SHOW_MAX_FD;
            LastTokIndex++;
            token = strtok(NULL, delim);
        }         

        else if (strcmp(token, "show-server-fd") == 0) {
            tokens[LastTokIndex].type = TOK_SHOW_SERVER_FD;
            LastTokIndex++;
            token = strtok(NULL, delim);
        } 

        else if (strcmp(token, "selected") == 0) {
            tokens[LastTokIndex].type = TOK_SELECTED;
            LastTokIndex++;
            token = strtok(NULL, delim);
        } 

        else if (strcmp(token, "status") == 0) {
            tokens[LastTokIndex].type = TOK_STATUS;
            LastTokIndex++;
            token = strtok(NULL, delim);
        } 

        else if (strcmp(token, "get-clients") == 0) {
            tokens[LastTokIndex].type = TOK_SHOW_CLIENTS;
            LastTokIndex++;
            token = strtok(NULL, delim);
        } 

        else{

            tokens[LastTokIndex].type = TOK_UNKNOWN;
            tokens[LastTokIndex].value[strlen(tokens[LastTokIndex].value)] = '\0';
            strcpy(tokens[LastTokIndex].value, token);
            LastTokIndex++;
            token = strtok(NULL, delim);

        }

    }

    switch (tokens[0].type) {

        case TOK_EXIT:
            exit(0);
            break;

        case TOK_HELP:
            help_menu();
            break;

        case TOK_CLEAR:
            clear_screen();
            break;

        case TOK_UNKNOWN:
            printf("Inavlid command: %s\n", tokens[0].value);
            break;

        case TOK_DISABLE:

            if(tokens[1].type == TOK_SHOW_CONNECTED_CLIENTS){
                settings.show_connected_clients = false;
            }

            else if(tokens[1].type == TOK_SERVER_INFO){
                settings.show_server_info = false;
            }

            else if(tokens[1].type == TOK_SHOW_UNAVLABLE_SLOTS){
                settings.show_unavalable_clients = false;
            
            }    
       
            else if(tokens[1].type == TOK_SHOW_MAX_FD){
                settings.show_max_fd = false;
            
            }             

            else if(tokens[1].type == TOK_SHOW_SERVER_FD){
                settings.show_server_fd = false;
            
            } 

            else{

                printf("Incomplete or incorrect usage of disble\n");
                printf("Tok 1: %s\n", tokens[1].value);
                break;

            }

            break;

        case TOK_ENABLE:

            if(tokens[1].type == TOK_SHOW_CONNECTED_CLIENTS){
                settings.show_connected_clients = true; 
            }

            else if(tokens[1].type == TOK_SERVER_INFO){
                settings.show_server_info = true;
                printf("ENABLED\n");
            }

            else if(tokens[1].type == TOK_SHOW_UNAVLABLE_SLOTS){
                settings.show_unavalable_clients = true;
            }    

            else if(tokens[1].type == TOK_SHOW_MAX_FD){
                settings.show_max_fd = true;
            
            }             

            else if(tokens[1].type == TOK_SHOW_SERVER_FD){
                settings.show_server_fd = true;
            
            } 

            else{

                printf("Incomplete or incorrect usage of enable\n");
                printf("Tok 1: %s\n", tokens[1].value);
                break;

            }

            break;

        case TOK_SELECT:

            tempt_num = atoi(tokens[1].value);

            if(tempt_num > 80 || tempt_num < 0){printf("Invalid usage of select\n"); break; }
            if(!client[tempt_num].IS_ACTIVE){printf("Invalid use of select, client is not avalable\n"); selected_fd_index = -1; break;}
            
            selected_fd = client[tempt_num].socket;
            selected_fd_index = client[tempt_num].index;

            break;

        case TOK_SEND:

            DNP_send(&client[selected_fd_index], tokens[1].value);
            printf("Send %s\n", tokens[1].value);

            break;

        case TOK_INFO:

            if(selected_fd != -1){

                printf("RECVD: " CYAN "%s\n" RESET_COLOR,client[selected_fd_index].packet.recv_payload);
                    
                for(int i = 0; i < MAX_CLIENTS; i++){

                    if(client[i].index == ready_clients[LastTokIndex]){

                        LastRedyIndex--;

                    }

                }
            }

            break;

        case TOK_SELECTED:

            if(selected_fd_index == -1){ printf("NO CLIENT SELECTED\n");    }

            else{
                
                printf("SELECTED CLIENT AT INDEX: " CYAN "%d\n", selected_fd_index);
            }

            break;

        case TOK_STATUS:

                settings.show_status = true;

            break;

        case TOK_SHOW_CLIENTS:   

                show_clients = true;

            break;

    }

}

void main_menu(){      
    
    if(settings.show_status){server_status(); settings.show_status = false; }

    for(int i = 0; i < LastRedyIndex; i++){

        printf("Ready clients: " CYAN "%d\n" RESET_COLOR, ready_clients[i]);

    }

    if(show_clients){

        for(int i = 0; i < MAX_CLIENTS; i++){

            if(client[i].IS_ACTIVE){printf("Client avalable at index: %d with fd %d\n", client[i].index, client[i].socket);}

        }

    }

    printf(GREEN "-$ " RESET_COLOR);
    fflush(stdout);

}

void server_status(){

    if(settings.show_server_info){

        printf(BLUE "[ SERVER INFO ]\n" RESET_COLOR);
        printf("\n");

        printf("SERVER VERSION   : " CYAN "%d\n" RESET_COLOR, SERVER_VERSION);
        printf("SERVER ROLL      : " CYAN "%s\n" RESET_COLOR, SERVER_ROLL);
        printf("\n");
    }

    if(settings.show_connected_clients || settings.show_unavalable_clients){
    
        printf(BLUE "[ CLIENT AND SLOT INFO ]\n" RESET_COLOR);
        printf("\n");

        if(settings.show_connected_clients){ printf("CONNECTED CLIENTS              : " CYAN "%d\n" RESET_COLOR, active_clients); }
        if(settings.show_unavalable_clients){ printf(   "UNAVALABLE / EMPTY SLOTS       : " CYAN "%d\n" RESET_COLOR, empty_slots); }

        printf("\n");
    }

    if(settings.show_max_fd || settings.show_server_fd){

        printf(BLUE "[ FILE DESRIPTORS ]\n" RESET_COLOR);
        printf("\n");

        if(settings.show_max_fd){ printf("MAX FD    : " CYAN "%d\n" RESET_COLOR, max_fd);}
        if(settings.show_server_fd){ printf("SERVER FD : " CYAN "%d\n" RESET_COLOR, sock);}

        printf("\n");
    }
}

void set_def_settings(){

    settings.show_connected_clients = true;
    settings.show_unavalable_clients = true;
    settings.show_server_info = true;
    settings.show_max_fd = true;
    settings.show_server_fd = true;
    settings.show_status = false;

}

void shutdown_server(){

    close(sock);
    exit(0);

}

bool net_log_init(){

    FILE* file;

    if(file == NULL){return false; }

    DNP_logger.file = file;

    return true;
}

void help_menu(){

    printf("To get a list of all clients enter: " CYAN "get-clients\n" RESET_COLOR);

    printf("To select a client to communicate with use: " CYAN "select " PURPLE "<client index>\n" RESET_COLOR);

    printf("The command 'enable' can be used to enable the following things: " 
    CYAN "server-info\n" RESET_COLOR "," CYAN "show-connected-clients "  RESET_COLOR "," CYAN "show-unavalable-slots\n" RESET_COLOR);



    printf("The disable command can set the following settings to false/inactive: " CYAN 
    "server-info " RESET_COLOR "," CYAN "show-connected-clients "  RESET_COLOR "," CYAN "show-unavalable-slots\n" RESET_COLOR);

    printf("To exit the program enter: " CYAN "exit\n" RESET_COLOR);
    printf("To get the server status/info enter: " CYAN "status\n" RESET_COLOR);
    printf("To view the selected client's index enter: " CYAN "selected\n" RESET_COLOR);
    printf("To view avalable clients enter: " CYAN "get-clients" RESET_COLOR ", if no client are avalable nothing will be displayed\n");
    printf("To see what a client has send enter: " CYAN "info " RESET_COLOR "Note a valid client must be selected\n");
}

void set_tokens(){

    for(int i = 0; i < MAX_TOKENS; i++){

        tokens[i].type = TOK_EMPTY;
        memset(tokens[i].value, 0, TOKEN_MAX_VAL_SIZE);

    }

}

void disconnect(fd_set *FdSet, int index){

	active_clients--;
	empty_slots++;
    max_fd = 0;

	FD_CLR(client[index].socket, FdSet);

    DNP_init_client(&client[index]);

    for(int i = 0; i < MAX_CLIENTS; i++){


        if(client[i].socket > max_fd && client[i].IS_ACTIVE){

            max_fd = client[i].socket;

            DNP_log("Assigned client at index %d socket num as new max fd\n", "a", i);

        }

    }

    if(sock > max_fd ){

        max_fd = sock;
        DNP_log("Assigned server fd as new max fd\n", "a");
        return;
    }

    DNP_log("Failed to get falid new fd\n", "a");
    DNP_log("Sock size = %d, max_fd = %d\n", "a", sock, max_fd);

}

void print_ascii(){
}