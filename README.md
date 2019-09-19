
![alt text](https://i.ibb.co/thLvvG4/reversi.jpg)

# Reversi

Trabajo de la asignatura Proyecto Hardware

## 1 - Explicación del juego

### 1.1 - Requisitos de juego

El Juego Reversi consta con dos participantes, utilizando un tablero para jugar.
* Jugadores - 2.
* Fichas - 64 fichas iguales, blancas por una cara y negras por la otra.
* Tablero - tablero de 64 casillas.
* Objetivo - Poseer más fichas con su color que el adversario, al final de la partida.

### 1.2 - El Juego

El juego se inicia con cuatro fichas posicionadas en el centro del tablero en forma de un cuadrado, dos mostrando el lado blanco
hacia arriba y dos el lado negro. El primer movimiento lo realiza el jugador con las fichas negras.

#### 1.2.1 - Colocando fichas

Para que un jugador pueda colocar una ficha en una casilla del tablero, es necesario que haya por lo menos una línea recta 
(horizontal, vertical o diagonal) sin espacios vacíos entre la posición que el jugador desea y otra ficha de su color, y que 
haya una o más fichas del oponente entre ellas.
Quando un jugador haga una jugada válida, todas las fichas del oponente que estén en una línea recta (horizontal, vertical o 
diagonal) con la pieza recién colocada y entre la pieza recién colocada y otra pieza del jugador que hizo el movimiento, serán 
giradas y pasan a ser del color de ese jugador.

### 1.3 - Fin de la Partida

El juego termina cuando ninguno de los jugadores pueda hacer movimientos (por ejemplo, cuando todas las casas estén ocupadas o 
todas las fichas del tablero estén con el mismo color girado hacia arriba).

* Vence el jugador que posea más fichas que el adversario.
* Si ambos jugadores tienen la misma cantidad de fichas, el juego termina en empate.

## 2 - Desarrollo del trabajo

### 2.1 - Práctica 1 

En la primera práctica se ha optimizado el rendimiento de un juego acelerando las funciones computacionalmente más costosas. A 
partir del código facilitado en C, se ha desarrollado un código para un procesador ARM y se ejecutado tanto sobre un procesador
real en la placa de desarrollo como sobre un emuladoren el PC. Para ello, se ha aprendido a trabajar con el entorno de 
desarrollo Eclipse + gcc  del laboratorio y con la placa (Embest  S3CEV40). En esta placa el procesador está acompañado de 
múltiples dispositivos, principalmente de entrada/salida. También se ha entendido la funcionalidad del código suministrado y se 
ha realizado una versión en ensamblador ARM optimizandolas funciones críticas. Se ha medido el rendimiento respecto al código 
optimizado generado por el compilador y se han documentado los resultados. Se ha medido el tamaño en bytes, número de
instrucciones ejecutadas y tiempo de ejecución. Para  medir el tiempo se han programado los temporizadores internos de la placa.
Se ha vericado también que todas las versiones del código dan un resultado equivalente.


### 2.2 - Práctica 2 y 3

En la segunda práctica se ha comenzado a desarrollar el soporte necesario para jugar al Reversi directamente desde la placa. 
Para ello se ha creado la entrada salida básica mediante el uso de los dos botones y el display de 8 segmentos de la placa. Se 
ha seguido interaccionando con ellos en C y se ha aprendido a depurar diversas fuentes concurrentes de interrupción. Además, se 
ha aislado la entrada salida de forma que podamos trabajar con y sin placa. 

En la tercera práctica se ha finalizado el proyecto en el que se ha estado trabajando para conseguir un sistema empotrado 
autónomo de los ordenadores del laboratorio con el que se pueda jugar directamente. Para ello se a:

* Utilizado la pantalla LCD táctil para visualizar e introducir movimientos en el tablero. 
* Cargado el código ensamblador en la memoria Flash de la placa mediante el estándar JTAG, de forma que al encenderla se pueda 
  jugar sin necesidad de conectarse ni descargar el programa.  

## 3 - Hardware empleado para el desarrollo del trabajo

* Placa modelo Embest S3CEV40

![alt text](https://i.ibb.co/1JTWrpF/ndice.jpg)

## 4 - Autores

Programadores y diseñadores del trabajo

* **Sakerdot** - [Sakerdot](https://github.com/Sakerdot)
* **ZgzInfinity**  - [ZgzInfinity](https://github.com/ZgzInfinity)
