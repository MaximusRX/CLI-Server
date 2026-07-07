#include "../include/server_defaults.h"
#include <stdbool.h>
#include <sys/socket.h>

server_settings settings;

void input_handler(char* input_buff){


	int read_bytes = read(STDIN, input_buff,   INPUT_BUFF_SIZE) -1;

	input_buff[ read_bytes ] = '\0';

    if(strcmp(input_buff, "get server info") == 0){ server_info(); }
    else if(strcmp(input_buff, "exit") == 0){ shutdown_server(); }
    else if(strcmp(input_buff, "enable server info") == 0){ settings.show_server_info = true; }
    else if(strcmp(input_buff, "disable server info") == 0){ settings.show_server_info = false; }
    else{

        printf(RED "[ERROR]: Unknown command...\n" RESET_COLOR);

    }

}

void main_menu(){

    server_status();

    printf(GREEN "-$ " RESET_COLOR);
    fflush(stdout);

}

void server_info(){
    
    printf("SERVER VERSION: " CYAN "%d\n" RESET_COLOR, SERVER_VERSION); 
    printf("SERVER ROLL: " CYAN "%s\n" RESET_COLOR, SERVER_ROLL); 

}

void server_status(){

    if (settings.show_server_info) { printf("SERVER VERSION: " CYAN "%d\n" RESET_COLOR, SERVER_VERSION); }
    if (settings.show_server_info) { printf("SERVER ROLL: " CYAN "%s\n" RESET_COLOR, SERVER_ROLL); }

    if(settings.show_connected_clients){ printf("CONNECTED CLIENTS: " WHITE "UNAVALABLE FATURE\n" RESET_COLOR); }
    if(settings.show_unavalable_clients){ printf("UNAVALABLE CLIENTS: " WHITE "UNAVALABLE FATURE\n" RESET_COLOR); }

}

void set_def_settings(){

    settings.show_connected_clients = true;
    settings.show_unavalable_clients = true;
    settings.show_server_info = true;

}

void shutdown_server(){

    exit(0);

}
