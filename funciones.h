#include <string>
using namespace std;

// Creamos el nodo de lista
// Cada ficha tiene 2 números
struct Ficha
{
    int lado1;
    int lado2;
};

struct NodoFicha
{
    Ficha dato; // La ficha que se guarda
    NodoFicha *siguiente; // puntero al siguiente nodo
};


struct ListaFicha
{
    NodoFicha *cabeza;// Apunta al primer nodo
};

struct Jugador
{
    string nombre;
    ListaFicha mano; //Lista dinámica con sus fichas
    int puntos;
};

struct Juego
{
    Jugador Jugadores[4];//Maximo 4 jugadores
    int numJugadores;
    ListaFicha pozo; // fichas no repartidas
};



//Creamos un ficha
Ficha crearFicha(int a, int b){
    Ficha final;
    final.lado1 = a;
    final.lado2 = b;
    return final;
}

// Insertar ficha al inicio
void insertarFicha(ListaFicha &lista, Ficha final){
    NodoFicha *nuevo = new NodoFicha;
    nuevo -> dato = final;
    nuevo -> siguiente = lista.cabeza;
    lista.cabeza = nuevo;
}

// Liberamos memoria
void liberarLista(ListaFicha &lista){
    NodoFicha *actual = lista.cabeza;
    while (actual != nullptr ){
        NodoFicha *aux = actual;
        actual = actual -> siguiente;
        delete aux;
    }
    lista.cabeza = nullptr;
}

// GeneramosFichas
void generarFichas(ListaFicha &lista){
    for (int i = 0; i <= 6; i++){
        for(int j = i; j <= 6; j++){
            Ficha final = crearFicha(i,j);// Creamos las variaciones de fichas
            insertarFicha(lista,final); // Insertamos las fichas una lista de fichas
        }
    }

}

// Contamos de las fichas 
int contarFichas(ListaFicha lista){
    int contador = 0;
    NodoFicha *actual = lista.cabeza;
    while(actual != nullptr){
        contador ++;
        actual = actual -> siguiente;
    }
    return contador;
}

