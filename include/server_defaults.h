#include "stdtypes.h"
#include "colors.h"
#include "../net/DNP.h"
#include <netinet/in.h>
#include <unistd.h>

#define TOKEN_MAX_VAL_SIZE 12

typedef struct{

    bool show_connected_clients;
    bool show_unavalable_clients;
    bool show_server_info;
    bool show_server_fd;
    bool show_max_fd;

} server_settings;

enum{

    TOK_ENABLE, TOK_DISABLE, TOK_SEND, TOK_INFO, TOK_INT, TOK_STRING, TOK_EXIT, TOK_HELP, TOK_CLEAR, 
    TOK_SELECT, TOK_UNKNOWN, TOK_SHOW_CONNECTED_CLIENTS, TOK_SHOW_UNAVLABLE_SLOTS, TOK_SHOW_INFO,
    TOK_SERVER_INFO, TOK_EMPTY, TOK_SHOW_MAX_FD, TOK_SHOW_SERVER_FD

};

typedef struct{

    int type;
    char value[TOKEN_MAX_VAL_SIZE];

} Token;


void input_handler(char* input_buff);
void main_menu(int* sock);
void server_status();
void set_def_settings();
void shutdown_server();
void server_info();
bool net_log_init();
void help_menu();
void set_tokens();
void disconnect(fd_set *fd, int index);

extern struct sockaddr_in addr;
extern DNP_CLIENT client[];

extern Token tokens[];
extern int ready_clients[MAX_CLIENTS];

extern int IsReady;
extern int LastRedyIndex;
extern int LastTokIndex;
extern int max_fd;
extern socklen_t addr_size;
extern int selected_fd;
extern int sock;
