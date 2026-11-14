/*
Sistema de gestión aeroportuaria (AeroGestión).
Permite administrar líneas aéreas, vuelos, destinos, duración y aforo.
Gestiona reservas de pasajeros en orden de llegada (cola FIFO), cancelaciones y liberaciones de cupos.
Modela la red de aeropuertos y conexiones mediante una red de rutas ponderada.
Persistencia en archivos binarios (.dat) y corte de control agrupado por aerolínea.

objetivo: Desarrollar un sistema de gestión de vuelos, reservas de pasajeros y rutas aeroportuarias aplicando estructuras de datos dinámicas, red de rutas, algoritmos de búsqueda, ordenamiento y persistencia con corte de control.

nombre: AeroGestion.c

estructura especial:
typedef tNodoVuelo* listaVuelos;
typedef tColaReservas colaConfirmadas, colaEspera;
typedef tRedRutas redRutas;

estrategia: 
    inicializarVuelos
    inicializarRutas
    cargarDatosBinarios
    cargarRutasTexto
    menu:
        1. cargarNuevoVuelo
        2. menuOrdenarYMostrarVuelos
        3. consultarVueloPorCodigo
        4. cargarSolicitudReserva
        5. procesarCancelacionReserva
        6. menuConsultasReservas
        7. menuDeRutas
        8. corteDeControlAerolineas
    guardarDatosBinarios
    guardarRutasTexto
    liberarListaVuelos
    liberarCola[de espera]
    liberarCola[Confirmadas]


nombre de funciones:
menu();
*/

#include "Persistencia.h"

#define TITULO_MENU "\n\t\tMENU AEROGESTION\n"

/* Declaración de prototipos */
void menu(tNodoVuelo **, tColaReservas *, tColaReservas *, tRedRutas *, int *);

/* Función principal */
int main() {
    tNodoVuelo *listaVuelos;
    tColaReservas colaConfirmadas;
    tColaReservas colaEspera;
    tRedRutas redRutas;
    int idReserva = 1;
    
    inicializar(&listaVuelos, &colaConfirmadas, &colaEspera);
    inicializarRedRutas(&redRutas);
    cargarDatosBinarios(&listaVuelos, &colaConfirmadas, &colaEspera, &redRutas);
    cargarRutasTexto(&redRutas);

    menu(&listaVuelos, &colaConfirmadas, &colaEspera, &redRutas, &idReserva);

    guardarDatosBinarios(listaVuelos, colaConfirmadas, colaEspera);
    guardarRutasTexto(redRutas);
    liberarListaVuelos(&listaVuelos);
    liberarCola(&colaConfirmadas);
    liberarCola(&colaEspera);

    return 0;
}

/* Implementación de funciones */

// Gestiona el menu interactivo y coordina las operaciones del sistema
void menu(tNodoVuelo **pListaVuelos, tColaReservas *pConfirmadas, tColaReservas *pEspera, tRedRutas *pRedRutas, int *pIdReserva) {
    int opcion;

    do {
        printf(TITULO_MENU);
        printf("1. Registrar vuelo\n");
        printf("2. Mostrar y ordenar vuelos\n");
        printf("3. Buscar vuelo por codigo\n");
        printf("4. Solicitar reserva\n");
        printf("5. Cancelar reserva por DNI\n");
        printf("6. Ver cola de reservas y buscar por DNI\n");
        printf("7. Aeropuertos y rutas\n");
        printf("8. Total de areonineas y pasajeros\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");
        fflush(stdin);
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                cargarNuevoVuelo(pListaVuelos, pRedRutas);
                guardarDatosBinarios(*pListaVuelos, *pConfirmadas, *pEspera);
                break;
            case 2:
                menuOrdenarYMostrarVuelos(*pListaVuelos);
                break;
            case 3:
                consultarVueloPorCodigo(*pListaVuelos);
                break;
            case 4:
                cargarSolicitudReserva(pConfirmadas, pEspera, *pListaVuelos, pIdReserva);
                guardarDatosBinarios(*pListaVuelos, *pConfirmadas, *pEspera);
                break;
            case 5:
                procesarCancelacionReserva(pConfirmadas, pEspera, *pListaVuelos);
                guardarDatosBinarios(*pListaVuelos, *pConfirmadas, *pEspera);
                break;
            case 6:
                menuConsultasReservas(*pConfirmadas, *pEspera);
                break;
            case 7:
                menuDeRutas(pRedRutas);
                break;
            case 8:
                corteDeControlAerolineas();
                break;
            case 0:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    } while (opcion != 0);
}
