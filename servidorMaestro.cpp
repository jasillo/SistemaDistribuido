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

        Cliente clienteTemp("127.0.0.1", 8001);
        if (!clienteTemp.conectar())
        	cout<< "esclavo no encontrado;" << endl;
        clienteTemp.enviarMensaje("holaaaaa");

        send(fd, clienteBuffer, 100, 0);
        bzero(clienteBuffer,2048);
    }
}




void aceptarClientes (){ //thread que recepciona a los clientes 
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
    char buffer[50];

    // 0 es el maestro, ver listas en Coneccion.h
    servidor = new Servidor(0); 
    if (!servidor->iniciar())
    	return -1;    
    conectado = true;

    thread (aceptarClientes).detach();

    //interfaz de consola   
    //"quit para cerrar la aplicacion"
    while( conectado ) {
        cin.getline(buffer,50);
        string line = buffer;
        string option = getWord(&line); //getRowd en Packet.h
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