#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"
#include <algorithm>

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;


// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar


// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador){

   
}

bool Player::Alineados4(Environment env, int fil, int col, int &ganador){
if (env.See_Casilla(fil, col)==0)
        return false;
    ganador=0;
    //En la misma Fila
    if (col<4){
      if ((env.See_Casilla(fil, col)%3)   == (env.See_Casilla(fil, col+1)%3) and
          (env.See_Casilla(fil, col+1)%3) == (env.See_Casilla(fil, col+2)%3) and
          (env.See_Casilla(fil, col+2)%3) == (env.See_Casilla(fil, col+3)%3)){
          //cout << "En la misma fila\n";
          ganador= (env.See_Casilla(fil, col)%3);
          return true;
      }
    }
    //En la misma columna
    if (fil<4){
      if ((env.See_Casilla(fil, col)%3)   == (env.See_Casilla(fil+1, col)%3) and
          (env.See_Casilla(fil+1, col)%3) == (env.See_Casilla(fil+2, col)%3) and
          (env.See_Casilla(fil+2, col)%3) == (env.See_Casilla(fil+3, col)%3)){
          //cout << "En la misma columna\n";

          ganador= (env.See_Casilla(fil, col)%3);
          return true;
      }
    }
    //En la diagonal hacia arriba
    if (fil<4 and col<4){
      if ((env.See_Casilla(fil, col)%3)     == (env.See_Casilla(fil+1, col+1)%3) and
          (env.See_Casilla(fil+1, col+1)%3) == (env.See_Casilla(fil+2, col+2)%3) and
          (env.See_Casilla(fil+2, col+2)%3) == (env.See_Casilla(fil+3, col+3)%3)){
          //cout << "En la diagonal principal\n";

          ganador= (env.See_Casilla(fil, col)%3);
          return true;
      }
    }

    //En la diagonal hacia abajo
    if (fil>2 and col<4){
      if ((env.See_Casilla(fil, col)%3)     == (env.See_Casilla(fil-1, col+1)%3) and
          (env.See_Casilla(fil-1, col+1)%3) == (env.See_Casilla(fil-2, col+2)%3) and
          (env.See_Casilla(fil-2, col+2)%3) == (env.See_Casilla(fil-3, col+3)%3)){
          //cout << "En la diagonal secundaria\n";

          ganador= (env.See_Casilla(fil, col)%3);
          return true;
      }
    }

    return false;
}





// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}






// Invoca el siguiente movimiento del jugador
 Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acción que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable

    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual

    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    //actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
	cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t));
    cout << endl;

    /*
    //--------------------- COMENTAR Desde aqui
    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = env.actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }
    */

    //--------------------- COMENTAR Hasta aqui


    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    // valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, alpha, beta);
    //cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    int count=AlphaBeta(actual_, jugador_, PROFUNDIDAD_ALFABETA, accion);

    return accion;
}

 int Player::AlphaBeta(Environment actual, int jugador, int profundidad, Environment::ActionType &accion){

  bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual
  int n_act; //Acciones posibles en el estado actual

  n_act = actual.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"

  cout << " Acciones aplicables ";
  (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
  for (int t=0; t<8; t++)
    if (aplicables[t])
      cout << " " << actual.ActionStr( static_cast< Environment::ActionType > (t));
  cout << endl;

  cout << "n_act" << n_act << endl;
  
  Environment::ActionType act[n_act];
  Environment nodo;
  double valor[n_act];
  int j = 0;

  if(n_act != 0){
    for(int i = 0; i<8; i++){
      cout << "\ni: " << i << "aplicables" << aplicables[i];
      if(aplicables[i]){
	act[j] = static_cast< Environment::ActionType > (i);
	cout << "Accion que compruebo: " << actual.ActionStr(act[j]);

	int k = i;
	nodo = actual.GenerateNextMove(k);
	valor[j] = podaAlphaBeta(nodo, jugador, profundidad - 1, menosinf, masinf, true);

	cout <<"Valor: " << valor[j]  << " j " << j << endl;
	j++;
      }
    }

    double max=menosinf;
    cout << "max: " << max;
    for(int i = 0; i<n_act; i++){
      cout<< "\nValor["<<i<<"]:" << valor[i] <<"\tMax" << max <<endl;
      cout << "Accion["<<i<<"]:" << actual.ActionStr(act[i]);
      if(max<=valor[i]){
	max = valor[i];
	accion = act[i];
	cout << "Mejorando accion: " << actual.ActionStr(act[i]) << endl;
	cout << "Mejorando accion: " << actual.ActionStr(accion) << endl;
      }
      cout<< "n_act: " << n_act << " i " << i; 
    }

    cout << "\nLa accion devuelta es: " << actual.ActionStr(accion) << endl;

    return 1;
    
  } else {
    cout << "No hay acciones posibles" << endl;
    return -1;
  }
}

double Player::podaAlphaBeta(Environment actual, int jugador, int profundidad, double alpha, double beta, bool maximizingPlayer){
  //Calculo con la heurística el valor del nodo hoja
  if(profundidad == 0 or actual.JuegoTerminado()){
    return ValoracionTest(actual, jugador);
  }

  //Calculo las posibles acciones a partir del nodo actual
  Environment siguientes[8];
  int n_siguientes;
  double eval, maxEval, minEval;

  n_siguientes=actual.GenerateAllMoves(siguientes);

  if(maximizingPlayer){
    maxEval = menosinf;
    
    for(int i=0; i<n_siguientes; i++){
      eval = podaAlphaBeta(siguientes[i], jugador, profundidad - 1, alpha, beta, false);
      maxEval = max(maxEval, eval);
      alpha = max(alpha, eval);

      if(beta <= alpha){
	break;
      }
    }

    return maxEval;

  } else {
    minEval = menosinf;

    for(int i=0; i<n_siguientes; i++){
      eval = podaAlphaBeta(siguientes[i], jugador, profundidad - 1, alpha, beta, true);
      minEval = min(minEval, eval);
      beta = min(beta, eval);

      if(beta <= alpha){
	break;
      }
    }

    return minEval;
  }

}

