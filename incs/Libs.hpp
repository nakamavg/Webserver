#pragma once
#include <algorithm>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include "ServerUp.hpp"
#include <vector>
#include <map>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#define END_LINE "\r\n"
#define READ 0
#define WRITE 1
#define MAX_EVENTS 64
#define MAX_REQUEST_SIZE 1024

std::string ft_read(std::string filename);