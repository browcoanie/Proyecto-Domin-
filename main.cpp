#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "funciones.h"
#include "utility.h"
#include "turnos.h"

// Aqui si se puede usar 'using namespace std;' porque es un .cpp
using namespace std;

// la funcion limpiarConsola se movio a utility.h

int main() {
    // 1. crear estructuras
    Juego miJuego;
    Mesa mesa;
    
    // 2. inicializar
    crearPila(miJuego.pozo);
    initMesa(mesa);
    
    // semilla para el rand(). Si no, el juego seria igual cada vez
    srand(time(0)); 

    // 3. Bienvenida
    cout << "🎲 BIENVENIDO AL JUEGO DE DOMINÓ 🎲" << endl;
    cout << "===================================" << endl;
    
    // 4. Configurar jugadores (con validacion)
    configurarJugadores(miJuego);
    
    // 5. Configurar puntaje (con validacion)
    int puntajeObjetivo;
    int opcion;
    bool opcionValida = false;

    // bucle de validacion
    while (!opcionValida) {
        cout << "\n¿A cuántos puntos quieren jugar?" << endl;
        cout << "  1. 50 puntos (partida rápida)" << endl;
        cout << "  2. 100 puntos (partida normal)" << endl;
        cout << "  3. 200 puntos (partida larga)" << endl;
        cout << "  4. Personalizado" << endl;
        cout << "Elige una opción (1-4): ";

        // revisa si es numero
        if (!(cin >> opcion)) {
            cout << "❌ Error: Debes ingresar un NÚMERO." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue; // vuelve a preguntar
        }

        // revisa el rango (1-4)
        switch (opcion) {
            case 1:
                puntajeObjetivo = 50;
                opcionValida = true; // valido, salimos
                break;
            case 2:
                puntajeObjetivo = 100;
                opcionValida = true;
                break;
            case 3:
                puntajeObjetivo = 200;
                opcionValida = true;
                break;
            case 4: // caso 4 (personalizado)
                cout << "Ingresa el puntaje objetivo: ";
                if (!(cin >> puntajeObjetivo)) {
                    cout << "❌ Error: Debes ingresar un NÚMERO." << endl;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    // 'opcionValida' sigue false, repite el menu
                } else if (puntajeObjetivo <= 0) {
                    cout << "❌ El puntaje debe ser mayor a 0." << endl;
                } else {
                    opcionValida = true; // valido, salimos
                }
                break;
            default: // si pone 5, -1, etc
                cout << "❌ Opción inválida. Por favor, elige un número del 1 al 4." << endl;
        }
    }
    
    limpiarConsola(); // limpiamos antes de empezar
    
    // 6. la funcion que hace todo
    jugarPartidaCompleta(miJuego, mesa, puntajeObjetivo);
    
    // 7. IMPORTANTE: Limpiar memoria (delete)
    cout << "\n🧹 Limpiando memoria..." << endl;
    limpiarPila(miJuego.pozo);
    for(int i = 0; i < miJuego.numJugadores; i++) {
        limpiarPila(miJuego.jugadores[i].mano); // limpiar manos
    }
    clearMesa(mesa); // limpiar mesa
    
    cout << "\n¡Gracias por jugar! Hasta la próxima 👋" << endl;
    return 0;
}
