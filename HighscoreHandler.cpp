#include "HighscoreHandler.h"

#define BYTE_SIZE 8

//Escribe una variable de 64 bits en la ROM, supone que hay 64 bits libres desde la posicion address
void writeInEEPROM(uint64_t address, uint64_t data){
  uint64_t byteToWrite = 0;
  for(int j=sizeof(data)-1; j>=0; j--){
    byteToWrite = data & 0xFF; //Agarro el byte menos significativo
    //Serial.print("Writing in ROM: ");
    //Serial.println((long) byteToWrite, DEC);

    EEPROM.write(address + j, byteToWrite); //Lo meto en la posicion del final reservada para ese numero
    data >>= BYTE_SIZE; //Me voy al proximo byte
  }
}

int8_t descendingCompareFunction(const void *a, const void *b) {
  int64_t result = *((uint64_t*) b) - *((uint64_t*) a);
  return result > 0 ? 1 : result < 0 ? -1 : result;
}

HighscoreHandler::HighscoreHandler(){
}

HighscoreHandler::HighscoreHandler(uint32_t startingAddress,uint32_t maxScores){
  this->startingAddress = startingAddress;
  this->maxScores = maxScores;
  currentLoadedScores = 0;
  scores = (uint64_t*)malloc((maxScores+1) * sizeof(*scores)); // + 1 porque en la primera posicion se guarda la cantidad de highscores
  initializeScores();
}

void HighscoreHandler::initializeScores(){
  //En los primeros 8 bytes se guarda la cantidad de scores que se guardaron en EEPROM
  uint64_t aux = 0;
  for(int j=0; j<sizeof(aux) - 1; j++){
      aux |= EEPROM.read(startingAddress + j);
      aux <<= BYTE_SIZE;
   }
   aux |= EEPROM.read(startingAddress + (sizeof(aux) - 1));
   currentLoadedScores = aux;
 
  for(int i=0; i<currentLoadedScores; i++){ 
    aux = 0;
    for(int j=0; j<sizeof(aux) - 1; j++){
      aux |= EEPROM.read(startingAddress + (i+1)*sizeof(*scores) + j);//Corro 1 porque la primera posicion es la cantidad de scores en la ROM
      aux <<= BYTE_SIZE;
    }
    aux |= EEPROM.read(startingAddress + (i+1)*sizeof(*scores) + (sizeof(aux) -1));
    scores[i] = aux;
  }
}

void HighscoreHandler::registerScore(uint64_t score){
    int scoreRanking = rankScore(score); //Posicion en la que se deberia insertar el score
    
    if(scoreRanking <= maxScores){ //si lo tengo que registrar
      //lo escribo en RAM para que quede ordenado
      for(int i=currentLoadedScores-1; i>=scoreRanking; i--){ //Muevo todo el array para dejar espacio al nuevo
        //Serial.println("Reemplazo");
        scores[i+1] = scores[i];
      }
      scores[scoreRanking] = score;
      if(currentLoadedScores < maxScores)
        currentLoadedScores++;

      //lo escribo en ROM ordenado, aprovechando que ya lo ordene en RAM
      for(int i=currentLoadedScores-1; i>=scoreRanking; i--){ //Muevo todo el array para dejar espacio al nuevo
        writeInEEPROM(startingAddress + (i+1)*sizeof(*scores), scores[i]); //corro uno en la ROM por el valor de la cantidad de scores
        writeInEEPROM(startingAddress, currentLoadedScores);
      }

    }
}

int HighscoreHandler::rankScore(uint64_t score){ //Devuelve la posicion dentro del array de ram que deberia estar este score
  for(int i=currentLoadedScores - 1; i>=0; i--){
    if(scores[i] > score){
      return i+1;
    }
  }
  return 0;
}

uint32_t HighscoreHandler::getScoresAmmount(){
  return currentLoadedScores;
}

uint64_t * HighscoreHandler::getScores(){
  return scores;
}

void HighscoreHandler::resetScores(){
  writeInEEPROM(startingAddress, 0);
  currentLoadedScores = 0;
}
