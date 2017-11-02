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
	char opcionBuffer[2];   
    char clienteBuffer[2048];
    cout << "cliente aceptado" << endl;
    Packet paquetador;

    while(conectado){       
        if ( ( paquetador.opcion = Servidor::recibirMensaje(1, fd) ) == "")        	           
            break;
        cout << paquetador.opcion <<endl;
        int tam;
        if ( paquetador.opcion == "n" ){
        	tam = stoi( Servidor::recibirMensaje(3, fd) );
        	cout << tam <<endl;
        	paquetador.datos.push_back(Servidor::recibirMensaje(tam, fd));
        	cout<<paquetador.datos[0]<<endl;
        }
        else if ( paquetador.opcion == "l" ){

        }
	    else if ( paquetador.opcion == "q" ){

        }
        else if ( paquetador.opcion == "p" ){

        }
        else if ( paquetador.opcion == "c" ){

        }
        else if ( paquetador.opcion == "s" ){

        }        


        bzero(clienteBuffer,2048);
    }
    cerrarCliente(fd);
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