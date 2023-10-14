// echo_client.c
#include "Parking_client.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <fcntl.h>

#define BUF_SIZE 1024   //버퍼사이즈

int main(int argc, char *argv[])
{
   init_socket();
   menu_select();
   close_socket();
   return 0;
}