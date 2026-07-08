#include "stdtypes.h"
#include "colors.h"
#include "../net/DNP.h"
#include <netinet/in.h>
#include <unistd.h>

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
bool net_log_init();
void client_manager(int index);
void help_menu();

extern struct sockaddr_in addr;
extern DNP_CLIENT client[];
extern int IsReady;
extern int max_fd;
extern socklen_t addr_size;
extern int selected_fd;
extern int sock;