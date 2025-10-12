#include <iostream>
#include "funciones.h"
#include <cstdlib>
#include <ctime>
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

// Mostrar mano en pirámide centrada
void mostrarManoPiramideCentrada(ListaFicha mano, string nombreJugador) {
    cout << "\n=== " << nombreJugador << " ===" << endl;
    
    NodoFicha* actual = mano.cabeza;
    int fichasMostradas = 0;
    int maxFichasPorLinea[] = {1, 2, 3, 1}; // Patrón de pirámide
    
    for (int linea = 0; linea < 4 && actual != nullptr; linea++) {
        int fichasEnLinea = maxFichasPorLinea[linea];
        
        // Centrar la línea
        int espacios = (7 - fichasEnLinea) * 2;
        for (int i = 0; i < espacios; i++) {
            cout << " ";
        }
        
        // Mostrar fichas de esta línea
        for (int i = 0; i < fichasEnLinea && actual != nullptr; i++) {
            cout << "[" << actual->dato.lado1 << "|" << actual->dato.lado2 << "] ";
            actual = actual->siguiente;
            fichasMostradas++;
        }
        cout << endl;
    }
    
    // Si hay más de 7 fichas (no debería pasar en dominó normal)
    while (actual != nullptr) {
        cout << "[" << actual->dato.lado1 << "|" << actual->dato.lado2 << "] ";
        actual = actual->siguiente;
    }
    cout << endl;
}

// Mostrar pozo en pirámide
void mostrarPozoPiramideCompleta(ListaFicha pozo) {
    cout << "\n" << string(40, '=') << endl;
    cout << "          POZO - " << contarFichas(pozo) << " FICHAS" << endl;
    cout << string(40, '=') << endl;
    
    NodoFicha* actual = pozo.cabeza;
    int total = contarFichas(pozo);
    
    if (total == 0) {
        cout << "     ╔═════════╗" << endl;
        cout << "     ║  VACÍO  ║" << endl;
        cout << "     ╚═════════╝" << endl;
        return;
    }
    
    // Pirámide adaptable según cantidad de fichas
    if (total <= 7) {
        // Pirámide pequeña
        int patron[] = {1, 2, 3, 1};
        for (int linea = 0; linea < 4 && actual != nullptr; linea++) {
            int espacios = (4 - patron[linea]) * 3;
            for (int i = 0; i < espacios; i++) cout << " ";
            for (int i = 0; i < patron[linea] && actual != nullptr; i++) {
                cout << "[" << actual->dato.lado1 << "|" << actual->dato.lado2 << "] ";
                actual = actual->siguiente;
            }
            cout << endl;
        }
    } else {
        // Pirámide grande
        int patron[] = {2, 3, 4, 3, 2};
        for (int linea = 0; linea < 5 && actual != nullptr; linea++) {
            int espacios = (5 - patron[linea]) * 2;
            for (int i = 0; i < espacios; i++) cout << " ";
            for (int i = 0; i < patron[linea] && actual != nullptr; i++) {
                cout << "[" << actual->dato.lado1 << "|" << actual->dato.lado2 << "] ";
                actual = actual->siguiente;
            }
            cout << endl;
        }
    }
    
    // Mostrar fichas restantes si hay muchas
    if (actual != nullptr) {
        cout << "\n... y " << (total - 14) << " fichas más ...";
    }
}

// Función para mostrar estado completo del juego
void mostrarEstadoCompleto(Juego juego) {
    cout << "\n" << string(60, '=') << endl;
    cout << "           ESTADO ACTUAL DEL JUEGO" << endl;
    cout << string(60, '=') << endl;
    
    // Mostrar pozo primero
    mostrarPozoPiramideCompleta(juego.pozo);
    
    cout << "\n" << string(40, '-') << endl;
    cout << "          MANOS DE JUGADORES" << endl;
    cout << string(40, '-') << endl;
    
    // Mostrar jugadores
    for(int i = 0; i < juego.numJugadores; i++){
        mostrarManoPiramideCentrada(juego.Jugadores[i].mano, 
                                   juego.Jugadores[i].nombre);
        cout << "Puntos acumulados: " << juego.Jugadores[i].puntos << "\n" << endl;
    }
}

// Generamos los jugadores
void inicilizarjugadores(Juego &juego){
    cout << "Ingrese el número de jugadores (2 a 4): ";
    cin >> juego.numJugadores;

    // Validar número de jugadores
    while (juego.numJugadores < 2 || juego.numJugadores > 4) {
        cout << "Error: debe ser entre 2 y 4 jugadores. Intente nuevamente: ";
        cin >> juego.numJugadores;
    }

    for(int i = 0; i < juego.numJugadores; i++){
        cout << "Nombre del jugador " << i + 1 << ": ";
        cin >> juego.Jugadores[i].nombre;
        juego.Jugadores[i].mano.cabeza = nullptr;
        juego.Jugadores[i].puntos = 0;
    }
}

// Busca una ficha aleatoria
Ficha extraerFichaAleatoria(ListaFicha &lista){
    int total = contarFichas(lista);
    if(total == 0){
        cout << "Error: pozo vacío\n";
        return crearFicha(-1,-1);
    }

    int posicion = rand() % total; // indice de la lista aleatorio
    NodoFicha *actual = lista.cabeza;
    NodoFicha *anterior = nullptr;

    for(int i = 0; i < posicion; i++){
        anterior = actual;
        actual = actual -> siguiente;
    }

    // Desconecta el nodo
    if(anterior == nullptr){
        lista.cabeza = actual -> siguiente;
    }
    else{
        anterior -> siguiente = actual -> siguiente;
    }

    Ficha final = actual -> dato;
    delete actual;
    return final;
}

// Repartimos las fichas - CORREGIDO
void repartirFichas(Juego &juego){
    srand(time(0)); //inicializa el generador aleatoriamente

    for(int i = 0; i < juego.numJugadores; i++){
        for(int j = 0; j < 7; j++){  // CORREGIDO: j < 7 en lugar de j > 7
            Ficha robada = extraerFichaAleatoria(juego.pozo);
            insertarFicha(juego.Jugadores[i].mano, robada);
        }
    }
}

// Mostramos la mano de los jugadores - CORREGIDO
void mostrarJugadores(Juego juego){
    for(int i = 0; i < juego.numJugadores; i++){  // CORREGIDO: i < juego.numJugadores
        cout << juego.Jugadores[i].nombre << ": ";
        mostarLista(juego.Jugadores[i].mano);
        cout << " (Fichas: " << contarFichas(juego.Jugadores[i].mano) << ")\n";
    }
}

// Función para debug del pozo
void mostrarEstadoPozo(Juego juego){
    cout << "\n=== ESTADO DEL POZO ===" << endl;
    cout << "Fichas en pozo: " << contarFichas(juego.pozo) << endl;
    cout << "Fichas en pozo: ";
    mostarLista(juego.pozo);
    cout << "========================\n" << endl;
}

int main(){
    Juego juego;
    juego.pozo.cabeza = nullptr;

    // Inicializar semilla para números aleatorios
    srand(time(0));

    cout << "=== INICIALIZACIÓN DEL JUEGO DOMINÓ ===" << endl;
    
    generarFichas(juego.pozo); // Crea las 28 fichas
    
    cout << "\n=== GENERACIÓN DE FICHAS ===" << endl;
    mostrarEstadoPozo(juego);  // Verificar que se generaron las 28 fichas
    
    inicilizarjugadores(juego); // Pide nombres
    repartirFichas(juego); // reparte las 7 fichas
    
    cout << "\n=== DESPUÉS DEL REPARTO ===" << endl;
    mostrarEstadoPozo(juego);  // Verificar cuántas fichas quedaron
    
    // Mostrar con el nuevo formato de pirámide
    mostrarEstadoCompleto(juego);

    // Liberar memoria
    liberarLista(juego.pozo);
    for(int i = 0; i < juego.numJugadores; i++){
        liberarLista(juego.Jugadores[i].mano);
    }
    
    cout << "\n¡Juego terminado! Memoria liberada correctamente." << endl;
    return 0;
}