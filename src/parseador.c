#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "parseador.h"

char** separa_request_line(char* rqs_ln){
    char** result = 0;
    size_t count = 0;
    char* tmp = rqs_ln;
    char* last_comma = 0;
    char sp[2];
    sp[0] = ' ';
    sp[1] = 0;
    
    // Contamos el numero de separadores que encontramos
    while (*tmp)
    {
        if (' ' == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (rqs_ln + strlen(rqs_ln) - 1);
    count++;
    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        // Separamos a la cadena con el delimitador
        char* token = strtok(rqs_ln, sp);

        while (token)
        {
            // Lo guardamos en un espacio aparte
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, sp);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    return result;
}

/* Aqui creamos el mensaje MAIL FROM */
char* crea_mail_from(char* fuente){
    // Reservamos la memoria necesaria consideramos 
    // 200 espacios para el mensaje por eleccion aunque
    // si se quiere ser mas preciso el tamaño se debe ajustar
    // al mensaje
    char* mail_from = malloc(sizeof(char) * 200);
    strcpy(mail_from, "MAIL FROM:<");
    strcat(mail_from, fuente);
    strcat(mail_from, ">\x0D\x0A");
    return mail_from;   
}

/* Aqui creamos el mensaje MAIL FROM */
char* crea_rcpt_to(char* destino){
    // Reservamos la memoria necesaria consideramos 
    // 200 espacios para el mensaje por eleccion aunque
    // si se quiere ser mas preciso el tamaño se debe ajustar
    // al mensaje
    char* rcpt_to = malloc(sizeof(char) * 200);
    strcpy(rcpt_to, "RCPT TO:<");
    strcat(rcpt_to, destino);
    strcat(rcpt_to, ">\x0D\x0A");
    return rcpt_to;
}