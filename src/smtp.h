#define HELO "HELO redes2015.ciencias.unam.mx \x0D\x0A"
#ifndef SMTP_H

/* Este metodo auxiliar nos sirve para manejar toda la sesion que se va a llevar acabo entre el cliente
   smtp y el servidor como parametros se recibe el identificador generado por el socket en la funcion 
   principal */
int sesion(int socket);

/* Esta funcion nos servira para procesar un estado recibido por el servidor SMTP 
   para saber si tenemos acceso al servicio o nos rechazo el acceso */
int procesa_estado(char** estado); 
#endif
