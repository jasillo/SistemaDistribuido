#pragma once

#ifndef _PACKET_H_
#define _PACKET_H_

#include <string>
#include <iostream>
#include <algorithm>


#define USERNAMESIZE 2
#define COMMANDSIZE 1

using namespace std;


/********************************
* funciones de manejo de strings
*********************************/
// leer un palabra de un string, luego borra esa palabra
string getWord(string *s){
    int pos;
    //eliminar espacios en blanco al inicio
    for (pos=0; pos < s->size() && s->at(pos) == ' '; ++pos){};
    *s = s->substr(pos);
    if (s->empty())
        return "";
    //obtencion de la primera palabra
    for (pos=0; pos < s->size() && s->at(pos) != ' '; ++pos){};
    string word = s->substr(0,pos);
    *s = s->substr(pos);
    return word;
};

//elimina los espacios delante de un string
string getString(string *s){
    int pos;
    //eliminar espacios en blanco al inicio
    for (pos=0; pos < s->size() && s->at(pos) == ' '; ++pos){};
    *s = s->substr(pos);
    if (s->empty())
        return "";    
    
    return *s;
};

// construir strings de tamaño fijo con ints
std::string fixedLength(int value, int digits = 3) {
    std::string result = std::to_string(value);
    int temp = digits - result.size();
    while (temp-- > 0) {
        result = '0' + result;
    }
    return result;
}




struct Packet { 
    string opcion = ""; //opcion del comando: send, login, logout
    string tamanio = "";
    string payload = "";

    vector<string> datos;
    int profundidad;
    string mensaje;
    
    Packet(){};

    string generar(){
        return opcion + tamanio + payload;
    }

    string generarPaqueteQ(){
        return opcion + fixedLength(payload.size(), 4) + payload;
    }
    
    string generarPaquete(){
    	string paquete = "";

        if ( opcion == "n" ){
            paquete = opcion + fixedLength(datos[0].size(),4) + datos[0];
        }
        else if ( opcion == "l" ){
            paquete = opcion + fixedLength(getSize(),4) + getPayload();
        }
	    else if ( opcion == "q" ){
            string mensaje = std::to_string(profundidad) + " " + datos[0]; 
            paquete = opcion + fixedLength(mensaje.size(),4) + mensaje;
        }
        else if ( opcion == "p" ){
            paquete = opcion + fixedLength(getSize(),4) + getPayload();
        }
        else if ( opcion == "c" ){

        }
        else if ( opcion == "s" ){

        }

        return paquete;
    };

    int getSize() {
        int s = 0;
        for (int i = 0; i < datos.size(); ++i)
        {
            s += datos[i].size();
        }
        s += datos.size();
        return s;
    } 

    string getPayload(){
        string s = "";
        for (int i = 0; i < datos.size(); ++i)
        {
            s += datos[i] + " ";
        }
        return s;
    }

    int hash(string word){
        int h = 0;
        for (int i = 0; i < word.size(); ++i)
        {
            h += int(word[i]);
        }
        h = h % 8;
        cout<<"hash: "<<h<<endl;
        return h;
    }

    void leerPayloadQ(string pay){
        profundidad = stoi(getString(&pay));
        datos.push_back(getString(&pay));
    }

    string paqueteVacio(){
        return "r0000";
    }

    string generarRQ(){
        string msm = getPayload(); 
        return "r"+ fixedLength(msm.size(), 4) + msm;
    }

    void procesarQ(){
        datos.clear();
        string pl = payload;
        profundidad = stoi(getString(&pl));
        datos.push_back(getString(&pl));
    }
};

#endif 