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
// devuelve la primera palabra de una cadena y la borra
string getWord(string *s){
    int pos;
    //eliminar espacios en blanco al inicio, trim
    for (pos = 0; pos < s->size() && s->at(pos) == ' '; ++pos){};
    *s = s->substr(pos);
    if (s->empty())
        return "";
    //obtencion de la primera palabra
    for (pos = 0; pos < s->size() && s->at(pos) != ' '; ++pos){};
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
    vector<string> nodos;
    int profundidad = 0;
    int tamanio = 0;
    
    Packet(){};
    
    string generarPaquete(){
    	string paquete = "";

        if ( opcion == "n" ){
            
        }
        else if ( opcion == "l" ){

        }
	    else if ( opcion == "q" ){

        }
        else if ( opcion == "p" ){

        }
        else if ( opcion == "c" ){

        }
        else if ( opcion == "s" ){

        }

        return paquete;
    };

    void leerCabezera(string s){
        opcion = s.substr(0,1);
        tamanio = stoi(s.substr(1,4));
    }

    void leerCuerpo(string s){

    }

    void clear(){

    }   
};

#endif 