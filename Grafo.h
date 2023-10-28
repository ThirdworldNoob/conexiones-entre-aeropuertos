#ifndef GRAFO_H_
#define GRAFO_H_
#include <map>
#include <list>
#include <iostream>
#include <string>

using namespace std;

template <typename C> class Grafo
{
public:
	class Arco
	{
	public:
		Arco();
		Arco(int adyacente, const C & costo);
		int devolver_adyacente() const;
		const C & devolver_costo() const;
	private:
		int vertice;
		C costo;
	}; // class Arco

public:
	// NOTA: Dependiendo de la implementación puede ser necesario incluir otras funciones constructuras
	Grafo();
	Grafo(const Grafo & otroGrafo);

	~Grafo();

	Grafo & operator = (const Grafo & otroGrafo);
	
	// Devuelve true si la cantidad de vértices es cero
	bool esta_vacio() const;

	// Indica la cantidad de vértices del grafo
	int devolver_longitud() const;

	bool existe_vertice(int vertice) const;

	bool existe_arco(int origen, int destino) const;

	// PRE CONDICION: existe_arco(origen, destino)
	const C & costo_arco(int origen, int destino) const;

	void devolver_vertices(list<int> & vertices) const;

	void devolver_adyacentes(int origen, list<Arco> & adyacentes) const;

	void agregar_vertice(int vertice);

	// POST CONDICION: Para todo vértice v != vertice: !existeArco(v, vertice) && !existeArco(vertice, v)
	void eliminar_vertice(int vertice);

	// PRE CONDICION: existeArco(origen, destino)
	void modificar_costo_arco(int origen, int destino, const C & costo);

	// PRE CONDICION: existeVertice(origen) && existeVertice(destino)
	// POST CONDICION: existeArco(origen, destino)
	void agregar_arco(int origen, int destino, const C & costo);

	// POST CONDICION: !existeArco(origen, destino)
	void eliminar_arco(int origen, int destino);

	void vaciar();

private:
	map<int, map<int,C> > grafo;
	int cantnodos;

}; // class Grafo


/*
 * Arco
 */

template <typename C> Grafo<C>::Arco::Arco()
{
	
}

template <typename C> Grafo<C>::Arco::Arco(int adyacente, const C & costo)
{
	this->vertice = adyacente;
	this->costo = costo;
}

template <typename C> int Grafo<C>::Arco::devolver_adyacente() const
{
	return this->vertice;
}

template <typename C> const C & Grafo<C>::Arco::devolver_costo() const
{
	return this->costo;
}


/*
 * Grafo
 */

template <typename C> Grafo<C>::Grafo()
{
	this->cantnodos = 0;
}

template <typename C> Grafo<C>::Grafo(const Grafo & otroGrafo)
{
	*this = otroGrafo;
}

template <typename C> Grafo<C>::~Grafo()
{
	this->vaciar();
}

template <typename C> Grafo<C> & Grafo<C>::operator = (const Grafo & otroGrafo)
{
	this->cantnodos = otroGrafo.cantnodos;
	this->grafo = otroGrafo.grafo;
    return *this;
}

template <typename C> bool Grafo<C>::esta_vacio() const
{
	if (this->cantnodos == 0){
		return false;
	}else{
		return true;
	}
    
}

template <typename C> int Grafo<C>::devolver_longitud() const
{
    return cantnodos;
}

template <typename C> bool Grafo<C>::existe_vertice(int vertice) const
{
	bool existe;
	if (grafo[vertice] != NULL){ /*not quite sure about this null*/
		existe = true;
	}else{
		existe = false;
	}
}

template <typename C> bool Grafo<C>::existe_arco(int origen, int destino) const
{
    bool existe;
	if (grafo[origen][destino] != NULL){ /*not quite sure about this null*/
		existe = true;
	}else{
		existe = false;
	}
}

template <typename C> const C & Grafo<C>::costo_arco(int origen, int destino) const
{
	return grafo[origen][destino];
}

template <typename C> void Grafo<C>::devolver_vertices(list<int> & vertices) const
{
	auto it = grafo.begin();
	while (it != grafo.end()){
		vertices.push_back(it->first);
		it++;
	}
}

template <typename C> void Grafo<C>::devolver_adyacentes(int origen, list<Arco> & adyacentes) const
{
	auto it = grafo[origen].begin();
	while (it != grafo[origen].end()){
		adyacentes.push_back(Arco(it->first, it->second));
		it++;
	}
}

template <typename C> void Grafo<C>::agregar_vertice(int vertice)
{
	if (!existe_vertice(vertice)){
		cantnodos++;
		grafo[vertice] = map<int,C>(); /*not sure*/
	}
}

template <typename C> void Grafo<C>::eliminar_vertice(int vertice)
{
	if (existe_vertice(vertice)){
		cantnodos--;
		grafo.erase(vertice);
	}
}

template <typename C> void Grafo<C>::modificar_costo_arco(int origen, int destino, const C & costo)
{
	if (existe_arco(origen, destino)){
		grafo[origen][destino] = costo;
	}
}

template <typename C> void Grafo<C>::agregar_arco(int origen, int destino, const C & costo)
{
	if (!existe_arco(origen, destino)){
		grafo[origen][destino] = costo;
	}
}

template <typename C> void Grafo<C>::eliminar_arco(int origen, int destino)
{
	if (existe_arco(origen, destino)){
		grafo[origen].erase(destino);
	}
}

template <typename C> void Grafo<C>::vaciar()
{
	grafo.clear();
	cantnodos = 0;
}

#endif /* GRAFO_H_ */
