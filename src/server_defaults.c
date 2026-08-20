#include "../include/server_defaults.h"
#include <stdio.h>
#include <sys/socket.h>

server_settings settings;
struct sockaddr_in addr;

Token tokens[MAX_TOKENS];
int LastTokIndex = 0;
int LastRedyIndex = 0;
int ready_clients[MAX_CLIENTS];

int IsReady = 0;
int token_count = 0;
socklen_t addr_size = sizeof(addr);
int max_fd = 0;
int selected_fd = -1;
int selected_fd_index = -1;
int sock = 0;
int tempt_num = 0;

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
            token = strtok(NULL, delim);
            LastTokIndex++;
        }

        else if (strcmp(token, "show-clients") == 0) {

            tokens[LastTokIndex].type = TOK_SHOW_CLIENTS;
            token = strtok(NULL, delim);
            LastTokIndex++;
        }

        else if (strcmp(token, "clear") == 0) {

            tokens[LastTokIndex].type = TOK_CLEAR;
            token = strtok(NULL, delim);
            LastTokIndex++;
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
            tokens[LastTokIndex].valueint = atoi (token);
            LastTokIndex++;

        }

        else if (strcmp(token, "send") == 0) {

            tokens[LastTokIndex].type = TOK_SEND;
            LastTokIndex++;

            token = strtok(NULL, delim);

            tokens[LastTokIndex].type = TOK_STRING;
            strcpy(tokens[LastTokIndex].valuestr, token);
            LastTokIndex++;      

            token = NULL;
        }

        else if (strcmp(token, "download") == 0) {

            tokens[LastTokIndex].type = TOK_DOWNLOAD;
            LastTokIndex++;

            token = strtok(NULL, delim);

            if(token == NULL){break; }

            tokens[LastTokIndex].type = TOK_STRING;
            strcpy(tokens[LastTokIndex].valuestr, token);
            LastTokIndex++;      

            token = NULL;
        }

        else if (strcmp(token, "upload") == 0) {

            tokens[LastTokIndex].type = TOK_UPLOAD;
            LastTokIndex++;

            token = strtok(NULL, delim);
            
            if(token == NULL){break; }

            strcpy(tokens[LastTokIndex].valuestr, token);
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
            tokens[LastTokIndex].type = TOK_GET_CLIENTS;
            LastTokIndex++;
            token = strtok(NULL, delim);
        } 

        else{

            tokens[LastTokIndex].type = TOK_UNKNOWN;
            tokens[LastTokIndex].valuestr[strlen(tokens[LastTokIndex].valuestr)] = '\0';
            strcpy(tokens[LastTokIndex].valuestr, token);
            LastTokIndex++;
            token = strtok(NULL, delim);

        }

    }

    switch (tokens[0].type) {

        case TOK_EXIT:
            shutdown_server();
            break;

        case TOK_HELP:
            help_menu();
            break;

        case TOK_CLEAR:
            clear_screen();
            break;

        case TOK_GET_CLIENTS:

            for(int i = 0; i < LastRedyIndex; i++){

                if(client[i].IS_ACTIVE){printf("Ready client: " CYAN "%d\n" RESET_COLOR, ready_clients[i]); }

            }

            for(int i = 0; i < MAX_CLIENTS; i++){

                if(client[i].IS_ACTIVE){printf("Active client at index: "  CYAN" %d\n" RESET_COLOR, i); }

            }

            break;

        case TOK_UNKNOWN:
            printf("Inavlid command: %s\n", tokens[0].valuestr);
            break;

        case TOK_DISABLE:

            switch(tokens[1].type){

                case TOK_SHOW_CLIENTS:

                    settings.show_clients = false;
                    break;
                
                case TOK_SHOW_CONNECTED_CLIENTS:
                    settings.show_connected_clients = false;
                    break; 

                case TOK_SERVER_INFO:
                    settings.show_server_info = false;
                    break;

                case TOK_SHOW_UNAVLABLE_SLOTS:
                    settings.show_unavalable_clients = false;
                    break;

                case TOK_SHOW_MAX_FD:
                    settings.show_max_fd = false;
                    break;

                case TOK_SHOW_SERVER_FD:
                    settings.show_server_fd = false;
                    break;

                default:
                    printf("Incomplete or incorrect usage of disble\n");
                    printf("Tok 1: %s\n", tokens[1].valuestr);
                    break;
            }   

            break;               
        

        case TOK_ENABLE:

            switch(tokens[1].type){

                case TOK_SHOW_CLIENTS:

                    settings.show_clients = true;
                    break;


                case TOK_SHOW_CONNECTED_CLIENTS:
                    settings.show_connected_clients = true;
                    break; 

                case TOK_SERVER_INFO:
                    settings.show_server_info = true;
                    break;

                case TOK_SHOW_UNAVLABLE_SLOTS:
                    settings.show_unavalable_clients = true;
                    break;

                case TOK_SHOW_MAX_FD:
                    settings.show_max_fd = true;
                    break;

                case TOK_SHOW_SERVER_FD:
                    settings.show_server_fd = true;
                    break;

                default:
                    printf("Incomplete or incorrect usage of enable\n");
                    printf("Tok 1: %s\n", tokens[1].valuestr);
                    break;
            }   
            
            break;   

        case TOK_SELECT:    

            if(tokens[1].valueint > 80 || tokens[1].valueint < 0){printf("Invalid client index given\n"); }

            if(!client[tokens[1].valueint].IS_ACTIVE){printf("Invalid use of select, client is not avalable\n"); selected_fd_index = -1; break;}
            
            selected_fd = client[tokens[1].valueint].socket;
            selected_fd_index = client[tokens[1].valueint].index;

            break;

        case TOK_SEND:

            DNP_message(&client[selected_fd_index], tokens[1].valuestr, DEFAULT_COMMAND);
            printf("Send %s\n", tokens[1].valuestr);

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

        case TOK_UPLOAD:
            if(tokens[1].type == TOK_EMPTY){printf("Provide file to upload...\n"); }
            upload(tokens[1].valuestr);

            break;

        case TOK_DOWNLOAD:
            if(tokens[1].type == TOK_EMPTY){printf("Provide file to download...\n"); }
            download(tokens[1].valuestr);
            break;

        case TOK_SELECTED:

            if(selected_fd_index == -1){ printf("NO CLIENT SELECTED\n");    }

            else{
                
                printf("SELECTED CLIENT AT INDEX: " CYAN "%d\n" RESET_COLOR, selected_fd_index);
            }

            break;

        case TOK_STATUS:

                settings.show_status = true;

            break;

        case TOK_SHOW_CLIENTS:   

                

            break;

    }

}

void main_menu(){      
    
    if(settings.show_status){server_status(); settings.show_status = false; }

    if(settings.show_clients){

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
    settings.show_clients = true;

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
        memset(tokens[i].valuestr, 0, TOKEN_MAX_VAL_SIZE);
        tokens[1].valueint = -1;

    }

}

void disconnect(fd_set *FdSet, int index){

	active_clients--;
	empty_slots++;
    max_fd = sock;

	FD_CLR(client[index].socket, FdSet);

    DNP_init_client(&client[index]);

    for(int i = 0; i < MAX_CLIENTS; i++){

        if(client[i].socket > max_fd && client[i].IS_ACTIVE){

            max_fd = client[i].socket;

            DNP_log("Assigned client at index %d socket num as new max fd\n", "a", i);

        }

    }

    DNP_log("Server socket = %d, max_fd = %d\n", "a", sock, max_fd);

}

void upload(char* filename){

    char buff[100];
    char msg[MAX_PAYLOAD_SIZE];

    FILE*

    file = fopen(filename, "r");

    if(file == NULL){ printf("Error, unable to open file, make sure the filename is correct or the file exists\n"); strcpy(msg, "Error, unabke to open file"); return;}
            
    while( fgets(buff, 100, file) != NULL){

        strcat(msg, buff);

    }



    DNP_message(&client[selected_fd_index], msg, UPLOAD);

}

void download(char* filename){

    FILE* file = fopen(filename, "w");

    DNP_message(&client[selected_fd_index], filename, DOWNLOAD);

    DNP_recv(&client[selected_fd_index]);

    fputs(client[selected_fd_index].packet.recv_payload, file);

}