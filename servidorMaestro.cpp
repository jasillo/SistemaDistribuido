#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
 
#include <netdb.h>
#include <unistd.h>
#include <thread>
 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <mutex> 

#include "Packet.h"

using namespace std;



 
int sockfd;
struct sockaddr_in serv_addr;
bool conectado = false;
mutex mtxScreen;
vector<int> clientesFds;




void conectar(int puerto){     
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
       cout << "error al crear el socket" << endl;
       return;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(puerto);
    serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
    memset(&(serv_addr.sin_zero), 0, 8);
 
    if ( bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)  
        cout << "error al asignar puerto" << endl;
 
    if ( listen(sockfd, 10) == -1)        
        cout << "error en liste" << endl;    

    conectado = true;
};




void desconectar(){
    if (!conectado)
        return;
    conectado = false;

    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);

    mtxScreen.lock();
    for (int i=0; i<clientesFds.size(); ++i){
        shutdown(clientesFds[i], SHUT_RDWR);
        close(clientesFds[i]);
    }
    mtxScreen.unlock();

    cout << "Ya esta desconectado;" << endl;
};




void cerrarCliente(int fd){
    mtxScreen.lock();
    for (int i=0; i<clientesFds.size(); ++i){
        if (fd == clientesFds[i]){            
            clientesFds.erase (clientesFds.begin()+i);
            break;
        }
    }
    mtxScreen.unlock();

    shutdown(fd, SHUT_RDWR);
    close(fd);

    cout << "cliente cerrado" << endl;
}




void cliente(int fd){ //envio y recepcion de mensajes    
    char clienteBuffer[2048];
    cout << "cliente aceptado" << endl;

    while(conectado){
        int tam = recv(fd, clienteBuffer, COMMANDSIZE, 0)  ;    
        if ( tam <= 0 ){
            cerrarCliente(fd);
            return;
        }
        cout << tam << endl;
        bzero(clienteBuffer,2048);
    }
}




void aceptarClientes (){
    socklen_t sin_size = sizeof(serv_addr);

    while (conectado){
        int newfd = accept(sockfd, (struct sockaddr *)&serv_addr, &sin_size);
        if (newfd == -1){
            continue;
        }

        mtxScreen.lock();
            clientesFds.push_back(newfd);
        mtxScreen.unlock();

        thread (cliente, newfd).detach();
    }
}




int main(int argc, char **argv) {
    
    char buffer[50];

    conectar(8000); //puerto

    thread (aceptarClientes).detach();
    //interfaz de consola   
    while( conectado ) {
        cin.getline(buffer,50);
        string line = buffer;
        string option = getWord(&line);
        if (option == "quit"){            
            conectado = false;            
            break;
        }       
        else{
            mtxScreen.lock();
            cout<<"orden no reconocida"<<endl;
            mtxScreen.unlock();
        }
        bzero(buffer,50);
    }
    
    desconectar();
    return 0;
} 