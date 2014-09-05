// Variable que define el retorno de carro salto de linea
#define CRLF "\x0D\x0A"
#ifndef PARSEADOR_H

// Estructura que representa un mensaje
typedef struct arreglo_mensaje{
    // En un principio pensemos solo en un destinatario
    char* destinatario;
    char* mensaje;
} Mensaje;

/* Funcion que nos separa la request line de un mensaje */
char** separa_request_line(char* rqs_ln);

/* Funcion que dada una fuente nos crea el mensaje MAIL FROM */
char* crea_mail_from(char* fuente);

/* Funcion que dado un destino nos crea el mensaje RCP TO */
char* crea_rcpt_to(char* destino);

#endif