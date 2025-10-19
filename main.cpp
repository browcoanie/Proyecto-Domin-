#include <iostream>
#include "funciones.h"
#include "utility.h"
#include <cstdlib>
#include <ctime>

using namespace std;

// Muestra todas las fichas de una pila (mano de jugador)
void mostrarFichasPila(pilasFicha pila) {
    NodoFicha *actual = pila.tope;
    
    if (actual == nullptr) {
        cout << "No hay fichas";
        return;
    }
    
    while (actual != nullptr) {
        cout << "[" << actual->ficha.lado1 << "|" << actual->ficha.lado2 << "] ";
        actual = actual->siguiente;
    }
}

// Muestra todas las fichas de una cola (pozo)
void mostrarFichasCola(colaFicha cola) {
    NodoFicha *actual = cola.frente;
    
    if (actual == nullptr) {
        cout << "El pozo está vacío";
        return;
    }
    
    while (actual != nullptr) {
        cout << "[" << actual->ficha.lado1 << "|" << actual->ficha.lado2 << "] ";
        actual = actual->siguiente;
    }
}

// Muestra la mano de un jugador en forma de pirámide
void mostrarManoConEstilo(pilasFicha mano, string nombreJugador) {
    cout << "\n--- " << nombreJugador << " ---" << endl;
    
    NodoFicha* fichaActual = mano.tope;
    int fichasMostradas = 0;
    
    // Patrón de pirámide: 1, 2, 3, 1 fichas por línea
    int fichasPorLinea[] = {1, 2, 3, 1};
    
    for (int linea = 0; linea < 4 && fichaActual != nullptr; linea++) {
        int cuantasFichas = fichasPorLinea[linea];
        
        // Centramos la línea
        int espacios = (7 - cuantasFichas) * 2;
        for (int i = 0; i < espacios; i++) {
            cout << " ";
        }
        
        // Mostramos las fichas de esta línea
        for (int i = 0; i < cuantasFichas && fichaActual != nullptr; i++) {
            cout << "[" << fichaActual->ficha.lado1 << "|" << fichaActual->ficha.lado2 << "] ";
            fichaActual = fichaActual->siguiente;
            fichasMostradas++;
        }
        cout << endl;
    }
    
    // Por si acaso hay más de 7 fichas (no debería pasar)
    while (fichaActual != nullptr) {
        cout << "[" << fichaActual->ficha.lado1 << "|" << fichaActual->ficha.lado2 << "] ";
        fichaActual = fichaActual->siguiente;
    }
}

// Muestra el estado completo del juego
void mostrarTodoElJuego(Juego juego) {
    cout << "\n" << string(50, '=') << endl;
    cout << "        ESTADO ACTUAL DEL JUEGO" << endl;
    cout << string(50, '=') << endl;
    
    // Mostramos el pozo
    cout << "\nPOZO (" << contarFichasEnCola(juego.pozo) << " fichas restantes):" << endl;
    mostrarFichasCola(juego.pozo);
    cout << "\n" << endl;
    
    // Mostramos los jugadores
    cout << string(30, '-') << endl;
    cout << "    JUGADORES" << endl;
    cout << string(30, '-') << endl;
    
    for(int i = 0; i < juego.numJugadores; i++) {
        mostrarManoConEstilo(juego.jugadores[i].mano, juego.jugadores[i].nombre);
        cout << "Puntos: " << juego.jugadores[i].puntos << "\n" << endl;
    }
}

// Pide los datos de los jugadores
void configurarJugadores(Juego &juego) {
    cout << "¿Cuántos jugadores van a jugar? (2-4): ";
    cin >> juego.numJugadores;

    // Validamos que el número sea correcto
    while (juego.numJugadores < 2 || juego.numJugadores > 4) {
        cout << "Tiene que ser entre 2 y 4 jugadores. Intenta de nuevo: ";
        cin >> juego.numJugadores;
    }

    // Pedimos los nombres de cada jugador
    for(int i = 0; i < juego.numJugadores; i++) {
        cout << "Nombre del jugador " << (i + 1) << ": ";
        cin >> juego.jugadores[i].nombre;
        crearPila(juego.jugadores[i].mano);  // Inicializamos su mano vacía
        juego.jugadores[i].puntos = 0;
    }
}

// Reparte 7 fichas a cada jugador
void repartirFichas(Juego &juego) {
    srand(time(0));  // Para que sea aleatorio cada vez
    
    cout << "\nRepartiendo fichas..." << endl;
    
    for(int jugador = 0; jugador < juego.numJugadores; jugador++) {
        for(int ficha = 0; ficha < 7; ficha++) {
            Ficha fichaRobada = sacarFichaCola(juego.pozo);
            insertarFichaPila(juego.jugadores[jugador].mano, fichaRobada);
        }
    }
    
    cout << "¡Fichas repartidas! Cada jugador tiene 7 fichas." << endl;
}

// Muestra información del pozo (útil para debug)
void verEstadoDelPozo(Juego juego) {
    cout << "\n*** INFORMACIÓN DEL POZO ***" << endl;
    cout << "Fichas en el pozo: " << contarFichasEnCola(juego.pozo) << endl;
    cout << "Fichas: ";
    mostrarFichasCola(juego.pozo);
    cout << "\n****************************\n" << endl;
}

void showMesa(const Mesa &mesa) {
    if (mesa.left == nullptr) {
        cout << "Mesa is empty" << endl;
        return;
    }
    mesaNode* current = mesa.left;
    while (current != nullptr) {
        cout << "[" << current->ficha.lado1 << "|" << current->ficha.lado2 << "] ";
        current = current->next;
    }
    cout << endl;
}

// Muestra la mano enumerando desde el tope (índice 0)
void mostrarManoEnumerada(const pilasFicha &mano) {
    NodoFicha* actual = mano.tope;
    int idx = 0;
    if (actual == nullptr) {
        cout << "(mano vacía)\n";
        return;
    }
    while (actual != nullptr) {
        cout << idx << ": [" << actual->ficha.lado1 << "|" << actual->ficha.lado2 << "]  ";
        actual = actual->siguiente;
        idx++;
    }
    cout << endl;
}

// Lógica de partida: rotar jugadores, permitir pasar o robar del pozo y jugar el tope
void jugarPartida(Juego &juego) {
    Mesa mesa;
    initMesa(mesa);

    int current = 0;
    int consecutivosPasan = 0;

    while (true) {
        Jugador &jug = juego.jugadores[current];
        cout << "\n--- Turno de " << jug.nombre << " ---\n";
        cout << "Pozo: " << contarFichasEnCola(juego.pozo) << " fichas\n";
        cout << "Mesa: ";
        showMesa(mesa);

        cout << "Tu mano:\n";
        mostrarManoEnumerada(jug.mano);

        cout << "Elige acción: (J)ugar tope, (D)robar, (P)asar : ";
        char accion;
        cin >> accion;

        if (accion == 'J' || accion == 'j') {
            if (pilaVacia(jug.mano)) {
                cout << "No tienes fichas para jugar.\n";
                // no cambia consecutivosPasan, el turno termina
            } else {
                Ficha tope = jug.mano.tope->ficha;
                bool puedeIzq = false, puedeDer = false;
                if (mesa.left == nullptr) {
                    puedeIzq = puedeDer = true;
                } else {
                    int vIzq = valueLeft(mesa);
                    int vDer = valueRight(mesa);
                    if (tope.lado2 == vIzq || tope.lado1 == vIzq) puedeIzq = true;
                    if (tope.lado1 == vDer || tope.lado2 == vDer) puedeDer = true;
                }

                if (!puedeIzq && !puedeDer) {
                    cout << "La ficha superior no puede colocarse en ningún lado.\n";
                } else {
                    cout << "Puedes colocar en: " << (puedeIzq ? "Izquierda " : "") << (puedeDer ? "Derecha" : "") << "\nSelecciona lado (I/D): ";
                    char lado; cin >> lado;
                    if ((lado == 'I' || lado == 'i') && puedeIzq) {
                        Ficha f = sacarFichaPila(jug.mano);
                        if (!placeLeft(mesa, f)) {
                            // si fallara (caso raro), devolver la ficha al tope
                            insertarFichaPila(jug.mano, f);
                            cout << "Error al colocar a la izquierda. Ficha devuelta.\n";
                        } else {
                            cout << "Ficha colocada a la izquierda.\n";
                            consecutivosPasan = 0;
                        }
                    } else if ((lado == 'D' || lado == 'd') && puedeDer) {
                        Ficha f = sacarFichaPila(jug.mano);
                        if (!placeRight(mesa, f)) {
                            insertarFichaPila(jug.mano, f);
                            cout << "Error al colocar a la derecha. Ficha devuelta.\n";
                        } else {
                            cout << "Ficha colocada a la derecha.\n";
                            consecutivosPasan = 0;
                        }
                    } else {
                        cout << "Lado inválido o no permitido. No se jugó.\n";
                    }
                }

                if (contarFichasEnPila(jug.mano) == 0) {
                    cout << "\n¡" << jug.nombre << " se quedó sin fichas y gana la partida!\n";
                    break;
                }
            }
        } else if (accion == 'D' || accion == 'd') {
            Ficha robada = sacarFichaCola(juego.pozo);
            if (robada.lado1 == -1 && robada.lado2 == -1) {
                cout << "El pozo está vacío. No puedes robar.\n";
            } else {
                insertarFichaPila(jug.mano, robada);
                cout << "Has robado [" << robada.lado1 << "|" << robada.lado2 << "].\n";
                // robar cuenta como acción que rompe la secuencia de pases
                consecutivosPasan = 0;
            }
        } else if (accion == 'P' || accion == 'p') {
            cout << jug.nombre << " pasa su turno.\n";
            consecutivosPasan++;
        } else {
            cout << "Opción inválida.\n";
            // no avanzar el contador de pases de forma automática
        }

        // Fin de ronda bloqueada: todos pasaron y pozo vacío
        if (consecutivosPasan >= juego.numJugadores && contarFichasEnCola(juego.pozo) == 0) {
            cout << "\nTodos los jugadores pasaron y el pozo está vacío. Fin de la ronda.\n";
            break;
        }

        // Rotar al siguiente jugador
        current = (current + 1) % juego.numJugadores;
    }

    // Estado final de la mesa y manos
    cout << "\nEstado final de la mesa:\n";
    showMesa(mesa);
    cout << "\nManos finales:\n";
    for (int i = 0; i < juego.numJugadores; ++i) {
        cout << juego.jugadores[i].nombre << ": ";
        mostrarManoEnumerada(juego.jugadores[i].mano);
    }

    clearMesa(mesa);
}

// Función principal
int main() {
    Juego miJuego;
    
    // Inicializamos el pozo vacío
    crearCola(miJuego.pozo);
    
    // Para que los números aleatorios funcionen bien
    srand(time(0));

    cout << "🎲 BIENVENIDO AL JUEGO DE DOMINÓ 🎲" << endl;
    cout << "===================================" << endl;
    
    // Paso 1: Generar todas las fichas
    cout << "\nGenerando las 28 fichas del dominó..." << endl;
    generarTodasLasFichas(miJuego.pozo);
    
    // 🆕 Paso 2: Mezclar las fichas
    cout << "Mezclando las fichas..." << endl;
    mezclarPozo(miJuego.pozo);
    verEstadoDelPozo(miJuego);
    
    // Paso 3: Configurar jugadores
    configurarJugadores(miJuego);
    
    // Paso 4: Repartir fichas
    repartirFichas(miJuego);
    verEstadoDelPozo(miJuego);
    
    // Paso 5: Mostrar el estado completo
    mostrarTodoElJuego(miJuego);

    cout << "\nLimpiando memoria..." << endl;
    limpiarCola(miJuego.pozo);
    for(int i = 0; i < miJuego.numJugadores; i++) {
        limpiarPila(miJuego.jugadores[i].mano);
    }
    
    cout << "\n¡Gracias por jugar! Hasta la próxima 👋" << endl;
    return 0;
}