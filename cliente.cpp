#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
 
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <thread> 
#include <mutex>
 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue> 

#include "Packet.h"

using namespace std;

int sockfd;
bool conectado = false;
struct  sockaddr_in direc;




void conectar(string ipServidor, int puerto){
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "error al crear el socket"<<endl; 
        return;    
    }

    direc.sin_family = AF_INET;
    direc.sin_port = htons(puerto);
    inet_pton(AF_INET, ipServidor.c_str(), &direc.sin_addr);

    if (connect(sockfd,(struct sockaddr *)&direc, sizeof(direc)) == -1 ){
        cout << "conexion al servidor fallo" << endl;
        return;
    }
    conectado = true;

    cout << "coneccion exitosa" << endl;   
};




void desconectar(){
    if (!conectado)
        return;
    conectado = false;

    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);
    cout << "Ya esta desconectado;"<< endl;
};




int main(int argc, char **argv) {   
    if ( argc < 3 ){
        cout<<"falta argumentos"<<endl;
        return -1;
    }
    string opcion = argv[1];    

    if (opcion == "-q"){
        conectar("127.0.0.1",8000);

        desconectar();
    }
   
    return 0;
};

 