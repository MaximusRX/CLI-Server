#include "stdtypes.h"
#include "colors.h"

typedef struct{

    bool show_connected;
    bool show_unavalable;

} server_settings;


void input_handler(char* input_buff);
void main_menu();
void server_info();
void set_def_settings();
void shutdown_server();