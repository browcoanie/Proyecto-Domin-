#include <iostream>
#include "funciones.h"

using namespace std;  

// Mostramos la fichas
void mostarLista(ListaFicha lista){
    NodoFicha *aux = lista.cabeza;
    while(aux != NULL){
        cout << "[" << aux -> dato.lado1 <<"|"<< aux -> dato.lado2 << "] ";
        aux = aux -> siguiente;
    }
    cout << endl;
}


int main(){
    ListaFicha lista;
    lista.cabeza = nullptr;

    generarFichas(lista);
    cout << "Fichas generadas:\n";
    mostarLista(lista);

    liberarLista(lista);
    return 0;

}
