#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
 public:
  Player(int jug);
  Environment::ActionType Think();
  void Perceive(const Environment &env);
  int AlphaBeta(Environment actual, int jugador, int profundidad, Environment::ActionType &accion);
  double podaAlphaBeta(Environment actual, int jugador, int profundidad, double alpha, double beta, bool maximizingPlayer);
  bool Alineados3(Environment env, int fil, int col, int &ganador, int jugador);
  bool Alineados2(Environment env, int fil, int col, int &ganador, int jugador);
  double Valoracion(const Environment &estado, int jugador);
  
 private:
  int jugador_;
  Environment actual_;
};
#endif
