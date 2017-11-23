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
#include "Coneccion.h"

using namespace std;


int main(int argc, char **argv) {   
    if ( argc < 2 ){
        cout<<"falta argumentos"<<endl;
        return -1;
    }

    struct Packet paquetador;
    string opcion = argv[1];    
    Cliente cliente;
    string cadena;

    if ( !cliente.conectar(0) ) // posicion 0 es la del servidor maestro
        return -1;

    if (opcion == "-N"){
        paquetador.opcion = "n";
        paquetador.nodos.push_back(argv[2]);        
        cliente.enviarMensaje(paquetador.generarPaquete());        

        paquetador.leerCabezera(cliente.recivirMensaje(5)); //cabezera
        paquetador.leerCuerpo(cliente.recivirMensaje(paquetador.tamanio));
    }
    else if (opcion == "-L"){
        paquetador.opcion =  "l";
        paquetador.nodos.push_back(argv[2]);
        paquetador.nodos.push_back(argv[3]);
        cliente.enviarMensaje(paquetador.generarPaquete());

        paquetador.leerCabezera(cliente.recivirMensaje(5)); //cabezera
        paquetador.leerCuerpo(cliente.recivirMensaje(paquetador.tamanio));
    }
    else if (opcion == "-Q"){
        paquetador.opcion =  "q";
        paquetador.nodos.push_back(argv[2]);
        paquetador.profundidad = argv[3];
        cliente.enviarMensaje(paquetador.generarPaquete());

        paquetador.leerCabezera(cliente.recivirMensaje(5)); //cabezera
        paquetador.leerCuerpo(cliente.recivirMensaje(paquetador.tamanio));
    }
    else if (opcion == "-P"){
        paquetador.opcion =  "p";
        for (int i = 2; i < argc; ++i)
            paquetador.push_back(argv[i]);
        cliente.enviarMensaje(paquetador.generarPaquete());

        do{
            paquetador.leerCabezera(cliente.recivirMensaje(5)); //cabezera
            paquetador.leerCuerpo(cliente.recivirMensaje(paquetador.tamanio)); //imprime en pantalla
        }while (paquetador.tamanio != 0)
    }
    else if (opcion == "-C"){
        paquetador.opcion =  "c|"; 

    }
    else if (opcion == "-S"){
        
    }

   
    return 0;
};

 