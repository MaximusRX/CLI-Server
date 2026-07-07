#include "include/colors.h"
#include "include/stdtypes.h"
#include "include/server_defaults.h"


int main(){

	fd_set FdSet;
	struct timeval time;

	char input_buff[INPUT_BUFF_SIZE];

	int IsReady = 0;

	int input_index = 0;

	FD_ZERO(&FdSet);
	FD_SET(STDIN, &FdSet);

	set_def_settings();
	main_menu();

	while(true){

		fd_set tempt_set = FdSet;

		time.tv_sec = 1;
		time.tv_usec = 0;

		IsReady = select(STDIN + 1, &tempt_set, NULL, NULL, &time);

		if(IsReady){

			if(FD_ISSET(STDIN, &tempt_set)){

				input_handler(input_buff);
				main_menu();

			}

		}

	}

	return 0;

}
