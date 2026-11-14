/*
Módulo de Gestión de Tipos Abstractos de Datos (TADs) Dinámicos.
Contiene las estructuras autorreferenciadas para la administración de vuelos (lista enlazada dinámica) y reservas de pasajeros (cola FIFO).
nota:
- Utiliza memoria dinámica garantizando inserción al final y liberación completa de memoria al cerrar para evitar desbordamiento.

objetivo: Implementar y encapsular las operaciones primitivas de listas enlazadas para vuelos y colas para reservas.

nombre: Gestion.h

estructura especial:

typedef char sstring[MAX];
typedef struct {
    sstring codigoVuelo;
    sstring aerolinea;
    int idOrigen;
    int idDestino;
    sstring destino;
    float duracion;
    int capacidadMax;
    int ocupados;
} tVuelo;

typedef struct nodoVuelo {
    tVuelo datos;
    struct nodoVuelo *siguiente;
} tNodoVuelo;
typedef struct {
    int idReserva;
    int dni;
    sstring nombrePasajero;
    sstring codigoVuelo;
    bool confirmada;
} tReserva;

typedef struct nodoReserva {
    tReserva datos;
    struct nodoReserva *siguiente;
} tNodoReserva;

typedef struct {
    tNodoReserva *principio;
    tNodoReserva *final;
} tColaReservas;

estrategia: 
    inicializar
    listaVuelosVacia
    insertarVuelo
    mostrarVuelos
    liberarListaVuelos
    colaVacia
    encolarReserva
    desencolarReserva
    mostrarColaReservas
    liberarCola

nombre de funciones:
    inicializar();
    listaVuelosVacia();
    insertarVuelo();
    mostrarVuelos();
    liberarListaVuelos();
    colaVacia();
    encolarReserva();
    desencolarReserva();
    mostrarColaReservas();
    liberarCola();
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 50
#define TITULO_VUELOS "\nCODIGO\t\tAEROLINEA\t\tDESTINO\t\tDURACION\tOCUPACION\n"
#define TITULO_RESERVAS "ID\tDNI\t\tPASAJERO\t\tVUELO\t\tESTADO\n"

/* Declaración de tipos de datos personalizados */

typedef char sstring[MAX];

/*Declaracion de tados de vuelos*/
typedef struct {
    sstring codigoVuelo;
    sstring aerolinea;
    int idOrigen;
    int idDestino;
    sstring destino;
    float duracion;
    int capacidadMax;
    int ocupados;
} tVuelo;

typedef struct nodoVuelo {
    tVuelo datos;
    struct nodoVuelo *siguiente;
} tNodoVuelo;

/*Reservas de Pasajeros*/
typedef struct {
    int idReserva;
    int dni;
    sstring nombrePasajero;
    sstring codigoVuelo;
    bool confirmada;
} tReserva;
//nodo de reservas
typedef struct nodoReserva {
    tReserva datos;
    struct nodoReserva *siguiente;
} tNodoReserva;
//colas
typedef struct {
    tNodoReserva *principio;
    tNodoReserva *final;
} tColaReservas;

/* Declaración de prototipos */
void inicializar(tNodoVuelo **, tColaReservas *, tColaReservas *);
bool listaVuelosVacia(tNodoVuelo *);
void insertarVuelo(tNodoVuelo **, tVuelo);
void mostrarVuelos(tNodoVuelo *);
void liberarListaVuelos(tNodoVuelo **);

bool colaVacia(tColaReservas);
void encolarReserva(tColaReservas *, tReserva);
void desencolarReserva(tColaReservas *, tReserva *);
void mostrarColaReservas(tColaReservas);
void liberarCola(tColaReservas *);

/* Implementación de funciones */

// Inicializa en NULL la lista enlazada de vuelos y las colas de reservas
void inicializar(tNodoVuelo **pLista, tColaReservas *pConfirmadas, tColaReservas *pEspera) {
    *pLista = NULL;
    pConfirmadas->principio = NULL;
    pConfirmadas->final = NULL;
    pEspera->principio = NULL;
    pEspera->final = NULL;
}

// Comprueba si la lista de vuelos no tiene elementos
bool listaVuelosVacia(tNodoVuelo *pLista) {
    return (pLista == NULL);
}

// Inserta un nuevo nodo de vuelo al final de la lista
void insertarVuelo(tNodoVuelo **pLista, tVuelo pVuelo) {
    tNodoVuelo *nuevo = (tNodoVuelo *)malloc(sizeof(tNodoVuelo));
    if (nuevo == NULL) {
        return;
    }
    nuevo->datos = pVuelo;
    nuevo->siguiente = NULL;

    if (listaVuelosVacia(*pLista)) {
        *pLista = nuevo;
    } else {
        tNodoVuelo *actual = *pLista;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
}

// Muestra en formato tabular todos los vuelos registrados en la lista
void mostrarVuelos(tNodoVuelo *pLista) {
    tNodoVuelo *aux = pLista;
    if (listaVuelosVacia(pLista)) {
        printf("No hay vuelos registrados.\n");
        return;
    }
    printf(TITULO_VUELOS);

    while (aux != NULL) {
        printf("%-s\t%-s\t%-s\t%.0f min\t\t%d/%d\n",
                aux->datos.codigoVuelo,
                aux->datos.aerolinea,
                aux->datos.destino,
                aux->datos.duracion,
                aux->datos.ocupados,
                aux->datos.capacidadMax);
            aux = aux->siguiente;
    }
}

// Libera la memoria dinamica de todos los nodos de la lista de vuelos
void liberarListaVuelos(tNodoVuelo **pLista) {
    tNodoVuelo *aux;
    while (*pLista != NULL) {
        aux = *pLista;
        *pLista = (*pLista)->siguiente;
        free(aux);
    }
}

// Comprueba si la cola de reservas no tiene elementos
bool colaVacia(tColaReservas pCola) {
    return (pCola.principio == NULL);
}

// Agrega una solicitud de reserva al final de la cola FIFO
void encolarReserva(tColaReservas *pCola, tReserva pDatos) {
    tNodoReserva *nuevo = (tNodoReserva *)malloc(sizeof(tNodoReserva));
    if (nuevo == NULL) {
        return;
    }
    nuevo->datos = pDatos;
    nuevo->siguiente = NULL;

    if (colaVacia(*pCola)) {
        pCola->principio = nuevo;
        pCola->final = nuevo;
    } else {
        pCola->final->siguiente = nuevo;
        pCola->final = nuevo;
    }
}

// Extrae el primer elemento al frente de la cola
void desencolarReserva(tColaReservas *pCola, tReserva *pSalida) {
    if (colaVacia(*pCola)) {
        return;
    }
    tNodoReserva *aux = pCola->principio;
    if (pSalida != NULL) {
        *pSalida = aux->datos;
    }
    pCola->principio = aux->siguiente;
    if (pCola->principio == NULL) {
        pCola->final = NULL;
    }
    free(aux);
}

// Muestra los datos de todos los pasajeros en la cola de reservas
void mostrarColaReservas(tColaReservas pCola) {
    if (colaVacia(pCola)) {
        printf("La cola de reservas esta vacia.\n");
        return;
    }
    printf(TITULO_RESERVAS);
    tNodoReserva *aux = pCola.principio;
    while (aux != NULL) {
        printf("%d\t%d\t%-s\t%-s\t%s\n",
               aux->datos.idReserva,
               aux->datos.dni,
               aux->datos.nombrePasajero,
               aux->datos.codigoVuelo,
               aux->datos.confirmada ? "CONFIRMADA" : "EN ESPERA");
        aux = aux->siguiente;
    }
}

// Vacia la cola y libera la memoria de todos sus nodos
void liberarCola(tColaReservas *pCola) {
    while (!colaVacia(*pCola)) {
        desencolarReserva(pCola, NULL);
    }
}
