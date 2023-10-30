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
