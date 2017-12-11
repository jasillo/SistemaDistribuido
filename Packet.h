#pragma once

#ifndef _PACKET_H_
#define _PACKET_H_

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <unistd.h>

#define USERNAMESIZE 2
#define COMMANDSIZE 1

using namespace std;

string data; //will hold the url's contents

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{
    size_t realsize = size * nmemb;
    data.append(buf, realsize);
    return realsize;
}

bool existe(string palabra){
    data = "";
    CURL* curl; //our curl object

    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();

    string url = "";
    url = "http://192.150.0.104:8983/solr/data/select?fl=nodo2&indent=on&q=nodo1:"+palabra+"&wt=json&rows=1000";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);

    curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    Json::Reader reader;
    Json::Value obj;
    reader.parse(data, obj);     // Reader can also read strings
    const Json::Value& objeto = obj["response"];
    int tam = objeto["numFound"].asUInt();   
    if (tam > 0)
        return true;
    return false;
}

void buscar (string palabra, std::vector<string> *v)
{
    data = "";
    CURL* curl; //our curl object

    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();

//nodo 1 se pasa por parametro y lo sumo al string http.....

    string url = "";
    //url = "http://localhost:8983/solr/data/select?fl=nodo2&indent=on&q=nodo1:"+palabra+"&wt=json&rows=1000";
    url = "http://192.150.0.104:8983/solr/data/select?fl=nodo2&indent=on&q=nodo1:"+palabra+"&wt=json&rows=1000";

//    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8983/solr/data/select?fl=nodo2&indent=on&q=nodo1:barbell&wt=json&rows=1000");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

    curl_easy_perform(curl);

    //cout << endl << data << endl;

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    /*____________________________________*/


    Json::Reader reader;
    Json::Value obj;
    reader.parse(data, obj);     // Reader can also read strings
    const Json::Value& objeto = obj["response"];
    int tam = objeto["numFound"].asUInt();
    //cout <<"numero coincidencias : "<< tam << endl;
    //cout <<"valores : "<<endl;
    const Json::Value& valores = objeto["docs"];

    //vector<string> columna2;


//creo vector de string igualando a valores
    for (int i=0; i<tam; ++i){
        //cout<<valores[i]["nodo2"][0].asString()<<endl;
	   v->push_back(valores[i]["nodo2"][0].asString());
    }

	//return columna2;
}
//192.150.0.104

void buscar_sinomimos(string palabra, std::vector<string> *v)
{
    data = "";
    CURL* curl; //our curl object

    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();

//nodo 1 se pasa por parametro y lo sumo al string http.....

    string url = "";
    //url = "http://localhost:8983/solr/data/select?fl=nodo2&indent=on&q=nodo1:"+palabra+"&wt=json&rows=1000";
    url = "http://192.150.0.104:8983/solr/sinonimos/select?fl=nodo2&indent=on&q=nodo1:"+palabra+"&wt=json&rows=1000";

//    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8983/solr/data/select?fl=nodo2&indent=on&q=nodo1:barbell&wt=json&rows=1000");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

    curl_easy_perform(curl);

    //cout << endl << data << endl;

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    /*____________________________________*/


    Json::Reader reader;
    Json::Value obj;
    reader.parse(data, obj);     // Reader can also read strings
    const Json::Value& objeto = obj["response"];
    int tam = objeto["numFound"].asUInt();
    //cout <<"numero coincidencias : "<< tam << endl;
    //cout <<"valores : "<<endl;
    const Json::Value& valores = objeto["docs"];

    //vector<string> columna2;


//creo vector de string igualando a valores
    for (int i=0; i<tam; ++i){
        //cout<<valores[i]["nodo2"][0].asString()<<endl;
       v->push_back(valores[i]["nodo2"][0].asString());
    }

    //return columna2;
}
void agregar (string pal1, string pal2)
{
    string url = "";
    //url = "curl \'http://localhost:8983/solr/data/update?commit=true\' -H \'Content-type: application/json\' -d \'[{\"nodo1\":\"" + pal1 + "\", \"nodo2\":\"" + pal2 + "\"}]\'";
    url = "curl \'http://192.150.0.104:8983/solr/data/update?commit=true\' -H \'Content-type: application/json\' -d \'[{\"nodo1\":\"" + pal1 + "\", \"nodo2\":\"" + pal2 + "\"}]\'";
    system(url.c_str());
}

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
        /*int h = 0;
        for (int i = 0; i < word.size(); ++i)
        {
            h += int(word[i]);
        }
        h = h % 8+1;
        //cout<<"hash: "<<h<<endl;
        return h;*/
        return 1;
    }

    void leerPayloadQ(string pay){
        profundidad = stoi(getString(&pay));
        datos.push_back(getString(&pay));
    }

    string paqueteVacio(){
        return "r0000";
    }

    string generarRQ(int inicio, int fin){
        string sms = "";
        for (int i = inicio; i < datos.size() && i < fin; ++i)
        {
            sms += datos[i] + " ";
        }
        return "r"+ fixedLength(sms.size(), 4) + sms;
    }

    void procesarQ(){
        datos.clear();
        string pl = payload;
        profundidad = stoi(getWord(&pl));
        datos.push_back(getWord(&pl));
    }

    void getListaPalabras(){
        string temp = payload;
        string s = getWord(&temp);
        while(s != ""){
            datos.push_back(s);
            s = getWord(&temp);
        }    
    }
};

#endif
