#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <bits/types/struct_timeval.h>
#include <sys/select.h>
#include <stdlib.h>

#pragma once

#define INPUT_BUFF_SIZE 21
#define SERVER_VERSION 1
#define SERVER_ROLL "OVERLORD"
#define STDIN 0

extern char input_buff[];
