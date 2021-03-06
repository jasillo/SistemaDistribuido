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
        paquetador.opcion =  "l";
        paquetador.payload = string(argv[2]) + " " + string(argv[3]);
        cout<<paquetador.generarPaqueteQ()<<endl;
        if (!cliente.enviarMensaje(paquetador.generarPaqueteQ())){
            cout<<"error en el maestro"<<endl;
            return;
        }

        paquetador.opcion = cliente.recibirMensaje(1);
        paquetador.tamanio = cliente.recibirMensaje(4);
        if (paquetador.tamanio == "") {
            cout<<"error en el maestro"<<endl;
            return;
        }
        paquetador.payload = cliente.recibirMensaje(stoi(paquetador.tamanio));
        cout<<"respuesta: "<<paquetador.payload<<endl;
    }
    else if (opcion == "-Q"){
        paquetador.opcion =  "q";
        paquetador.payload = string(argv[2]) + " " + string(argv[3]);
        cout<<paquetador.generarPaqueteQ()<<endl;
        if (!cliente.enviarMensaje(paquetador.generarPaqueteQ())){
            cout<<"error en el maestro"<<endl;
            return;
        }

        do {
            paquetador.opcion = cliente.recibirMensaje(1);
            paquetador.tamanio = cliente.recibirMensaje(4);
            if (paquetador.tamanio == "") {
                cout<<"error en el maestro"<<endl;
                return;
            }
            paquetador.payload = cliente.recibirMensaje(stoi(paquetador.tamanio));
            paquetador.datos.clear();
            paquetador.getListaPalabras();
            for (int i = 0; i < paquetador.datos.size(); ++i)
            {
                cout<<paquetador.datos[i]<<endl;
            }
        } while (stoi(paquetador.tamanio) != 0) ;        
        
    }
    else if (opcion == "-P"){
        paquetador.opcion =  "p";
        paquetador.payload = "";
        for (int i = 2; i < argc; ++i)
        {
            paquetador.payload += string(argv[i]) + " ";
        }
        cliente.enviarMensaje(paquetador.generarPaqueteQ());       

    }
    else if (opcion == "-C"){
        paquetador.opcion =  "c";
        paquetador.payload =  string(argv[2]);
        if (cliente.enviarMensaje( paquetador.generarPaqueteQ() )){
            cout<<"error en el maestro"<<endl;
            return;
        }

        paquetador.opcion = cliente.recibirMensaje(1);
        paquetador.tamanio =  cliente.recibirMensaje(4);
        if (paquetador.tamanio == "") {
            cout<<"error en el maestro"<<endl;
            return;
        }
        paquetador.payload = cliente.recibirMensaje(stoi(paquetador.tamanio));
        int concu = stoi(getWord(&paquetador.payload));
        cout<<"concurrencia: "<< concu<<endl;
        for (int i = 0; i < concu; ++i)
        {
            cout<<getWord(&paquetador.payload)<<endl;
        }
        
    }
    else if (opcion == "-S"){
        if(cliente.enviarMensaje( "s0000" )){
            cout<<"error en el maestro"<<endl;
            return;
        }

        paquetador.opcion = cliente.recibirMensaje(1);
        paquetador.tamanio =  cliente.recibirMensaje(4);
        if (paquetador.tamanio == "") {
            cout<<"error en el maestro"<<endl;
            return;
        }
        paquetador.payload = cliente.recibirMensaje(stoi(paquetador.tamanio));
        for (int i = 0; i < 8; ++i)
        {
            cout<<getWord(&paquetador.payload)<<endl;
        }
    }

   
    return 0;
};

 