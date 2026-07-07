#include "../include/server_defaults.h"
#include <sys/socket.h>

server_settings settings;

void input_handler(char* input_buff){


	int read_bytes = read(STDIN, input_buff,   INPUT_BUFF_SIZE) -1;

	input_buff[ read_bytes ] = '\0';

    if(strcmp(input_buff, "GET SERVER INFO") == 0){ server_info(); }
    else if(strcmp(input_buff, "exit") == 0){ shutdown_server(); }
    else{

        printf(RED "[ERROR]: Unknown command...\n" RESET_COLOR);

    }

}

void main_menu(){

    printf(GREEN "-$ " RESET_COLOR);
    fflush(stdout);

}

void server_info(){

    printf("SERVER VERSION: " CYAN "%d\n" RESET_COLOR, SERVER_VERSION);
    printf("SERVER ROLL: " CYAN "%s\n" RESET_COLOR, SERVER_ROLL);

    if(settings.show_connected){ printf("CONNECTED CLIENTS: " WHITE "UNAVALABLE FATURE\n" RESET_COLOR); }
    if(settings.show_unavalable){ printf("UNAVALABLE CLIENTS: " WHITE "UNAVALABLE FATURE\n" RESET_COLOR); }

}

void set_def_settings(){

    settings.show_connected = true;
    settings.show_unavalable = true;

}

void shutdown_server(){

    exit(0);

}
