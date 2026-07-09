#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <bits/types/struct_timeval.h>
#include <sys/select.h>
#include <stdlib.h>
#include <sys/socket.h>

#pragma once

#define INPUT_BUFF_SIZE 51
#define SERVER_VERSION 1
#define SERVER_ROLL "OVERLORD"
#define STDIN 0
#define MAX_TOKENS 20

extern char input_buff[];
