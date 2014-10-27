Tarea 3 ELO-330
=========================

Nombre
------

**erp_upd** : Emulador de retardo y pérdida de paquetes en
transferencias UDP

Sinopsis
--------

  erp_upd retardo_promedio variación_retardo porcentaje_pérdida
puerto_local [host_remoto] puerto_remoto 

Descripción
-----------

Ejemplo
-------

Abrir 3 temrinales:

1 -> *$make && make client* Este tendra el cliente de origen de pruebas.
2 -> *$make && make server* Este tendra el servidor final de pruebas.
3 -> *$make && make run* Este contendra nuestro programa erp_udp, que recibira datos desde la Terminal 1, y las redirijira con los parametros entregados hacia la terminal 2.


Retorno
-------

Dificultades
------------
