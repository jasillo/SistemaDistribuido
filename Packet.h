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
    unsigned int uvalue = value;
    if (value < 0) {
        uvalue = -uvalue;
    }
    std::string result;
    while (digits-- > 0) {
        result += ('0' + uvalue % 10);
        uvalue /= 10;
    }
    if (value < 0) {
        result += '-';
    }
    reverse(result.begin(), result.end());
    return result;
}




struct PACKET { 
    string opt; //opcion del comando: send, login, logout
    string message;
    
    PACKET(){};

    void analizeHeader(string command){    	
    	opt = command.substr(USERNAMESIZE, COMMANDSIZE);
    };
    
    string generate(){
    	string r = "";
	    
        return r;
    };    
};

#endif /* _PACKET_H_ */