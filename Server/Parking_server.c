//TCP방식 소켓통신
#include "Parking_server.h"
#define PARKINGCARINFOROUTE "Parkinginfo.bin"
#define COMMUTERCARROUTE "CommuterCar.bin"
#define COMMUTERINFOROUTE "Commuterinfo.txt"
#define SALESROUTE "Sales.txt"
#define FEEOPTIONROUTE "Feeoptioninfo.txt"
#define PLACEINFOROUTE "Placeinfo.txt"



int main()
{
   void* dataptr;

   Parkcar tmp;
   for(int i = 0; i < 5; i++) {
      strcpy(tmp.carnumber, "12가3456");
      strcpy(tmp.phonenumber, "010-2173-4577");
      strcpy(tmp.intime, "1234567890");
      savethecar(tmp);
   }
   

   dataptr = readFile(PARKINGCARINFOROUTE, 0, 5);
   
   for(int i = 0; i < 5; i++) {
      printf("carnumber: %s\n" , ((Parkcar*)dataptr)[i].carnumber);
      printf("phonenumber: %s\n" , ((Parkcar*)dataptr)[i].phonenumber);
      printf("intime: %s\n" , ((Parkcar*)dataptr)[i].intime);
   }


   
   
   //init_socket();
   //ListenFromtheClient();

   


   /*
   for(i = 0; i < 5; i++)
   {
      //accept함수로 클라이언트의 접속 요청 허락, 커널이 소켓 생성(클라이언트 소켓 생성)
      clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
      if(clnt_sock == -1)
         error_handling("accept() error");
      else
         printf("Connected client %d \n", i + 1);
      //read(읽어들이는 위치(클라이언트 소켓)), 저장 위치, 버퍼 사이즈)
      //데이터의 길이를 반환
      while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) {
         //write(전송위치(디스크립터), 전송내용, 길이);
         write(clnt_sock, message, str_len);
         message[str_len] = '\0';
         savethedata(message);
      }

      close(clnt_sock);
   }
   */
   return 0;

}