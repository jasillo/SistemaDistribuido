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
vector<int> esclavosFDs;




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

    if (conectado){       
        paquetador.opcion = Servidor::recibirMensaje(1, fd);
        cout << "opcion : 	"<<paquetador.opcion <<endl;
        
        paquetador.tamanio = Servidor::recibirMensaje(4, fd);
        int tam = stoi(paquetador.tamanio);

        if ( paquetador.opcion == "n" ){        	
        	//paquetador.datos.push_back(Servidor::recibirMensaje(tam, fd));
        	//cout<<paquetador.datos[0]<<endl;
        }
        else if ( paquetador.opcion == "l" ){
            if (paquetador.tamanio == "") {
                cout<<"error al recibir tamanio del cliente"<<endl;
                return;
            }
            paquetador.payload = Servidor::recibirMensaje(tam, fd);
            //cout<<"pay:"<<paquetador.payload<<endl;
            paquetador.getListaPalabras();
            int esclavoID = paquetador.hash(paquetador.datos[0]);
            int esclavoID2 = (esclavoID+1)%(NumeroEsclavos+1);
            if (esclavoID2 == 0)
                esclavoID2 =1;
            
            Cliente *cliente = new Cliente();
            if ( !cliente->conectar(esclavoID) ) // posicion 0 es la del servidor maestro
            {
                if (!cliente->conectar(esclavoID2)){
                    Servidor::enviarMensaje("r0005false", fd);
                    return ;    
                }                
            }

            //cout<<"paquete:"<<paquetador.generar()<<endl;
            if (!cliente->enviarMensaje(paquetador.generar())){
                Servidor::enviarMensaje("r0000", fd);
                delete cliente;    
            }
            paquetador.opcion = cliente->recibirMensaje(1);
            paquetador.tamanio = cliente->recibirMensaje(4);
            if (paquetador.tamanio == "") {
                cout<<"error al recibir tamanio del esclavo"<<endl;
                delete cliente;
                return;
            }
            paquetador.payload = cliente->recibirMensaje(stoi(paquetador.tamanio));
            Servidor::enviarMensaje(paquetador.generar(), fd);
            delete cliente;
        }
	    else if ( paquetador.opcion == "q" ){
	    	paquetador.payload = Servidor::recibirMensaje(tam, fd);
	    	paquetador.procesarQ();

	    	int esclavoID = paquetador.hash(paquetador.datos[0]);
            int esclavoID2 = (esclavoID+1)%(NumeroEsclavos+1);
            if (esclavoID2 == 0)
                esclavoID2 =1;

	    	Cliente cliente;
		    if ( !cliente.conectar(esclavoID) ) // posicion 0 es la del servidor maestro
		    {
		    	if (!cliente->conectar(esclavoID2)){
                    Servidor::enviarMensaje("r0000", fd);
                    return ;    
                }
		    }
		    cliente.enviarMensaje(paquetador.generar());

            do {
                paquetador.opcion = cliente.recibirMensaje(1);
                paquetador.tamanio = cliente.recibirMensaje(4);
                if (paquetador.tamanio == "") {
                    cout<<"error al recibir tamanio del esclavo"<<endl;
                    Servidor::enviarMensaje("r0000", fd);
                    return;
                }
                paquetador.payload = cliente.recibirMensaje(stoi(paquetador.tamanio));
                if (paquetador.tamanio != "0000"){
                    Servidor::enviarMensaje(paquetador.generar(),fd);
                }
            } while (stoi(paquetador.tamanio) != 0) ;       
		    Servidor::enviarMensaje("r0000", fd);
        }
        else if ( paquetador.opcion == "p" ){
            paquetador.payload = Servidor::recibirMensaje(tam, fd);
            paquetador.getListaPalabras();
            struct Packet p;
            for (int i = 0; i < paquetador.datos.size()-1; ++i)
            {
                int esclavoID = paquetador.hash(paquetador.datos[i]);
                int esclavoID2 = (esclavoID+1)%(NumeroEsclavos+1);
                if (esclavoID2 == 0)
                    esclavoID2 =1;

                Cliente *cliente = new Cliente();
                if ( !cliente->conectar(esclavoID) ) // posicion 0 es la del servidor maestro
                {
                    if (!cliente->conectar(esclavoID2)){
                        Servidor::enviarMensaje("r0000", fd);
                        return ;    
                    }
                }
                p.opcion = "p";
                p.payload = paquetador.datos[i] + " " + paquetador.datos[i+1];
                cliente->enviarMensaje(p.generarPaqueteQ());
                delete cliente;
            }
                       
        }
        else if ( paquetador.opcion == "c" ){            
            paquetador.payload = Servidor::recibirMensaje(tam, fd);
            //cout<<"pay:"<<paquetador.payload<<endl;
            int redundancia = 3;
            int esclavoID = paquetador.hash(paquetador.payload);
            string paq = paquetador.generar();
            paquetador.payload = "";
            for (int i = 0; i < 3; ++i)
            {
                Cliente *cliente = new Cliente();
                if (esclavoID == 0) 
                    esclavoID ++;
                
                if ( !cliente->conectar(esclavoID) )// posicion 0 es la del servidor maestro
                    redundancia--;
                else{
                    cliente->enviarMensaje(paq);
                    cliente->recibirMensaje(5);
                    string b = cliente->recibirMensaje(1);

                    if (b == "f"){
                        redundancia--;
                    }
                    else{
                        paquetador.payload += cliente->ipDestino + "-"+std::to_string(cliente->puertoDestino)+" ";    
                    }                    
                }
                
                delete cliente;
                esclavoID = (esclavoID + 1) % (NumeroEsclavos+1);
            }
            paquetador.opcion = "r";
            paquetador.payload = std::to_string(redundancia) +" "+ paquetador.payload; 
            Servidor::enviarMensaje(paquetador.generarPaqueteQ(), fd);
        }
        else if ( paquetador.opcion == "s" ){
            for (int i = 1; i < 9; ++i)
            {
                Cliente *cliente = new Cliente();
                
                if ( cliente->conectar(i) )// posicion 0 es la del servidor maestro
                {
                    paquetador.payload += cliente->ipDestino + "-"+std::to_string(cliente->puertoDestino)+"_"+"ACTIVO"+" ";
                    cliente->enviarMensaje("c0000");
                }
                else{
                    paquetador.payload += listaDeIps[i]+"_"+std::to_string(listaDePuertos[i])+"_"+"INACTIVO"+" ";
                }
                
                delete cliente;
            }
            paquetador.opcion = "r";
            Servidor::enviarMensaje(paquetador.generarPaqueteQ(),fd);
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
	for (int i = 0; i < 8; ++i)
	{
		esclavosFDs.push_back(-1);
	}
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