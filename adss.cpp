void listar_reservas(vector<Reserva> reservas){

    //itero sobre info_rutas y voy imprimiendo la info de cada ruta y lo devuelvo en un archivo nuevo
    cout<< "RESERVAS " << endl;
    cout<< "------------------------" << endl;
    //abro archivo
    ofstream archivo;
    archivo.open("ReservasHechas.txt",ios::in);
    if (!archivo.is_open()){
        cout << "no se pudo abrir el archivo ReservasHechas.tx" << endl;
    }
    else{
        cout<< "RESERVAS REALIZADAS " << endl;
        archivo << "RESERVAS REALIZADAS " << "\n"<< endl;
        for (int i =0;i<reservas.size();i++){
            cout << reservas[i].aeropuerto_origen << " - " << reservas[i].aeropuerto_destino << " - " << reservas[i].aerolinea << " - " << reservas[i].asientos_reservados << endl;
            archivo << reservas[i].aeropuerto_origen << " - " << reservas[i].aeropuerto_destino << " - " << reservas[i].aerolinea << " - " << reservas[i].asientos_reservados <<"\n" <<endl;
        }
    }
    archivo.close();

}

struct Reserva{
    string aeropuerto_origen;
    string aeropuerto_destino;
    string aerolinea;
    int asientos_reservados;
};

void cargar_reservas(string path,  map<string,Ruta> &info_rutas, vector<Reserva>&reservas)
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

            Reserva reserva;
            reserva.aeropuerto_origen = nombre_aeropuerto_origen;
            reserva.aeropuerto_destino = nombre_aeropuerto_destino;
            reserva.aerolinea = aerolinea;
            reserva.asientos_reservados = asientos_reservados;
            reservas.push_back(reserva);

            cout <<nombre_aeropuerto_origen << " - " << nombre_aeropuerto_destino << " - " << aerolinea
                << " - " << asientos_reservados << endl;
        }
    }
}








///////////---servicio 2---///////////
struct info_recorrido{
    map<string,int> aerolineas;
    /*vector<string> aerolineas; //debe ser un vector de aerolineas porq tal vez entre origen y un adyacente el viaje se podia hacer con mas de una aerolinea
    int cant_aerolineas;*/
    vector<int> aeropuertos;
    double kms;
    int cant_escalas;
};

void buscar_caminos(Grafo<string> grafo,int origen, int destino, vector<info_recorrido> &rutas,info_recorrido &ruta, bool visitados[]){

    visitados[origen] = true;
    ruta.aeropuertos.push_back(origen);
    ruta.cant_escalas++;
    if (origen == destino){
        rutas.push_back(ruta);
    }else{

        list<Grafo<string>::Arco> adyacentes;
        grafo.devolver_adyacentes(origen,adyacentes);
        for (list<Grafo<string>::Arco>::iterator it = adyacentes.begin(); it != adyacentes.end(); ++it){
            
            int adyacente = it->devolver_adyacente();
            if (!visitados[adyacente]){
                buscar_caminos(grafo,adyacente,destino,rutas,ruta,visitados);
            }
        }
    }
    visitados[origen] = false;
    ruta.cant_escalas--;
    ruta.aeropuertos.pop_back();
    
}
void verificar_caminos( vector<info_recorrido> &rutas,map<string,Ruta>info_rutas){

    if( !rutas.empty()){
        for (int i =0;i<rutas.size();i++){

            //tengo q chequear para la primer conexion cual/es son las aerolineas que hacen el viaje
            info_recorrido ruta = rutas[i];
            int origen = ruta.aeropuertos[0];
            int destino = ruta.aeropuertos[1];
            string aeropuerto_origen = relaciones.find(origen)->first;
            string aeropuerto_destino = relaciones.find(destino)->first;
            ruta.kms=0;
            ruta.kms+= info_rutas[aeropuerto_origen + "-" + aeropuerto_destino].distancia;
            ///las rutas pueden arrancar con mas de una aerolinea si una conexion entre origen y su siguiente tiene +1 aerolinea posible, pero al final la ruta va a tener una sola aerolinea
            //esto ocurre en pocos casos para 2 aerolineas y una vez para 3 aerolineas posibles para la primer conexion
            ruta.aerolineas = info_rutas[aeropuerto_origen + "-" + aeropuerto_destino].aerolineas_asientos;
            
            //ahora chequeo si las demas conexiones comparten alguna/s de las aerolinea, notar que una misma ruta podria tener mas de una aerolinea posible, en ese caso duplico la ruta y cargo una aerolinea en cada una
            if(!ruta.aerolineas.empty() && ruta.aerolineas.size() > 1){

                for (int j =1;j<ruta.aeropuertos.size()-1;j++){

                    int aux_origen = ruta.aeropuertos[j];
                    int aux_destino = ruta.aeropuertos[j+1];
                    string a_origen = relaciones.find(aux_origen)->first;
                    string a_destino = relaciones.find(aux_destino)->first;
                    ruta.kms+= info_rutas[a_origen + "-" + a_destino].distancia;
                    map<string,int> aerolineas_viaje = info_rutas[a_origen + "-" + a_destino].aerolineas_asientos;

                    for (map<string,int>::iterator it = ruta.aerolineas.begin(); it != ruta.aerolineas.end(); ++it){
                    
                        if(!ruta.aerolineas.empty()){
                            if(aerolineas_viaje.find(it->first) == aerolineas_viaje.end()){ 
                                ruta.aerolineas.erase(it->first);
                            }

                        }else{
                            rutas.erase(rutas.begin() + i);
                            break;
                        }
                    
                    }
                }
                ///ahora chequeo si una ruta quedo con mas de una aerolinea posible, en ese caso creo n-rutas iguales y cargo una aerolinea en cada una
                if(!ruta.aerolineas.empty() && ruta.aerolineas.size() > 1){
                    for (map<string,int>::iterator it = ruta.aerolineas.begin(); it != ruta.aerolineas.end(); ++it){
                        info_recorrido ruta_aux = ruta;
                        ruta_aux.aerolineas.clear();
                        ruta_aux.aerolineas[it->first] = it->second;
                        rutas.push_back(ruta_aux);
                    }
                }
            }    
        }
    }
    else cout <<" no existen caminos"<<endl;
        
}
void imprimir_caminos(vector<info_recorrido> rutas){
    
        if(!rutas.empty()){
            cout<<"RUTAS "<<endl;
            for (int i =0;i<rutas.size();i++){
                
                cout << "ruta " << i << endl;
                cout << "aerolinea: " << rutas[i].aerolineas.begin()->first << endl;
                cout << "kms: " << rutas[i].kms << endl;
                cout << "cantidad de escalas: " << rutas[i].cant_escalas << endl;
                cout << "aeropuertos: " << endl;
            }
        }
        else cout <<" no existen caminos"<<endl;
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

    vector<info_recorrido> rutas;
    int cantnodos = grafo.devolver_longitud();
    bool visitados[cantnodos]; // lo inicializo en falso
    for (int i =0;i<cantnodos;i++){
        visitados[i]=false;
    }
    info_recorrido ruta;
    buscar_caminos(grafo,origen,destino,rutas,ruta,visitados);//encuentro todos los caminos entre los dos aeropuertos
    verificar_caminos(rutas,info_rutas); //elimino las rutas que no cumplen las condiciones pedidas
    imprimir_caminos(rutas);
}

