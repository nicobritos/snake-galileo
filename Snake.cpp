#include "Snake.h"

// TODO: Chequear constraints iniciales
Snake::Snake(uint16_t initialLength, Direction initialDirection, uint64_t initialSpeed, uint16_t initialRow, uint16_t initialColumn, uint16_t _columns, uint16_t _rows) {
  currentDirection = initialDirection;
  currentLength = initialLength;
  currentSpeed = initialSpeed;
  alive = true;
  columns = _columns;
  rows = _rows;
  
  
  // TODO: Corregir esto para soportar diferentes initial directions
  //La serpiente empieza en linea recta mirando para la derecha
  for(uint16_t i=0; i < 3; i++){
    body[i].y = initialRow;
    body[i].x = initialColumn - currentLength + i;
  }
  
  head = currentLength - 1;
}

/* Getters */
Direction Snake::getCurrentDirection() {
  return currentDirection;
}

uint16_t Snake::getCurrentLength() {
  return currentLength;
}

uint64_t Snake::getCurrentSpeed() {
  return currentSpeed;
}

uint64_t Snake::getAliveTime() {
  return alive ? stopTime - startTime : millis() - startTime;
}

Position* Snake::getBody() {
  return body;
}

uint16_t Snake::getHead() {
  return head;
}

bool Snake::isAlive() {
  return alive;
}

/* Setters */
bool Snake::moveSnake(Direction newDirection, bool enlarge) {
  if (!alive) {
    return false;
  }

  uint64_t possibleStopTime = millis();  // Don't account for time spent on routine
  
  switch(newDirection){
    case UP:
      if(currentDirection != DOWN){
        body[(head+1) % (rows * columns)].x = body[head].x;
        body[(head+1) % (rows * columns)].y = body[head].y + 1;
        currentDirection = newDirection;
      }
      else{
        body[(head+1) % (rows * columns)].x = body[head].x;
        body[(head+1) % (rows * columns)].y = body[head].y - 1;
      }
    break;
    case DOWN:
      if(currentDirection != UP){
        body[(head+1) % (rows * columns)].x = body[head].x;
        body[(head+1) % (rows * columns)].y = body[head].y - 1;
        currentDirection = newDirection;
      }
      else{
        body[(head+1) % (rows * columns)].x = body[head].x;
        body[(head+1) % (rows * columns)].y = body[head].y + 1;
      }
    break;
    case LEFT:
      if(currentDirection != RIGHT){
        body[(head+1) % (rows * columns)].x = body[head].x - 1;
        body[(head+1) % (rows * columns)].y = body[head].y;
        currentDirection = newDirection;
      }
      else{
        body[(head+1) % (rows * columns)].x = body[head].x + 1;
        body[(head+1) % (rows * columns)].y = body[head].y;
      }
    break;
    case RIGHT:
      if(currentDirection != LEFT){
        body[(head+1) % (rows * columns)].x = body[head].x + 1;
        body[(head+1) % (rows * columns)].y = body[head].y;
        currentDirection = newDirection;
      }
      else{
        body[(head+1) % (rows * columns)].x = body[head].x - 1;
        body[(head+1) % (rows * columns)].y = body[head].y;
      }
    break;
  }
  
  head = (head + 1) % (rows * columns);
  
  if(enlarge){
    currentLength++;
  }
    
  if((body[head].x >= columns) || (body[head].x < 0) || (body[head].y >= rows) || (body[head].y < 0)) { //si se choco contra algun borde
    stopTime = possibleStopTime;
    return alive = false;
  }
  
  for(uint16_t i=1; i < currentLength; i++){
    if((body[head].x == body[((rows * columns) + head - i)% (rows * columns)].x) && (body[head].y == body[((rows * columns) + head - i)% (rows * columns)].y)) { //si se choco con alguna parte de su cuerpo
      stopTime = possibleStopTime;
      return alive = false;
    }
  }
  
  return alive;
}

void Snake::setCurrentSpeed(uint64_t newSpeed) {
  currentSpeed = newSpeed;
}
