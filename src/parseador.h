#ifndef PARSEADOR_H

typedef struct arreglo_h{
    int num_headers;
    char **arreglo;
} Headers;

typedef struct cuerpo{
    int size;
    char* contenido;
} Body;

typedef struct peticion{
    char *metodo;
    char *uri;
    char *version;
    Headers* headers;
    char *body;
} Solicitud;

typedef struct contesto{
    char *version;
    char *status;
    char *motivo;
    Headers* headers;
    Body *body;
} Respuesta;

/* Funcion que nos separa la request line de un mensaje */
char** separa_request_line(char* rqs_ln);

/* Funcion que nos separa cada cabecera de un mensaje */
Headers* separa_cabeceras(char* header);

/* Funcion que nos separa la cabecera y el cuerpo de un mensaje */
char** separa_mensaje(char* mensaje);

/* Funcion que nos crea una nueva solicitud 
   que podamos procesar a partir de un mensaje */
Solicitud* new_solicitud(char* mensaje);

/* Funcion que nos permite imprimir los datos de un mensaje */

void imprime_solicitud(Solicitud*);

char* Respuesta_toString(Respuesta* respuesta);

#endif
