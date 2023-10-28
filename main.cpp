#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include "Grafo.h"
#include "/Users/agustin/Downloads/intento tpe 2/include/Grafo.h"
using namespace std;

/*void cargar_aeropuertos(string path);
void cargar_reservas(string path);
void cargar_aeropuertos_rutas(string path);
void cargar_rutas(string path);*/

struct Ruta{
        string aeropuerto_origen;
        string aeropuerto_destino;        
        bool cabotaje;
        double distancia;
        map<string,int> aerolineas_asientos;//relaciona aerolinea/s q hace la ruta con la cantidad de asientos disponibles

};

int main()
{  
    Grafo<string> conexiones_aeropuertos; //modela conexiones entre aeropuertos
    map<string,int> relaciones; //relaciona el nombre del aeropuerto con un int que lo representa en el grafo
    map<string,Ruta> info_rutas; //relaciona aerop_origen+aerop_destino con la info de la ruta, tiene cargada las reservas
    cout << "grafo creado" << endl;
    cargar_aeropuertos("datasets/Aeropuertos.txt",conexiones_aeropuertos,relaciones);
    cargar_reservas("datasets/Reservas.txt",info_rutas);
    cargar_rutas("datasets/Rutas.txt",conexiones_aeropuertos,relaciones,info_rutas);
    menu(conexiones_aeropuertos,relaciones,info_rutas);
    return 0;
}
void cargar_aeropuertos(string path,Grafo<string> &grafo, map<string,int>& relaciones)
{
    
    int i=0;
    ifstream origen(path.c_str());
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else {
        cout << "AEROPUERTOS " << endl;
        cout << "------------------------" << endl;
        while (!origen.eof()) {
            string linea;
            getline(origen,linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto = linea.substr(0, primera_pos);
            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_ciudad = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);
            string pais = linea.substr(segunda_pos + 1);
            
            relaciones[nombre_aeropuerto]=i; //guardo la relacion int-string,si la idea es poner todos los datos hacer una struct con 3 str
            grafo.agregar_vertice(i);
            i++;
            cout <<nombre_aeropuerto << " - " << nombre_ciudad << " - " << pais << endl;
        }
    }
}
void cargar_aerolineas_rutas(string aerolineas, Ruta & nueva_ruta)
{
    //Se elimina la llave inicial
    aerolineas = aerolineas.substr(1);
    size_t inicial = 0;
    size_t pos = 0;
    size_t pos_final = aerolineas.find('}');
    while ((pos != std::string::npos) && (pos + 1 < pos_final)) {
        pos = aerolineas.find(',', inicial);
        string dato_aerolinea = aerolineas.substr(inicial, pos - inicial);
        size_t pos_separador = dato_aerolinea.find('-');
        string aerolinea = dato_aerolinea.substr(0, pos_separador);
        string asientos_texto = dato_aerolinea.substr(pos_separador + 1);
        int asientos = atoi(asientos_texto.c_str());
        nueva_ruta.aerolineas_asientos[aerolinea] = asientos; //guardo la aerolinea y la cantidad de asientos disponibles
        cout <<"[ " << aerolinea << " - " << asientos << " ]" << endl;
        inicial = pos + 1;
    }
}
void cargar_rutas(string path,Grafo<string> &grafo, map<string,int>relaciones, map<string,Ruta> &info_rutas)
{
    ifstream origen(path.c_str());
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else {
        cout << "RUTAS " << endl;
        cout << "------------------------" << endl;
        int i =0;
        while (!origen.eof()) {
            string linea;
            getline(origen,linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto_origen = linea.substr(0, primera_pos);

            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_aeropuerto_destino = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);

            size_t tercera_pos = linea.find(',', segunda_pos + 1);
            string distancia_texto = linea.substr(segunda_pos + 1, tercera_pos - segunda_pos - 1);
            double distancia = atof(distancia_texto.c_str());

            size_t cuarta_pos = linea.find(',', tercera_pos + 1);
            string cabotaje_texto = linea.substr(tercera_pos + 1, cuarta_pos - tercera_pos - 1);

            string aerolineas = linea.substr(cuarta_pos + 1);
            
            //guardo arcos en el grafo
            int origen =relaciones[nombre_aeropuerto_origen];
            int destino =relaciones[nombre_aeropuerto_destino];
            grafo.agregar_arco(origen,destino,distancia_texto);
            //guardo la info de la ruta al mapa que las guarda
            Ruta nueva_ruta;
            nueva_ruta.aeropuerto_origen = nombre_aeropuerto_origen;
            nueva_ruta.aeropuerto_destino = nombre_aeropuerto_destino;
            nueva_ruta.distancia = distancia;
            nueva_ruta.cabotaje = (cabotaje_texto == "true");

            cout << nombre_aeropuerto_origen << " - " << nombre_aeropuerto_destino << " - " << distancia << " - "
                << cabotaje_texto << " - ";
            cargar_aerolineas_rutas(aerolineas,nueva_ruta); //cargo las aerolineas que hacen la ruta y su cantidad de asientos disp

            info_rutas[nombre_aeropuerto_origen + "-" + nombre_aeropuerto_destino] = nueva_ruta; //guardo la info de la ruta en el mapa
        }
    }
}
void cargar_reservas(string path,  map<string,Ruta> &info_rutas)
{
    ifstream origen(path.c_str());
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else {
        cout << "RESERVAS " << endl;
        cout << "------------------------" << endl;
        while (!origen.eof()) {
            string linea;
            getline(origen,linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto_origen = linea.substr(0, primera_pos);

            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_aeropuerto_destino = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);

            size_t tercera_pos = linea.find(',', segunda_pos + 1);
            string aerolinea = linea.substr(segunda_pos + 1, tercera_pos - segunda_pos - 1);

            string asientos_reservados_texto = linea.substr(tercera_pos + 1);
            int asientos_reservados = atoi(asientos_reservados_texto.c_str());
            
            info_rutas[nombre_aeropuerto_origen + "-" + nombre_aeropuerto_destino].aerolineas_asientos[aerolinea] -= asientos_reservados;
            cout <<nombre_aeropuerto_origen << " - " << nombre_aeropuerto_destino << " - " << aerolinea
                << " - " << asientos_reservados << endl;
        }
    }
}

void menu(Grafo<string>grafo, map<string,int>relaciones, map<string,Ruta>info_rutas){

    int opcion= -1;
    cout << "   --------       MENU    ----------  " << endl;
    while (opcion !=0){

        cout << " INGRESE 0 si quiere salir" << endl;
        cout << " INGRESE 1 si quiere listar todos los aeropuertos" << endl;
        cout << " INGRESE 2 si quiere listar todas las reservas realizadas" << endl;
        cout << " INGRESE 3 si quiere usar el servicio 1" << endl;        
        cout << " INGRESE 4 si quiere usar el servicio 2" << endl;
        cout << " INGRESE 5 si quiere usar el servicio 3" << endl;
        cin >> opcion;

        switch (opcion)
        {
            case 1: 
                listar_aeropuertos(relaciones);
                break;
            case 2: 
                //listar_reservas(info_rutas);
                break;
            case 3: 
                servicio_1(info_rutas);
                break;
            case 4: 
                //servicio_2(grafo,relaciones,info_rutas);
                break;
            case 5: 
                //servicio_3(grafo,relaciones,info_rutas);
                break;
            default:
                cout << "opcion invalida" << endl;
                break;
        }
    }
}



void listar_aeropuertos(map<string,int>relaciones){

    list<string> aeropuertos;
    //itero sobre relaciones y agrego los nombres de los aeropuertos a la lista
    for (map<string,int>::iterator it = relaciones.begin(); it != relaciones.end(); ++it){
        aeropuertos.push_back(it->first);
    }
    //imprimo la lista
    cout << "AEROPUERTOS " << endl;
    cout << "------------------------" << endl;
    for (list<string>::iterator it = aeropuertos.begin(); it != aeropuertos.end(); ++it){
        cout << *it << endl;
    }
}

//void listar_reservas(info_rutas); que onda solo tengo q traer el archivo de reservas y mostrarlo? o tengo q hacer algo mas?


void servicio_1(map<string,Ruta> info_rutas){

    cout<< "servicio 1: verificar vuelo directo entre dos aeropuertos para una linea en particular" << endl;
    string aeropuerto_origen;
    string aeropuerto_destino;
    string aerolinea;
    cout<< "ingrese el nombre del aeropuerto de origen" << endl;
    cin >> aeropuerto_origen;
    cout<< "ingrese el nombre del aeropuerto de destino" << endl;
    cin >> aeropuerto_destino;
    cout<< "ingrese el nombre de la aerolinea" << endl;
    cin >> aerolinea;
    //verifico que existe un vuelo directo entre los dos aeropuertos para la aerolinea dada
    if (info_rutas[aeropuerto_origen + "-" + aeropuerto_destino].aerolineas_asientos[aerolinea] > 0){
        cout << "existe un vuelo directo entre " << aeropuerto_origen << " y " << aeropuerto_destino << " para la aerolinea " << aerolinea << endl;
        cout << "la cantidad de asientos disponibles es: " << info_rutas[aeropuerto_origen + "-" + aeropuerto_destino].aerolineas_asientos[aerolinea] << endl;
    }
    else{
        cout << "no existe un vuelo directo entre " << aeropuerto_origen << " y " << aeropuerto_destino << " para la aerolinea " << aerolinea << endl;
    }
}
/////////////////////-------servicio 2-------////////////////////////
struct info_recorrido{
    string aerolinea;
    double kms;
    int cant_escalas;
};

void DFS_listar_rutas(Grafo<string> grafo,int aeropuerto, int destino, string aerolinea,list<info_recorrido> &lista_rutas, map<string,Ruta>info_rutas, bool visitados[],info_recorrido recorrido, string aeropuerto_destino, map<string,int>relaciones)
{
    //recorro con DFS desde nodo origen y voy verificando si es la misma aerolinea y si al nodo al que me muevo es el nodo destino
    //recordar q tmb para q este en la lista tiene q haber asientos disponibles

    visitados[aeropuerto] = true;
    string aeropuerto_actual = relaciones.find(aeropuerto)->first;
    if (aeropuerto == destino) {
        //guardo la info del recorrido en la lista
        recorrido.kms += info_rutas[aeropuerto_actual + "-" + aeropuerto_destino].distancia;
        lista_rutas.push_back(recorrido);
    }
    else{
            //itero sobre los adyencentes en el grafo
            list<Arco> adyacentes;
            grafo.devolver_adyacentes(aeropuerto,adyacentes);
            for (list<Arco>::iterator it = adyacentes.begin(); it != adyacentes.end(); ++it){

                int adyacente = it->devolver_adyacente();
                if (!visitados[adyacente]){
                    string aeropuerto_adyacente = relaciones.find(adyacente)->first;
                    //verifico si es la misma aerolinea y si hay asientos disponibles
                    if (info_rutas[aeropuerto_actual + "-" + aeropuerto_adyacente].aerolineas_asientos[aerolinea] > 0){
                    //guardo la info del recorrido
                    recorrido.kms += info_rutas[aeropuerto_actual + "-" + aeropuerto_adyacente].distancia;
                    recorrido.cant_escalas++;
                    //llamo a DFS para el adyacente
                    DFS_listar_rutas(grafo,adyacente,destino,aerolinea,lista_rutas,info_rutas,visitados,recorrido,aeropuerto_destino,relaciones);
                    }
                }
            }
        }
    }
    
    

void encontrar_aerolineas(string aeropuerto_origen,list<Arco> adyacentes_origen,vector<string>& aerolineas, map<string,int>relaciones,map<string,Ruta>info_rutas){
    //recorro la lista, busco el arco entre origen y su adyacente y guardo todas las aerolineas que pueden hacer el viaje en aerolineas
    for (list<Arco>::iterator it = adyacentes_origen.begin(); it != adyacentes_origen.end(); ++it){

        int destino = it->devolver_adyacente();
        string aeropuerto_destino = relaciones.find(destino)->first;       

        //recorro las posibles aerolineas que hacen el viaje y las guardo en aerolineas
        for (map<string,int>::iterator it = info_rutas[aeropuerto_origen + "-" + aeropuerto_destino].aerolineas_asientos.begin(); it != info_rutas[aeropuerto_origen + "-" + aeropuerto_destino].aerolineas_asientos.end(); ++it){
            aerolineas.push_back(it->first);
        }
    }
}

void servicio_2(Grafo<string> grafo, map<string,int>relaciones, map<string,Ruta>info_rutas){

    cout<< "servicio 2: obtener todos los vuelos directos y con escalas entre dos aeropuertos para un par de aeropuertos usando siempre la misma aerlinea" << endl;
    string aeropuerto_origen;
    string aeropuerto_destino;
    cout<< "ingrese el nombre del aeropuerto de origen" << endl;
    cin >> aeropuerto_origen;
    cout<< "ingrese el nombre del aeropuerto de destino" << endl;
    cin >> aeropuerto_destino;
    int origen = relaciones[aeropuerto_origen];
    int destino = relaciones[aeropuerto_destino];

    list<Arco> adyacentes_origen; // no se porq no me toma la clase Arco
    grafo.devolver_adyacentes(origen,adyacentes_origen);
    vector<string> aerolineas;
    encontrar_aerolineas(aeropuerto_origen,adyacentes_origen,aerolineas,relaciones,info_rutas); //encuentro las aerolineas que conectan con los adyacentes a origen y los cargo en aerolineas
    
    
    int cantnodos = grafo.devolver_longitud();
    bool visitados[cantnodos]; // lo inicializo en falso
    //hago DFS para cada aerolinea desde origen
    list<info_recorrido> lista_rutas;
    string aerolinea;
    for (int i =0;i<aerolineas.size();i++){

        for (int j =0;j<cantnodos;j++){
            visitados[j]=false;
        }
        aerolinea = aerolineas[i];
        info_recorrido recorrido;
        recorrido.aerolinea = aerolineas[i];
        recorrido.kms = 0;
        recorrido.cant_escalas = 0; 
       
        DFS_listar_rutas(grafo,origen,destino,aerolinea,lista_rutas,info_rutas,visitados,recorrido,aeropuerto_destino,relaciones);
    }
}
