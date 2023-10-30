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
