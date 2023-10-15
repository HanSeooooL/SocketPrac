// echo_client.c
#include "Parking_client.h"

int main(int argc, char *argv[])
{
   init_socket();
   menu_select();
   close_socket();
   return 0;
}