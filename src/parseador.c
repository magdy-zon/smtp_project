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

Headers* separa_cabeceras(char* header){
    // Reservamos la memoria necesaria para guardar nuestros headers
    Headers* headers = malloc(sizeof(Headers));
    // Variable temporal de las cabeceras
    char* tmp = strdup(header);
    char** resultado;
    // Tamaño de caracteres del header actual
    int tam_head_actual = 0;
    // Numero de header actual que se recorre
    int num_header = 0;
    // Tamaño del los headers
    int tam_headers[100];
    int i, contador;
    while(*tmp != '\0'){
        // El header es almenos de tamaño 2 para poder diferenciarlo
        // asi que podemos aumentar su tamaño desde el inicio
        tam_head_actual++;
        // Buscamos por un retorno de carro
        if(*tmp == '\x0D') {
        // OK, encontramos un retorno de carro
            if(*(tmp + 1) == '\x0A') {
                // Reducimos el tamaño del header para quitar el CRFL
                tam_head_actual--;
                tam_headers[num_header] = tam_head_actual;
                num_header++;
                tam_head_actual = 0;
                tmp++;
                if(*(tmp + 2) == '\0'){
                    // Dado que si al final hay un CRFL aumenta el numero
                    // de headers lo disminuimos para evitar inconcistencias
                    num_header--;
                }
            }
        }
        tmp++;
    }
    // No habia un CRFL al final de los headers    
    if(tam_head_actual != 0){
        num_header++;
        tam_head_actual--;
        tam_headers[num_header] = tam_head_actual;
    }
    resultado = malloc(sizeof(char*) * ++num_header);
    contador = 0;
    // Guardamos por cada header su valor
    for(i = 0; i < num_header; i++ ){
        //printf("%d\n", tam_headers[i]);
        resultado[i] = strndup((header+contador), tam_headers[i]);
        // Vamos contando hasta que caracteres ya van revisados
        contador = contador + tam_headers[i] + 2;
    }
    headers->num_headers = num_header;
    headers->arreglo = resultado;
    
}

char** separa_mensaje(char* mensaje){
    char* tmp = strdup(mensaje);
    char** resultado = malloc(sizeof(char*) * 2);
    char *body, *header;
    int tamano_cabecera = 0;
    int listo = 1;
    while(*tmp != '\0' && listo){
        // Buscamos por un retorno de carro
        if(*tmp == '\x0D') {
        // OK, encontramos un salto de linea
            if(*(tmp + 1) == '\x0A') {
                // Encontramos un CRLF
                if(*(tmp + 2) == '\x0D'){
                    // Encontramos el limite de la cabecera
                    tamano_cabecera++;
                    listo = 0;
                }
            }
        }
        tmp++;
        tamano_cabecera++;
    }
    header = strndup(mensaje, tamano_cabecera);
    body = strdup(mensaje+tamano_cabecera);
    resultado[0] = header;
    resultado[1] = body;
    return resultado;
}

Solicitud* new_solicitud(char* mensaje){
    // Reservamos la memoria necesaria para guardar nuestra solicitud
    Solicitud* solicitud = malloc(sizeof(Solicitud));
    // Aqui tenemos al header en mensaje_separado[0], y al body en mensaje_
    // separado[1]
    char** mensaje_separado = separa_mensaje(mensaje);
    // Aqui tenemos al request_line en headers[0]
    Headers* cabeceras = separa_cabeceras(mensaje_separado[0]);
    // Aqui separamos al request_line en 0, 1, 2
    char** rqs = separa_request_line(cabeceras->arreglo[0]);
    // A partir de aqui agregamos lo necesario
    solicitud->metodo = rqs[0];
    solicitud->uri = rqs[1];
    solicitud->version = rqs[2];
    // Movemos el arreglo para que no tome en cuenta al request line
    cabeceras->arreglo++;
    cabeceras->num_headers--;
    solicitud->headers = cabeceras; 
    solicitud->body = mensaje_separado[1];
    return solicitud;
}

void imprime_solicitud(Solicitud* solicitud){
    int i = sizeof(solicitud->headers) / sizeof(solicitud->headers[0]);
    printf("Metodo: %s\n", solicitud->metodo);
    printf("Request-uri: %s\n", solicitud->uri);
    printf("Version: %s\n", solicitud->version);
    printf("Headers: \n");
    for(i=0; i<(solicitud->headers->num_headers); i++){
        printf("%d.-%s \n",i+1, solicitud->headers->arreglo[i]);
    }
    printf("Listo!\n");
    return;
}

char* Respuesta_toString(Respuesta* respuesta){
    char* resultado = malloc(sizeof(char)* 5000);
    char tamano[15]; 
    strcpy(resultado, respuesta->version);
    strcat(resultado, " ");
    strcat(resultado, respuesta->status);
    strcat(resultado, " ");
    strcat(resultado, respuesta->motivo);
    strcat(resultado, "\x0D\x0A\x0D\x0A");
    /* strcat(resultado, "Content-Type: text/html");
    strcat(resultado, "\x0D\x0A");
    strcat(resultado, "Content-Length: ");
    sprintf(tamano, "%d", respuesta->body->size);
    strcat(resultado, tamano);
    strcat(resultado, "\x0D\x0A\x0D\x0A"); */
    strcat(resultado, respuesta->body->contenido);
    return resultado;    
}
