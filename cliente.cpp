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
    string opcion = argv[1];
    
    Cliente cliente("127.0.0.1", 8000);

    if (opcion == "-q"){
        if ( !cliente.conectar() )
            return -1;
        if (cliente.enviarMensaje("hola mundo"))
            cout << "mensaje enviado exitosamente" << endl;
        cout << cliente.recibirMensaje()<<endl;
    }
   
    return 0;
};

 