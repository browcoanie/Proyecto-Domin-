#ifndef TURNOS_H
#define TURNOS_H

#include "funciones.h"
#include "utility.h"
#include <iostream>
using namespace std;

// FUNCIONES DE VISUALIZACIÓN


// Muestra todas las fichas de una pila con números
void mostrarFichasPila(pilasFicha pila) {
    NodoFicha *actual = pila.tope;
    int contador = 1;
    
    if (actual == nullptr) {
        cout << "No hay fichas";
        return;
    }
    
    while (actual != nullptr) {
        cout << contador << ".[" << actual->ficha.lado1 << "|" << actual->ficha.lado2 << "] ";
        actual = actual->siguiente;
        contador++;
    }
}

// Muestra la mesa de forma visual
void mostrarMesa(const Mesa &mesa) {
    if (mesa.left == nullptr) {
        cout << "[MESA VACÍA]" << endl;
        return;
    }
    
    cout << "MESA: ";
    mesaNode* current = mesa.left;
    while (current != nullptr) {
        cout << "[" << current->ficha.lado1 << "|" << current->ficha.lado2 << "]";
        if (current->next != nullptr) cout << "-";
        current = current->next;
    }
    cout << "  (Izq:" << valueLeft(mesa) << " | Der:" << valueRight(mesa) << ")" << endl;
}


// FUNCIONES AUXILIARES DE JUEGO

// Obtiene una ficha de la mano por su posición
Ficha obtenerFichaPorPosicion(pilasFicha &mano, int posicion) {
    NodoFicha *actual = mano.tope;
    int contador = 1;
    
    while (actual != nullptr && contador < posicion) {
        actual = actual->siguiente;
        contador++;
    }
    
    if (actual == nullptr) {
        Ficha fichaInvalida = {-1, -1};
        return fichaInvalida;
    }
    
    Ficha fichaSeleccionada = actual->ficha;
    
    // Ahora la sacamos de la pila
    sacarFichaEspecifica(mano, fichaSeleccionada.lado1, fichaSeleccionada.lado2, fichaSeleccionada);
    
    return fichaSeleccionada;
}


// FUNCIÓN PRINCIPAL DE TURNO


// Ejecuta el turno de un jugador
// Retorna true si el jugador ganó la ronda
bool jugarTurno(Juego &juego, Mesa &mesa) {
    Jugador &jugadorActual = juego.jugadores[juego.turnoActual];
    
    cout << "\n" << string(60, '=') << endl;
    cout << "🎯 TURNO DE: " << jugadorActual.nombre << endl;
    cout << string(60, '=') << endl;
    
    // Mostramos el estado actual
    mostrarMesa(mesa);
    cout << "\n💼 Tu mano: ";
    mostrarFichasPila(jugadorActual.mano);
    cout << "\n🎲 Fichas en el pozo: " << contarFichasEnCola(juego.pozo) << endl;
    
    int valorIzq = valueLeft(mesa);
    int valorDer = valueRight(mesa);
    
    // Verificamos si tiene jugadas válidas
    bool tieneJugadas = tieneJugadaValida(jugadorActual.mano, valorIzq, valorDer);
    
    if (!tieneJugadas) {
        cout << "\n❌ No tienes jugadas válidas." << endl;
        
        // Intentar robar del pozo
        if (!colaVacia(juego.pozo)) {
            cout << "🎴 Robando del pozo..." << endl;
            if (robarDelPozo(juego, juego.turnoActual)) {
                cout << "✅ Robaste: [" << verTopePila(jugadorActual.mano).lado1 
                     << "|" << verTopePila(jugadorActual.mano).lado2 << "]" << endl;
                
                // Volver a verificar si puede jugar
                tieneJugadas = tieneJugadaValida(jugadorActual.mano, valorIzq, valorDer);
                if (!tieneJugadas) {
                    cout << "❌ Aún no puedes jugar. Pasas turno." << endl;
                    jugadorActual.paso = true;
                    return false;
                }
            }
        } else {
            cout << "🚫 No hay fichas en el pozo. Pasas turno." << endl;
            jugadorActual.paso = true;
            return false;
        }
    }
    
    // El jugador puede jugar
    jugadorActual.paso = false;
    
    cout << "\n¿Qué quieres hacer?" << endl;
    cout << "1. Jugar una ficha" << endl;
    cout << "2. Robar del pozo (si hay)" << endl;
    
    int opcion;
    cin >> opcion;
    
    if (opcion == 2) {
        if (robarDelPozo(juego, juego.turnoActual)) {
            cout << "✅ Robaste: [" << verTopePila(jugadorActual.mano).lado1 
                 << "|" << verTopePila(jugadorActual.mano).lado2 << "]" << endl;
        } else {
            cout << "❌ No hay fichas en el pozo." << endl;
        }
        return false;
    }
    
    // Jugar una ficha
    cout << "\n¿Qué ficha quieres jugar? (número): ";
    int numFicha;
    cin >> numFicha;
    
    Ficha fichaAJugar = obtenerFichaPorPosicion(jugadorActual.mano, numFicha);
    
    if (fichaAJugar.lado1 == -1) {
        cout << "❌ Ficha inválida. Pierdes el turno." << endl;
        return false;
    }
    
    // Verificar si puede jugar esa ficha
    if (!puedeJugarFicha(fichaAJugar, valorIzq, valorDer)) {
        cout << "❌ Esa ficha no se puede jugar. Pierdes el turno." << endl;
        // Devolver la ficha a la mano
        insertarFichaPila(jugadorActual.mano, fichaAJugar);
        return false;
    }
    
    // Preguntar dónde quiere jugarla
    char lado;
    if (valorIzq == -1 && valorDer == -1) {
        // Primera ficha, no importa el lado
        placeLeft(mesa, fichaAJugar);
        cout << "✅ ¡Ficha colocada!" << endl;
    } else {
        cout << "¿Dónde quieres colocarla? (I)zquierda o (D)erecha: ";
        cin >> lado;
        
        bool colocada = false;
        if (lado == 'I' || lado == 'i') {
            colocada = placeLeft(mesa, fichaAJugar);
        } else if (lado == 'D' || lado == 'd') {
            colocada = placeRight(mesa, fichaAJugar);
        }
        
        if (colocada) {
            cout << "✅ ¡Ficha colocada!" << endl;
        } else {
            cout << "❌ No se pudo colocar la ficha. Pierdes el turno." << endl;
            insertarFichaPila(jugadorActual.mano, fichaAJugar);
            return false;
        }
    }
    
    // Verificar si el jugador ganó
    if (pilaVacia(jugadorActual.mano)) {
        cout << "\n🎉🎉🎉 ¡" << jugadorActual.nombre << " GANÓ LA RONDA! 🎉🎉🎉" << endl;
        return true; // Hay un ganador
    }
    
    return false;
}


// FUNCIONES DE CONFIGURACIÓN DE JUEGO


// Pide los datos de los jugadores
void configurarJugadores(Juego &juego) {
    cout << "¿Cuántos jugadores van a jugar? (2-4): ";
    cin >> juego.numJugadores;

    while (juego.numJugadores < 2 || juego.numJugadores > 4) {
        cout << "Tiene que ser entre 2 y 4 jugadores. Intenta de nuevo: ";
        cin >> juego.numJugadores;
    }

    for(int i = 0; i < juego.numJugadores; i++) {
        cout << "Nombre del jugador " << (i + 1) << ": ";
        cin >> juego.jugadores[i].nombre;
        crearPila(juego.jugadores[i].mano);
        juego.jugadores[i].puntos = 0;
        juego.jugadores[i].paso = false;
    }
}

// Reparte 7 fichas a cada jugador
void repartirFichas(Juego &juego) {
    cout << "\n🎴 Repartiendo fichas..." << endl;
    
    for(int jugador = 0; jugador < juego.numJugadores; jugador++) {
        for(int ficha = 0; ficha < 7; ficha++) {
            Ficha fichaRobada = sacarFichaCola(juego.pozo);
            insertarFichaPila(juego.jugadores[jugador].mano, fichaRobada);
        }
    }
    
    cout << "✅ ¡Fichas repartidas! Cada jugador tiene 7 fichas." << endl;
}

// FUNCIONES DE CONTROL DE FLUJO


// Inicializa una nueva ronda
void iniciarRonda(Juego &juego, Mesa &mesa) {
    // Limpiar el pozo y la mesa
    limpiarCola(juego.pozo);
    clearMesa(mesa);
    
    // Limpiar las manos de los jugadores
    for(int i = 0; i < juego.numJugadores; i++) {
        limpiarPila(juego.jugadores[i].mano);
        juego.jugadores[i].paso = false;
    }
    
    // Generar y mezclar fichas
    generarTodasLasFichas(juego.pozo);
    mezclarPozo(juego.pozo);
    
    // Repartir fichas
    repartirFichas(juego);
    
    // Determinar quién empieza
    int fichaDobleInicial = -1;
    juego.turnoActual = determinarQuienEmpieza(juego, fichaDobleInicial);
    cout << "\n🎮 Empieza: " << juego.jugadores[juego.turnoActual].nombre << endl;
    if (fichaDobleInicial >= 0) {
        cout << "💎 Tiene la ficha doble [" << fichaDobleInicial << "|" << fichaDobleInicial << "]" << endl;
    } else {
        cout << "(No hay fichas dobles, se eligió al azar)" << endl;
    }
}

// Ejecuta una ronda completa
// Retorna el índice del ganador, o -1 si hay tranca
int jugarRonda(Juego &juego, Mesa &mesa) {
    bool hayGanador = false;
    int turnosSinJugar = 0;
    
    while (!hayGanador) {
        hayGanador = jugarTurno(juego, mesa);
        
        if (!hayGanador) {
            // Verificar si todos pasaron (juego trancado)
            if (juego.jugadores[juego.turnoActual].paso) {
                turnosSinJugar++;
                if (turnosSinJugar >= juego.numJugadores) {
                    cout << "\n🚫 ¡JUEGO TRANCADO! Nadie puede jugar." << endl;
                    return -1; // Tranca
                }
            } else {
                turnosSinJugar = 0;
            }
            
            siguienteTurno(juego);
            
            cout << "\nPresiona ENTER para continuar...";
            cin.ignore();
            cin.get();
        } else {
            return juego.turnoActual; // Retorna el índice del ganador
        }
    }
    
    return -1;
}

#endif 