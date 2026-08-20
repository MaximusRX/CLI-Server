#include "include/colors.h"
#include "include/stdtypes.h"
#include "include/server_defaults.h"
#include "net/DNP.h"
#include <asm-generic/socket.h>
#include <sys/select.h>
#include <sys/socket.h>


int main(){

	int FORCE_BIND_VAL = 1;

	clear_screen();

	sock = DNP_SETUP_SERVER(&addr);

	fd_set FdSet;
	struct timeval time;

	char input_buff[INPUT_BUFF_SIZE];

	max_fd = sock;

	if(!net_log_init() ){ return -1;}

	DNP_log("Logger Set Up...", "a");

	if(sock == -1){DNP_log("Failed To Get Valid FD\n", "a"); return -1;}

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &FORCE_BIND_VAL, sizeof(FORCE_BIND_VAL));

	int binder = bind(sock, (struct sockaddr*) &addr, addr_size);

	if(binder == -1){DNP_log("Server Failed to bind", "a"); exit(1);}
	else if(binder == 0){DNP_log("SERVER SUCESFULLY BINDED", "a"); }

	int listener = listen(sock, MAX_CLIENTS);

	if(listener == -1){DNP_log("[DNP LOG] FAILED TO LISTEN FOR CONNECTIONS", "a"); exit(1); }
	else if(listener == 0){DNP_log("[DNP LOG] LISTENING FOR CONNECTIONS", "a"); }

	DNP_init_client_arr();
	DNP_get_client_stats();

	FD_ZERO(&FdSet);
	FD_SET(STDIN,&FdSet);
	FD_SET(sock, &FdSet);

	set_def_settings();
	main_menu();

	if(sock == -1){ DNP_log("DAILED TO SETUP SERVER, SOCK == -1\n", "a") ;return -1;}


	while(true){


		fd_set tempt_set = FdSet;

		time.tv_sec = 1;
		time.tv_usec = 0;

		IsReady = select(max_fd + 1, &tempt_set, NULL, NULL, &time);

		if(IsReady){

			if(FD_ISSET(STDIN, &tempt_set)){
				clear_screen();
				input_handler(input_buff);
				main_menu();

			}

			else if(FD_ISSET(sock, &tempt_set)){

				int accepted_fd = accept(sock, (struct sockaddr* ) &addr, &addr_size);

				if(accepted_fd > 0){

					if(accepted_fd > max_fd){

						max_fd = accepted_fd;
						DNP_log("Max fd = an accepted conenction\n", "a");

					}

					int status = DNP_get_valid_slot();

					if(status == -1){ DNP_log("Failed to get falid index for client", "a"); close(accepted_fd); }

					else{
						
						DNP_assing_client(&client[status], accepted_fd);

						FD_SET(accepted_fd, &FdSet);
						active_clients++;
						empty_slots--;

						DNP_log("Client connected with FD %d and was assigned slot %d\n", "a", accepted_fd, status);
					}
				}

			}

			else{

				for(int i = 0; i < MAX_CLIENTS; i++){

					if(FD_ISSET(client[i].socket, &tempt_set)){

						int status = DNP_recv(&client[i]);

						bool is_in_arr = false;

						for(int i = 0; i > LastRedyIndex; i++)
						{

							if(client[i].index == ready_clients[i]){is_in_arr = true; }

						}

						if(!is_in_arr) { ready_clients[LastRedyIndex] = client[i].index; }

						if(status == 0 || status == -1){

							disconnect(&FdSet, i);

						}

						else{

							LastRedyIndex++;
						}

					}

				}

			}

		}

	}

	return 0;

}