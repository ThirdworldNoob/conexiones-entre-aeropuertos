/////////////////////-------servicio 3-------////////////////////////


vector<int> vecino_mas_cercano(int vertice, Grafo<string> grafo, bool visitados[],map<string,int>relaciones,vector<int> &solucion,double kms){

    visitados[vertice]=true;
    solucion.push_back(vertice);
    int kms_aeropuerto_mas_cercano= 1e9; //distancia infita
    int aeropuerto_mas_cercano= -1;

    //itero sobre los adyacentes del vertice 
    for(auto it = grafo[vertice].begin(); it != grafo[vertice].end(); it++){
      if (!visitados[it->first] && grafo[vertice][it->first] < kms_aeropuerto_mas_cercano){
        kms_aeropuerto_mas_cercano = grafo[vertice][it->first];
        aeropuerto_mas_cercano = it->first;
      }
    }
    if (aeropuerto_mas_cercano != -1){
        kms += kms_aeropuerto_mas_cercano;
        vecino_mas_cercano(aeropuerto_mas_cercano,grafo,visitados,relaciones,solucion,kms);
    }
    else{
        
        return solucion;
    }

}

bool existe_camino(Grafo<string> grafo,int origen,map<string,int>relaciones,vector<int> & solucion,double kms){

    int cant_aeropuertos = grafo.devolver_longitud();
    bool visitados[cant_aeropuertos];
    for (int i =0;i<cant_aeropuertos;i++){
        visitados[i]=false;
    };
    solucion = vecino_mas_cercano(origen,grafo,visitados,relaciones,solucion,kms);
    if (solucion.size() == cant_aeropuertos && solucion.back() != -1){
        solucion.push_back(origen); //agrego el origen al final para que el camino sea cerrado
        return true;
    }
    else{
        return false;
    }
}

void servicio3(Grafo<string> grafo, map<string,int>relaciones){
    //problema del viajante, lo resuelvo usando la heuristica del vecino mas cercano
    cout<< "ingrese un aeropuerto desde el que quiere verificar si es que hay una ruta que recorra todos los aeropuertos una vez y vuelva a el " << endl; 
    int origen;
    cin >> origen;

    vector<int> solucion;
    double kms;
    bool existe = existe_camino(grafo,origen,relaciones,solucion,kms);

    if (existe != false){
        //imprimo solucion
        int i;
        for (i = 0;i<solucion.size();i++){
            cout << solucion[i] << " -> ";
        }
        cout << "se visitaron "<<i<<" aeropuertos desde "<<origen<<"    y se recorrieron "<<kms<<" kms" << endl;
    }
    else{
        cout << "no existe un camino que recorra todos los nodos del grafo una vez y vuelva a el" << endl;
    }
}
