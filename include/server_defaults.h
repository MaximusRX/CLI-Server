#include "stdtypes.h"
#include "colors.h"

typedef struct{

    bool show_connected_clients;
    bool show_unavalable_clients;
    bool show_server_info;

} server_settings;


void input_handler(char* input_buff);
void main_menu();
void server_status();
void set_def_settings();
void shutdown_server();
void server_info();