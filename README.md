Tarea 3 ELO-330
=========================

Nombre
------

**erp_upd** : Emulador de retardo y pérdida de paquetes en
transferencias UDP

Sinopsis
--------

    erp_upd retardo_promedio variación_retardo porcentaje_pérdida puerto_local [host_remoto] puerto_remoto 

Descripción
-----------

El programa **erp_upd** cumple con las especificaciones dadas en las instrucciones de la tarea. Además muestra por pantalla las estampas de tiempo al recibir y enviar los paquetes, con resoluciones en nanosegundos.

Nuestra metodología para generar el retraso fue mediante la función de sistema **usleep()**, que se utilizó para dormir el hilo de envio la cantidad de milisegundos deseada en el retraso, considerando la desviación porcentual. Es por esto que nuestro programa trabaja con retrasos inferiores a 1 segundo, debido a la resolución de **usleep()**. 

Ejemplo
-------

Dentro de nuestra tarea se adjuntan dos programas externos, un cliente y un servidor UDP, que permiten verificar la funcionalidad del programa **erp_udp**. Para hacer esto se describe el siguiente procedimiento:

1.- Compilar el programa,

    $ make

2.- Correr un primer peer UDP,

    $ make peerA

3.- En una terminal aparte, correr un segundo peer UDP,

    $ make peerB

4.- Finalmente, en una tercera terminal correr el programa **erp_udp**,

    $ make run

El programa de cliente UDP recibe el input estándar y lo envia al programa **erp_udp**, quien a su vez redirige los paquetes con retardo hacia el servidor UDP, que los muestra por pantalla al recibirlos.

Retorno
-------

**erp_udp** permanece en estado de vigilia para procesamiento de paquetes, por lo que no retorna de forma autónoma. El programa se puede cerrar enviando la señal SIGINT (mediante Control+C).
