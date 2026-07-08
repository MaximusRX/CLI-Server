#include "../include/server_defaults.h"
#include <string.h>
#include <unistd.h>

server_settings settings;
struct sockaddr_in addr;

int IsReady = 0;
socklen_t addr_size = sizeof(addr);
int max_fd = 0;
int selected_fd = 0;
int sock = 0;

void input_handler(char* input_buff){

    memset(input_buff, 0, INPUT_BUFF_SIZE);

    const char delim[] = " ";

	int read_bytes = read(STDIN, input_buff,   INPUT_BUFF_SIZE) -1;

	input_buff[ read_bytes ] = '\0';

    char* token = strtok(input_buff, delim);

    while(token != NULL){

        if(strcmp(token, "exit") == 0){

            exit(0);

        }

        else if (strcmp(token, "help") == 0) {

            help_menu();
            return;
        }

        else if (strcmp(token, "clear") == 0) {

            clear_screen();
            return;
        }

        else if (strcmp(token, "enable") == 0) {

            token = strtok(NULL, delim);

            if(token == NULL){ printf("Error, invalid command\n"); return; }

            else if (strcmp(token, "server-info") == 0) {
                
                settings.show_server_info = true;
                return;

            }

            else if (strcmp(token, "show-connected-clients") == 0) {
                
                settings.show_connected_clients = true;
                return;

            }

            else if (strcmp(token, "show-unavalable-slots") == 0){

                settings.show_unavalable_clients = true;
                return;
            }

            else{

                printf("Error, else hit for strtok, enable secton\n");
                help_menu();
                return;
            }


        }

        else if (strcmp(token, "disable") == 0) {

            token = strtok(NULL, delim);

            if(token == NULL){ printf("Error, invalid command\n"); return; }

            else if (strcmp(token, "server-info") == 0) {
                
                settings.show_server_info = false;
                return;
        
            }

            else if (strcmp(token, "show-connected-clients") == 0) {
                
                settings.show_connected_clients = false;
                return;
            }

            else if (strcmp(token, "show-unavalable-slots") == 0){

                settings.show_unavalable_clients = false;
                return;
            }

            else{

                printf("Error, unknown option paired with enable\n");
                help_menu();
                return;
            }
        }
        
        else if (strcmp(token, "select") == 0) {
            
            token = strtok(NULL, delim);
            if(token == NULL){ printf("Error, invalid command\n"); return; }

            int num = *token - '0';

            if(client[num].IS_ACTIVE){

                selected_fd = client[num].socket;
                return;
            }

            else if (!client[num].IS_ACTIVE) {

                printf("Unavalable client selected\n");
                return;

            }

        }

        else{

            printf("Invalid command: " CYAN "%s\n" RESET_COLOR, token);
            help_menu();
            break;
        }

    }

}

void main_menu(){

    server_status();

    int index;


    if(selected_fd == 0){printf("SELECTED CLIENT AT INDEX: " CYAN "NONE\n"); }
    else{

        for(int i = 0; i < MAX_CLIENTS; i++){

            if(client[i].socket == selected_fd){ index = client[i].index; }
            break;
        }

        printf("SELECTED CLIENT AT INDEX: " CYAN "%d\n", index);

    }

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

    if(settings.show_connected_clients){ printf("CONNECTED CLIENTS: " WHITE "%d\n" RESET_COLOR, active_clients); }
    if(settings.show_unavalable_clients){ printf("UNAVALABLE / EMPTY SLOTS: " CYAN "%d\n" RESET_COLOR, empty_slots); }

}

void set_def_settings(){

    settings.show_connected_clients = true;
    settings.show_unavalable_clients = true;
    settings.show_server_info = true;

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

void client_manager(int index){

    DNP_recv(&client[index]);

}

void help_menu(){

    printf("To get a list of all clients enter:" CYAN "get clients\n" RESET_COLOR);

    printf("To see all the server's settings enter: " CYAN "get settings\n" RESET_COLOR);

    printf("To select a client to communicate with use: " CYAN "select " PURPLE "<client index>\n" RESET_COLOR);

    printf("The command 'enable' can be used to set the following things to active/true: " 
    CYAN "server-info\n" RESET_COLOR);



    printf("The disable command can set the following settings to false/inactive: " CYAN 
    "server-info " RESET_COLOR "," CYAN "show-connected-clients "  RESET_COLOR "," CYAN "show-unavalable-slots\n" RESET_COLOR);


}