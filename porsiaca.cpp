// buscar ( palabra ) // primera columna, segunda es la respuesta
//
//
// agregar (pal1 , pal2 ) agrega a la tabla
//
// *********funciona***********
// g++ main.cpp -o solr -lcurl -ljsoncpp
//
// insert CSV
// bin/post -c mycol1 bin/datos_redes/Nodos_B.csv
// *********funciona***********
//
//
// INSERT
// *********funcionaBACAN***********
// curl 'http://localhost:8983/solr/mycol1/update?commit=true' -H 'Content-type: application/json' -d '[{"nodo1":"pruebaa", "nodo2":"alejooo"}]'
// *********funcionaBACAN***********
//
// *********funciona***********
// curl 'http://localhost:8983/solr/mycol1/update?commit=true' --data-binary @jasoon.json -H 'Content-type:application/json'
// *********funciona***********
//
// nodo1:pruebaa
//
// *********funciona***********
// SELECT
// http://localhost:8983/solr/mycol1/select?fl=nodo2&indent=on&q=nodo1:pruebaa&wt=json&rows=1000
// *********funciona***********

#include <string>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <unistd.h>
using namespace std;

void agregar (string pal1, string pal2)
{
    string url = "";
    url = "curl \'http://localhost:8983/solr/mycol1/update?commit=true\' -H \'Content-type: application/json\' -d \'[{\"nodo1\":\"" + pal1 + "\", \"nodo2\":\"" + pal2 + "\"}]\'";
    system(url.c_str());
}

int main()
{
  agregar("pruebaa","jbalviiin");
  return 0;

}
