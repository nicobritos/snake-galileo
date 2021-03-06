#ifndef _Snake_H_
#define _Snake_H_

#include "Arduino.h"
#include "Direction.h"
#include "Position.h"
#include <EEPROM.h>

class Snake {
  private:
    Direction currentDirection; // En la direccion que esta yendo actualmente la vibora
    Position * body; // Arreglo que marca en que posicion de la pantalla esta cada parte del cuerpo de la vibora
    uint16_t currentLength; // Tamanio de la vibora
    uint32_t currentSpeed; // Cantidad de ms que se espera hasta el siguiente movimiento
    uint32_t startTime; // Tiempo en la que se empezo a jugar
    uint32_t stopTime; // Tiempo en el que se perdio
    uint16_t head; // Posicion dentro del array en la que esta la cabeza
    bool alive; // True si todavia no perdio
    uint16_t columns; // Cantidad de columnas en la que se puede mover la vibora
    uint16_t rows; // Cantidad de filas en la que se puede mover la vibora
  
  public:
    /* Crea el cuerpo de la serpiente y lo posiciona en las matrices */
    Snake(uint16_t initialLength, Direction initialDirection, uint32_t initialSpeed, uint16_t initialRow, uint16_t initialColumn, uint16_t columns, uint16_t rows);
    
    /* Getters */
    Direction getCurrentDirection();
    uint16_t getCurrentLength();
    uint32_t getCurrentSpeed();
    uint32_t getAliveTime();
    Position* getBody();
    uint16_t getHead();
    bool isAlive();

    void revive(uint16_t initialLength, Direction initialDirection, uint32_t initialSpeed, uint16_t initialRow, uint16_t initialColumn); // Empieza un nuevo juego
    bool moveSnake(Direction newDirection, bool enlarge); // Mueve la serpiente en la direccion del parametro, no elimina la cola si enlarge = true
    void setCurrentSpeed(uint32_t newSpeed);

    /* Libera el espacio almacenado en heap por el arreglo de body */
    void freeSnake();
};

#endif
