#include <iostream>
#include <cstdlib>
#include <ctime>
#include "funciones.h"
#include "utility.h"
#include "turnos.h"

using namespace std;

int main() {
    Juego miJuego;
    Mesa mesa;
    
    // Inicialización
    crearCola(miJuego.pozo);
    initMesa(mesa);
    srand(time(0));

    // Bienvenida
    cout << "🎲 BIENVENIDO AL JUEGO DE DOMINÓ 🎲" << endl;
    cout << "===================================" << endl;
    
    // Configurar jugadores (solo una vez)
    configurarJugadores(miJuego);
    
    // Iniciar la primera ronda
    cout << "\n📦 Preparando la ronda..." << endl;
    iniciarRonda(miJuego, mesa);
    
    cout << "\nPresiona ENTER para empezar...";
    cin.ignore();
    cin.get();
    
    // Jugar la ronda
    int ganador = jugarRonda(miJuego, mesa);
    
    // Mostrar resultado final
    cout << "\n" << string(60, '=') << endl;
    cout << "           FINAL DE LA RONDA" << endl;
    cout << string(60, '=') << endl;
    mostrarMesa(mesa);
    
    if (ganador != -1) {
        cout << "\n🏆 Ganador: " << miJuego.jugadores[ganador].nombre << endl;
    } else {
        cout << "\n🚫 Juego trancado" << endl;
    }
    
    // Limpiar memoria
    cout << "\n🧹 Limpiando memoria..." << endl;
    limpiarCola(miJuego.pozo);
    for(int i = 0; i < miJuego.numJugadores; i++) {
        limpiarPila(miJuego.jugadores[i].mano);
    }
    clearMesa(mesa);
    
    cout << "\n¡Gracias por jugar! Hasta la próxima 👋" << endl;
    return 0;
}