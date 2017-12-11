#pragma once

#ifndef _CONECCION_H_
#define _CONECCION_H_

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


static vector<string> listaDeIps = {   "127.0.0.1","127.0.0.1","127.0.0.1","127.0.0.1","127.0.0.1",
                                "127.0.0.1","127.0.0.1","127.0.0.1","127.0.0.1" };
static vector<int> listaDePuertos = {8000,8001,8002,8003,8004,8005,8006,8007,8008};


class Cliente
{
private:
    
public:
    int socketServidor;
    string ipDestino;
    int puertoDestino;
    struct  sockaddr_in direc;
    bool conectado;

    Cliente(){        
        conectado = false;        
    };

    bool conectar(int indice){
        ipDestino = listaDeIps[indice];
        puertoDestino = listaDePuertos[indice];

        if((socketServidor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            cout << "error al crear el socket"<<endl; 
            return false;    
        }

        direc.sin_family = AF_INET;
        direc.sin_port = htons(puertoDestino);
        inet_pton(AF_INET, ipDestino.c_str(), &direc.sin_addr);

        if (connect(socketServidor,(struct sockaddr *)&direc, sizeof(direc)) < 0 ){
            shutdown(socketServidor, SHUT_RDWR);
            close(socketServidor);
            cout << "conexion al servidor fallo" << endl;
            return false;
        }
        conectado = true;

        cout << "coneccion exitosa" << endl; 
        return true;
    }

    ~Cliente(){
        if (!conectado)
            return;
        conectado = false;

        shutdown(socketServidor, SHUT_RDWR);
        close(socketServidor);
        cout << "cliente se desconecto;"<< endl;
    };

    bool enviarMensaje(string mensaje){
        int tam = send(socketServidor, mensaje.c_str(), mensaje.size(), 0);
        if (tam < 1){
            cout << "error al enviar mensaje" << endl;
            return false;
        }
        return true;
    };

    string recibirMensaje(int tamanio){
        if (tamanio == 0)
            return "";
        char buffer[tamanio+1];
        memset(buffer,0, tamanio +1);
        int tam = recv(socketServidor, buffer, tamanio, 0);
        if (tam < 1){
            cout << "error al recibir mensaje" << endl;
            return "";
        }
        string msm = buffer;
        return buffer;
    };
};






class Servidor
{
private:
    
public:    
    int sockfd;
    int puerto;
    struct  sockaddr_in direc;
    bool conectado;

    //recibe el numero de esclavo que es
    Servidor(int numeroEsclavo){ 
        puerto = listaDePuertos[numeroEsclavo];
        conectado = false;
    };

    bool iniciar(){
        if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
           cout << "error al crear el socket" << endl;
           return false;
        }
        
        direc.sin_family = AF_INET;
        direc.sin_port = htons(puerto);
        direc.sin_addr.s_addr = htons(INADDR_ANY);
        memset(&(direc.sin_zero), 0, 8);
     
        if ( bind(sockfd, (struct sockaddr *)&direc, sizeof(struct sockaddr)) == -1)  {
            cout << "error al asignar puerto" << endl;
            return false;
        }
     
        if ( listen(sockfd, 10) == -1) {
            cout << "error en liste" << endl;    
            return false;
        }

        conectado = true;
        return true;
    };

    int aceptarConeccion(){
        socklen_t sin_size = sizeof(direc);
        return accept(sockfd, (struct sockaddr *)&direc, &sin_size);
    }

    static bool enviarMensaje(string mensaje, int fdCliente){
        int tam = send(fdCliente, mensaje.c_str(), mensaje.size(), 0);
        if (tam < 1){
            cout << "error al enviar mensaje" << endl;
            return false;
        }
        return true;  
    }

    static string recibirMensaje(int tamanio, int fdCliente){
        if (tamanio == 0)
            return "";
        char buffer[tamanio +1];
        bzero(buffer,tamanio+1);
        int tam = recv(fdCliente, buffer, tamanio, 0) ;        
        if (tam < 1){
            cout << "error al recibir mensaje" << endl;
            return "";
        }
        string mensaje = buffer;
        return mensaje;
    }

    ~Servidor(){
        if (!conectado)
            return;
        conectado = false;

        shutdown(sockfd, SHUT_RDWR);
        close(sockfd);
    };
    
};



#endif /* _PACKET_H_ */