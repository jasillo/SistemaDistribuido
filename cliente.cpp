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
    if ( !cliente.conectar(0) ) // posicion 0 es la del servidor maestro
        return -1;
    //cout <<argv[1]<<endl;
    if (opcion == "-N"){
        

    }
    else if (opcion == "-L"){
        

    }
    else if (opcion == "-Q"){
        paquetador.opcion =  "q";
        paquetador.payload = string(argv[2]) + " " + string(argv[3]);
        cout<<paquetador.generarPaqueteQ()<<endl;
        cliente.enviarMensaje( paquetador.generarPaqueteQ() );

        paquetador.opcion = cliente.recibirMensaje(1);
        paquetador.tamanio = cliente.recibirMensaje(4);
        paquetador.payload = cliente.recibirMensaje(stoi(paquetador.tamanio));
        cout<<"respuesta : "<<paquetador.payload<<endl;
    }
    else if (opcion == "-P"){
        paquetador.opcion =  "p";
        for (int i = 2; i < argc; ++i)
        {
            paquetador.datos.push_back(argv[i]);
        }
        cout<<cliente.recibirMensaje(1)<<endl;
        cout<<cliente.recibirMensaje(4)<<endl;


    }
    else if (opcion == "-C"){
        paquetador.opcion =  "c|"; 

    }
    else if (opcion == "-S"){
        paquetador.opcion =  "s";

    }

   
    return 0;
};

 