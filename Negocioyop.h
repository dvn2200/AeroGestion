/*
Módulo de Lógica de Negocio y Algoritmos Operativos.
Integra las estructuras dinámicas con las reglas de negocio del aeropuerto: búsqueda de vuelos, ordenamiento multicriterio, solicitud de reservas, cancelaciones y reasignación de vacantes.
nota:
- Aplica ordenamiento burbuja sobre listas enlazadas, búsqueda binaria sobre arreglos dinámicos y cancelación de reservas respetando el orden mediante cola auxiliar.

objetivo: Coordinar los servicios operativos de reservas, búsquedas y ordenamientos del sistema aeroportuario.

nombre: Negocioyop.h

estructura especial:
tNodoVuelo *pListaVuelos;
tColaReservas pConfirmadas, pEspera;
tRedRutas *pRedRutas;

estrategia: 
    buscarVueloPorCodigo
    debeIntercambiarVuelo
    ordenarVuelos
    busquedaBinariaVuelo
    registrarVuelo
    solicitarReserva
    extraerReservaDeCola
    buscarPorDniEnCola
    cargarNuevoVuelo
    menuOrdenarYMostrarVuelos
    consultarVueloPorCodigo
    cargarSolicitudReserva
    procesarCancelacionReserva
    mostrarReservasPorDni
    menuConsultasReservas

nombre de funciones:
    buscarVueloPorCodigo();
    debeIntercambiarVuelo();
    ordenarVuelos();
    busquedaBinariaVuelo();
    registrarVuelo();
    solicitarReserva();
    extraerReservaDeCola();
    buscarPorDniEnCola();
    cargarNuevoVuelo();
    menuOrdenarYMostrarVuelos();
    consultarVueloPorCodigo();
    cargarSolicitudReserva();
    procesarCancelacionReserva();
    mostrarReservasPorDni();
    menuConsultasReservas();
*/

/*librerias*/
#include "topologia.h"

#define TITULO_CONFIRMADAS "\n--- RESERVAS CONFIRMADAS ---\n"
#define TITULO_ESPERA "\n--- COLA DE ESPERA ---\n"

/* Declaración de prototipos */
tNodoVuelo *buscarVueloPorCodigo(tNodoVuelo *, sstring);
bool debeIntercambiarVuelo(tVuelo, tVuelo, int);
void ordenarVuelos(tNodoVuelo *, int);
bool busquedaBinariaVuelo(tNodoVuelo *, sstring);
bool registrarVuelo(tNodoVuelo **, tRedRutas *, tVuelo, int);
int solicitarReserva(tColaReservas *, tColaReservas *, tNodoVuelo *, tReserva);
bool extraerReservaDeCola(tColaReservas *, int, sstring, tReserva *);
bool buscarPorDniEnCola(tColaReservas, int);
void cargarNuevoVuelo(tNodoVuelo **, tRedRutas *);
void menuOrdenarYMostrarVuelos(tNodoVuelo *);
void consultarVueloPorCodigo(tNodoVuelo *);
void cargarSolicitudReserva(tColaReservas *, tColaReservas *, tNodoVuelo *, int *);
void procesarCancelacionReserva(tColaReservas *, tColaReservas *, tNodoVuelo *);
void mostrarReservasPorDni(tColaReservas, tColaReservas);
void menuConsultasReservas(tColaReservas, tColaReservas);

/* Implementación de funciones */

// Busqueda secuencial de un vuelo en la lista segun su codigo
tNodoVuelo *buscarVueloPorCodigo(tNodoVuelo *pLista, sstring pCodigo) {
    tNodoVuelo *aux = pLista;
    while (aux != NULL && strcmp(aux->datos.codigoVuelo, pCodigo) != 0) {
        aux = aux->siguiente;
    }
    return aux;
}

// Evalua el criterio de ordenamiento seleccionado entre dos vuelos
bool debeIntercambiarVuelo(tVuelo pVueloA, tVuelo pVueloB, int pCriterio) {
    switch (pCriterio) {
        case 1: 
            return strcasecmp(pVueloA.codigoVuelo, pVueloB.codigoVuelo) > 0;
        case 2: 
            return strcasecmp(pVueloA.aerolinea, pVueloB.aerolinea) > 0;
        case 3: 
            return strcasecmp(pVueloA.destino, pVueloB.destino) > 0;
        case 4: 
            return pVueloA.duracion > pVueloB.duracion;
        default: 
            return false;
    }
}

// Ordena la lista de vuelos mediante el metodo burbuja
void ordenarVuelos(tNodoVuelo *pLista, int pCriterio) {
    if (listaVuelosVacia(pLista)) {
        return;
    }
    bool cambio;
    tNodoVuelo *actual;

    do {
        cambio = false;
        actual = pLista;
        while (actual->siguiente != NULL) {
            if (debeIntercambiarVuelo(actual->datos, actual->siguiente->datos, pCriterio)) {
                tVuelo temp = actual->datos;
                actual->datos = actual->siguiente->datos;
                actual->siguiente->datos = temp;
                cambio = true;
            }
            actual = actual->siguiente;
        }
    } while (cambio);
}

// Aplica busqueda binaria sobre un vector ordenado de codigos de vuelo
bool busquedaBinariaVuelo(tNodoVuelo *pLista, sstring pCodigo) {
    if (listaVuelosVacia(pLista)) {
        return false;
    }

    ordenarVuelos(pLista, 1);

    int cant = 0;
    int i;
    
    tNodoVuelo *aux = pLista;
    while (aux != NULL) {
        cant++;
        aux = aux->siguiente;
    }

    tVuelo *vector = (tVuelo *)malloc(sizeof(tVuelo) * cant);
    if (vector == NULL) {
        return false;
    }

    aux = pLista;

    for (i = 0; i < cant; i++) {
        vector[i] = aux->datos;
        aux = aux->siguiente;
    }

    int extInf = 0, extSup = cant - 1, posMedio;
    bool encontrado = false;

    while (!encontrado && (extSup >= extInf)) {
        posMedio = (extInf + extSup) / 2;
        int cmp = strcasecmp(pCodigo, vector[posMedio].codigoVuelo);
        if (cmp == 0) {
            encontrado = true;
        } else if (cmp > 0) {
            extInf = posMedio + 1;
        } else {
            extSup = posMedio - 1;
        }
    }

    free(vector);
    return encontrado;
}

// Inserta un vuelo en la lista y actualiza los destinos en la red de rutas
bool registrarVuelo(tNodoVuelo **pLista, tRedRutas *pRedRutas, tVuelo pVuelo, int pDistanciaKm) {
    if (pVuelo.capacidadMax <= 0 || buscarVueloPorCodigo(*pLista, pVuelo.codigoVuelo) != NULL) {
        return false;
    }
    agregarAeropuerto(pRedRutas, pVuelo.idOrigen, "Origen");
    agregarAeropuerto(pRedRutas, pVuelo.idDestino, pVuelo.destino);
    if (pDistanciaKm > 0) {
        tAristaRuta ruta = {pVuelo.idOrigen, pVuelo.idDestino, {pDistanciaKm, (int)pVuelo.duracion}};
        agregarRuta(pRedRutas, ruta);
    }
    insertarVuelo(pLista, pVuelo);
    return true;
}

// Asigna una reserva a confirmadas o a lista de espera segun cupo del vuelo
int solicitarReserva(tColaReservas *pConfirmadas, tColaReservas *pEspera, tNodoVuelo *pListaVuelos, tReserva pReserva) {
    tNodoVuelo *vuelo = buscarVueloPorCodigo(pListaVuelos, pReserva.codigoVuelo);
    if (vuelo == NULL) {
        return 0;
    }
    if (vuelo->datos.ocupados < vuelo->datos.capacidadMax) {
        vuelo->datos.ocupados++;
        pReserva.confirmada = true;
        encolarReserva(pConfirmadas, pReserva);
        return 1;
    }
    pReserva.confirmada = false;
    encolarReserva(pEspera, pReserva);
    return 2;
}

// Extrae un pasajero especifico usando una cola auxiliar bajo reglas FIFO
bool extraerReservaDeCola(tColaReservas *pCola, int pDni, sstring pCodigoVuelo, tReserva *pEliminada) {
    tColaReservas colaAux;
    colaAux.principio = NULL;
    colaAux.final = NULL;
    bool encontrado = false;
    tReserva elem;

    while (!colaVacia(*pCola)) {
        desencolarReserva(pCola, &elem);
        if (!encontrado && (pDni == 0 || elem.dni == pDni) && strcasecmp(elem.codigoVuelo, pCodigoVuelo) == 0) {
            *pEliminada = elem;
            encontrado = true;
        } else {
            encolarReserva(&colaAux, elem);
        }
    }
    *pCola = colaAux;
    return encontrado;
}

// Recorre y visualiza las reservas asociadas a un DNI en la cola
bool buscarPorDniEnCola(tColaReservas pCola, int pDni) {
    bool encontrado = false;
    tNodoReserva *aux = pCola.principio;
    while (aux != NULL) {
        if (aux->datos.dni == pDni) {
            printf("[%s] ID: %d | Pasajero: %s | Vuelo: %s\n",
                   aux->datos.confirmada ? "CONFIRMADA" : "EN ESPERA",
                   aux->datos.idReserva, aux->datos.nombrePasajero, aux->datos.codigoVuelo);
            encontrado = true;
        }
        aux = aux->siguiente;
    }
    return encontrado;
}

// Pide datos por teclado y solicita el registro de un nuevo vuelo
void cargarNuevoVuelo(tNodoVuelo **pListaVuelos, tRedRutas *pRedRutas) {
    tVuelo nuevo;
    int distancia;
    nuevo.idOrigen = 0;
    nuevo.ocupados = 0;
    
    printf("Codigo de vuelo (ej. AR34): ");
    fflush(stdin);
    scanf(" %[^\n]", nuevo.codigoVuelo);
    printf("Aerolinea: ");
    fflush(stdin);
    scanf(" %[^\n]", nuevo.aerolinea);
    printf("ID Aeropuerto destino (1 a 14): ");
    fflush(stdin);
    scanf("%d", &nuevo.idDestino);
    printf("Nombre del destino: ");
    fflush(stdin);
    scanf(" %[^\n]", nuevo.destino);
    printf("Duracion estimada (min): ");
    fflush(stdin);
    scanf("%f", &nuevo.duracion);
    printf("Capacidad maxima: ");
    fflush(stdin);
    scanf("%d", &nuevo.capacidadMax);
    printf("Distancia en km: ");
    fflush(stdin);
    scanf("%d", &distancia);

    if (registrarVuelo(pListaVuelos, pRedRutas, nuevo, distancia)) {
        printf("Vuelo %s registrado correctamente.\n", nuevo.codigoVuelo);
    } else {
        printf("Error: Codigo duplicado o capacidad invalida.\n");
    }
}

// Despliega opciones de criterio y muestra los vuelos ordenados
void menuOrdenarYMostrarVuelos(tNodoVuelo *pListaVuelos) {
    int criterio;
    printf("\nOrdenar por: 1.Codigo | 2.Aerolinea | 3.Destino | 4.Duracion: ");
    fflush(stdin);
    scanf("%d", &criterio);
    ordenarVuelos(pListaVuelos, criterio);
    mostrarVuelos(pListaVuelos);
}

// Permite buscar un vuelo mediante busqueda secuencial o binaria
void consultarVueloPorCodigo(tNodoVuelo *pListaVuelos) {
    sstring codigo;
    int metodo;
    printf("\n1. Busqueda Secuencial\n2. Busqueda Binaria\nSeleccione metodo: ");
    fflush(stdin);
    scanf("%d", &metodo);
    printf("Codigo de vuelo a buscar: ");
    fflush(stdin);
    scanf(" %[^\n]", codigo);

    if (metodo == 2) {
        if (busquedaBinariaVuelo(pListaVuelos, codigo)) {
            tNodoVuelo *buscado = buscarVueloPorCodigo(pListaVuelos, codigo);
            printf("Encontrado: Vuelo %s | %s | Destino: %s | Cupo: %d/%d\n",
                   buscado->datos.codigoVuelo, buscado->datos.aerolinea,
                   buscado->datos.destino, buscado->datos.ocupados, buscado->datos.capacidadMax);
        } else {
            printf("Vuelo no encontrado por Busqueda.\n");
        }
    } else {
        tNodoVuelo *buscado = buscarVueloPorCodigo(pListaVuelos, codigo);
        if (buscado != NULL) {
            printf("Encontrado: Vuelo %s | %s | Destino: %s | Cupo: %d/%d\n",
                   buscado->datos.codigoVuelo, buscado->datos.aerolinea,
                   buscado->datos.destino, buscado->datos.ocupados, buscado->datos.capacidadMax);
        } else {
            printf("Vuelo no encontrado.\n");
        }
    }
}

// Solicita datos del pasajero y procesa su reserva en el sistema
void cargarSolicitudReserva(tColaReservas *pConfirmadas, tColaReservas *pEspera, tNodoVuelo *pListaVuelos, int *pIdReserva) {
    tReserva vreserva;
    vreserva.idReserva = (*pIdReserva)++;
    vreserva.confirmada = false;

    printf("DNI del pasajero: ");
    fflush(stdin);
    scanf("%d", &vreserva.dni);
    printf("Nombre del pasajero: ");
    fflush(stdin);
    scanf(" %[^\n]", vreserva.nombrePasajero);
    printf("Codigo de vuelo: ");
    fflush(stdin);
    scanf(" %[^\n]", vreserva.codigoVuelo);

    int resultado = solicitarReserva(pConfirmadas, pEspera, pListaVuelos, vreserva);
    if (resultado == 1) {
        printf("Reserva confirmada para %s en vuelo %s.\n", vreserva.nombrePasajero, vreserva.codigoVuelo);
    } else if (resultado == 2) {
        printf("Vuelo %s completo. %s ingresa a la cola de espera.\n", vreserva.codigoVuelo, vreserva.nombrePasajero);
    } else {
        printf("Error: No existe el vuelo %s.\n", vreserva.codigoVuelo);
    }
}

// Cancela una reserva y asigna la vacante al primero en espera
void procesarCancelacionReserva(tColaReservas *pConfirmadas, tColaReservas *pEspera, tNodoVuelo *pListaVuelos) {
    int dni;
    sstring codigo;
    tReserva eliminada, promovida;

    printf("DNI del pasajero: ");
    fflush(stdin);
    scanf("%d", &dni);
    printf("Codigo de vuelo: ");
    fflush(stdin);
    scanf(" %[^\n]", codigo);

    if (extraerReservaDeCola(pConfirmadas, dni, codigo, &eliminada)) {
        tNodoVuelo *vuelo = buscarVueloPorCodigo(pListaVuelos, codigo);
        if (vuelo != NULL) {
            vuelo->datos.ocupados--;
        }
        printf("Reserva confirmada cancelada: %s (vuelo %s). Cupo liberado.\n", eliminada.nombrePasajero, codigo);

        if (extraerReservaDeCola(pEspera, 0, codigo, &promovida)) {
            promovida.confirmada = true;
            if (vuelo != NULL) {
                vuelo->datos.ocupados++;
            }
            encolarReserva(pConfirmadas, promovida);
            printf("Cupo asignado por orden de llegada a: %s (DNI %d)\n", promovida.nombrePasajero, promovida.dni);
        }
    } else if (extraerReservaDeCola(pEspera, dni, codigo, &eliminada)) {
        printf("Solicitud en espera cancelada: %s.\n", eliminada.nombrePasajero);
    } else {
        printf("No se encontro reserva para ese DNI y vuelo.\n");
    }
}

// Consulta el estado de las reservas de un pasajero por su DNI
void mostrarReservasPorDni(tColaReservas pConfirmadas, tColaReservas pEspera) {
    int dni;
    printf("Ingrese DNI: ");
    fflush(stdin);
    scanf("%d", &dni);

    bool enConf = buscarPorDniEnCola(pConfirmadas, dni);
    bool enEsp = buscarPorDniEnCola(pEspera, dni);
    
    if (!enConf && !enEsp) {
        printf("No se encontraron reservas para el DNI %d.\n", dni);
    }
}

// Menu de consulta para visualizar las colas o buscar por DNI
void menuConsultasReservas(tColaReservas pConfirmadas, tColaReservas pEspera) {

    int subopcion;

    printf("\n1. Mostrar colas de reservas\n2. Buscar por DNI\nOpcion: ");
    fflush(stdin);
    scanf("%d", &subopcion);
    if (subopcion == 1) {
        printf(TITULO_CONFIRMADAS);
        mostrarColaReservas(pConfirmadas);
        printf(TITULO_ESPERA);
        mostrarColaReservas(pEspera);
    } else if (subopcion == 2) {
        mostrarReservasPorDni(pConfirmadas, pEspera);
    }
}
