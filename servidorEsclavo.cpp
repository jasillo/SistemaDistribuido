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
#include "Coneccion.h"

using namespace std;

Servidor *servidor;
bool conectado;
mutex mtxScreen;




void cerrarCliente(int fd){
    shutdown(fd, SHUT_RDWR);
    close(fd);
    cout << "thread cliente cerrado" << endl;
}




void cliente(int fd){ //thread envio y recepcion de mensajes    
    char clienteBuffer[2048];
    cout << "cliente aceptado" << endl;

    while(conectado){
        int tam = recv(fd, clienteBuffer, 100, 0)  ;    
        if ( tam <= 0 ){ 
            cerrarCliente(fd);           
            return;
        }
        cout << "mensaje recibido: "<< clienteBuffer <<endl;

        bzero(clienteBuffer,2048);
    }
}



void aceptarClientes (){ //thread que recepciona a los clientes(maestro o esclavo)
    cout << "aceptaClientes thread iniciadad" << endl;
    while (conectado){
        int newfd = servidor->aceptarConeccion();
        if (newfd == -1)            
            continue;
        thread (cliente, newfd).detach();
    }
    cout << "cerrando thread aceptarClientes()" << endl;
}




int main(int argc, char **argv) {
    if ( argc < 2 ){
        cout<<"falta argumentos"<<endl;
        return -1;
    }
    int esclavoNumero = atoi(argv[1]);
    if (esclavoNumero < 0 || esclavoNumero >= 8){
        cout << "esclavo fuera de rango" << endl;
        return -1;
    }

    char buffer[50];
    servidor = new Servidor(esclavoNumero);

    if ( !servidor->iniciar() ) //puerto
        return -1;
    conectado = true;

    thread (aceptarClientes).detach();

    //interfaz de consola   
    //"quit" para cerrar el esclavo
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
    
    delete servidor;
    sleep(1);
    return 0;
} 