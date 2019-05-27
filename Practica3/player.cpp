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


//Comprueba que haya 3 fichas alineadas del jugador que le pasa como parámetro
bool Player::Alineados3(Environment env, int fil, int col, int &ganador, int jugador){
  if (env.See_Casilla(fil, col)==0)
    return false;
  ganador=0;
  //En la misma Fila
  if (col<5){
    if ((env.See_Casilla(fil, col)%3)   == jugador and
	(env.See_Casilla(fil, col)%3)   == (env.See_Casilla(fil, col+1)%3) and
	(env.See_Casilla(fil, col+1)%3) == (env.See_Casilla(fil, col+2)%3)){

      //cout << "En la misma fila 3\n";
      ganador= (env.See_Casilla(fil, col)%3);
      return true;
    }
  }
  //En la misma columna
  if (fil<5){
    if ((env.See_Casilla(fil, col)%3)   == jugador and
	(env.See_Casilla(fil, col)%3)   == (env.See_Casilla(fil+1, col)%3) and
	(env.See_Casilla(fil+1, col)%3) == (env.See_Casilla(fil+2, col)%3)){

      //cout << "En la misma columna 3\n";
      ganador= (env.See_Casilla(fil, col)%3);
      return true;
    }
  }
  //En la diagonal hacia arriba
  if (fil<5 and col<5){
    if ((env.See_Casilla(fil, col)%3)   == jugador and
	(env.See_Casilla(fil, col)%3)     == (env.See_Casilla(fil+1, col+1)%3) and
	(env.See_Casilla(fil+1, col+1)%3) == (env.See_Casilla(fil+2, col+2)%3)){
      //cout << "En la diagonal principal\n";

      ganador= (env.See_Casilla(fil, col)%3);
      return true;
    }
  }

  //En la diagonal hacia abajo
  if (fil>1 and col<5){
    if ((env.See_Casilla(fil, col)%3)   == jugador and
	(env.See_Casilla(fil, col)%3)     == (env.See_Casilla(fil-1, col+1)%3) and
	(env.See_Casilla(fil-1, col+1)%3) == (env.See_Casilla(fil-2, col+2)%3)){

      //cout << "En la diagonal secundaria 3\n";
      ganador= (env.See_Casilla(fil, col)%3);
      return true;
    }
  }

  return false;
}

//Comprueba que haya 2 fichas alineadas del jugador que le pasa como parámetro
bool Player::Alineados2(Environment env, int fil, int col, int &ganador, int jugador){
  if (env.See_Casilla(fil, col)==0)
    return false;
  ganador=0;
  //En la misma Fila
  if (col<6){
    if ((env.See_Casilla(fil, col)%3)   == jugador and
	(env.See_Casilla(fil, col)%3) == (env.See_Casilla(fil, col+1)%3)){

      //cout << "En la misma fila 2\n";
      ganador= (env.See_Casilla(fil, col)%3);
      return true;
    }
  }
  //En la misma columna
  if (fil<6){
    if ((env.See_Casilla(fil, col)%3)   == jugador and
	(env.See_Casilla(fil, col)%3) == (env.See_Casilla(fil+1, col)%3)){

      //cout << "En la misma columna 2\n";
      ganador= (env.See_Casilla(fil, col)%3);
      return true;
    }
  }
  //En la diagonal hacia arriba
  if (fil<6 and col<6){
    if ((env.See_Casilla(fil, col)%3)   == jugador and
	(env.See_Casilla(fil, col)%3) == (env.See_Casilla(fil+1, col+1)%3)){

      //cout << "En la diagonal principal 2\n";
      ganador= (env.See_Casilla(fil, col)%3);
      return true;
    }
  }

  //En la diagonal hacia abajo
  if (fil>0 and col<6){
    if ((env.See_Casilla(fil, col)%3)   == jugador and
	(env.See_Casilla(fil, col)%3) == (env.See_Casilla(fil-1, col+1)%3)){

      //cout << "En la diagonal secundaria 2\n";
      ganador= (env.See_Casilla(fil, col)%3);
      return true;
    }
  }

  return false;
}

// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Player::Valoracion(const Environment &estado, int jugador){
  double max = 99999999.0; //maxima puntuación
  double min = -99999999.0; //mínima puntuación
  int enemigo;  //Contrincante
  bool alineados_mios;
  bool alineados_enemigo;

  //Inicializo al enemigo
  if(jugador == 1){
    enemigo = 2;
  } else{
    enemigo = 1;
  }

  int ganador1 = estado.RevisarTablero(); //Vemos quien gana
  int ganador2 = 0; //La variable de alineación que le paso al enemigo

  if(ganador1==jugador){ // Gana el jugador que pide la valoracion
    //cout << "GANAS " << jugador << endl;
    return max;

  } else if (ganador1!=0){ // Pierde el jugador que pide la valoracion
    //cout << "Pierdes " << jugador << endl;
    return min;

  } else if  (estado.Get_Casillas_Libres()==0){
    return 0;  // Hay un empate global y se ha rellenado completamente el tablero

  } else {

    for(int i = 0; i<7; i++){ //Cuanto menos casillas alineadas tengas, mayor puntuación obtienes

      for(int j = 0; j<7; j++){

	alineados_mios=Alineados3(estado, i , j, ganador1, jugador);
	alineados_enemigo=Alineados3(estado, i , j, ganador2, enemigo);

	//Si yo tengo 3 alineadas
	if(ganador1==jugador){
	  if(ganador2==enemigo){ //Y el enemigo tiene 3 alineadas
	    return -100;
	  } else{
	    alineados_enemigo=Alineados2(estado, i , j, ganador2, enemigo);
	    if(ganador2==enemigo){ //Y el enemigo tiene 2 alineadas
	      return -10000;
	    } else {
	      return -1000000; //Y el enemigo no tiene ninguna alineada
	    }
	  }
	}

        alineados_mios=Alineados2(estado, i , j, ganador1, jugador);
	alineados_enemigo=Alineados3(estado, i , j, ganador2, enemigo);
	
	//Si yo tengo 2 alinaedas
	if(ganador1==jugador){
	  if(ganador2==enemigo){ //Y el enemigo tiene 3 alineadas
	    return 1000;
	  } else{
	    alineados_enemigo=Alineados2(estado, i , j, ganador2, enemigo);
	    if(ganador2==enemigo){ //Y el enemigo tiene 2 alineadas
	      return -10;
	    } else {
	      return -1000; //Y el enemigo no tiene ninguna alineada
	    }
	  }
	}

        alineados_enemigo=Alineados3(estado, i , j, ganador2, enemigo);

	//Si yo no tengo ninguna alineada
	if(ganador2==enemigo){ //Y el enemigo tiene 3 alineadas
	  return 1000000;
	} else{
	  alineados_enemigo=Alineados2(estado, i , j, ganador2, enemigo);
	  if(ganador2==enemigo){ //Y el enemigo tiene 2 alineadas
	    return 10000;
	  } else {
	    return 100; //Y el enemigo no tiene ninguna alineada
	  }
	}
      }
    }
  }
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

  cout << "n_act " << n_act << endl;
  
  Environment::ActionType* act=new Environment::ActionType[n_act];
  Environment nodo;
  double valor[n_act];
  int j = 0;

  if(n_act != 0){ //Para cada acción aplicable genero el tablero a partir de esa opción y calculo el resultado de la poda AlphaBeta
    for(int i = 0; i<8; i++){
      cout << "\ni: " << i << " aplicables " << aplicables[i] << endl;
      if(aplicables[i]){
	act[j] = static_cast< Environment::ActionType > (i);
	cout << "Accion que compruebo: " << actual.ActionStr(act[j]);

	int k = i-1;
	nodo = actual.GenerateNextMove(k);
	valor[j] = podaAlphaBeta(nodo, jugador, profundidad - 1, menosinf, masinf, false);

	cout <<"Valor: " << valor[j]  << " j " << j << endl;
        j++;
      }
    }

  /*
  Environment sig_mov[8];
  int n_sig_mov = actual.GenerateAllMoves(sig_mov);

  double max = menosinf;
  Environment::ActionType max_action; 
  for(int i=0; i<n_sig_mov; i++) {
    Environment e = sig_mov[i];
    double v = podaAlphaBeta(e, jugador, profundidad - 1, menosinf, masinf, false);
  }
  POR TERMINAR
  */
  
    double max=menosinf;
    //cout << "max: " << max <<endl;
    for(int i = 0; i<n_act; i++){ //Me quedo con la acción cuyo valor de la poda sea el máximo
      cout<< "\nValor["<<i<<"]:" << valor[i] << endl; //<<"\tMax" << max <<endl;
      cout << "Accion["<<i<<"]:" << actual.ActionStr(act[i]) <<endl;
      if(max<=valor[i]){
	max = valor[i];
	accion = act[i];
	//cout << "Mejorando accion: " << actual.ActionStr(act[i]) << endl;
	//cout << "Mejorando accion: " << actual.ActionStr(accion) << endl;
      }
      cout<< "n_act: " << n_act << " i " << i; 
    }

    cout << "\nLa accion devuelta es: " << actual.ActionStr(accion) << endl;

    delete[] act;
    return 1;
    
  } else {
    cout << "No hay acciones posibles" << endl;

    delete[] act;
    return -1;
  }
}

double Player::podaAlphaBeta(Environment actual, int jugador, int profundidad, double alpha, double beta, bool maximizingPlayer){
  //Calculo con la heurística el valor del nodo hoja
  if(profundidad == 0 or actual.JuegoTerminado()){
    return Valoracion(actual, jugador);
  }

  //Calculo las posibles acciones a partir del nodo actual
  Environment* siguientes=new Environment[8];
  int n_siguientes;
  double eval, maxEval, minEval;

  n_siguientes=actual.GenerateAllMoves(siguientes);

  //Si soy el jugador que intenta maximizar la función
  if(maximizingPlayer){
    maxEval = menosinf;

    //cout<<"*********************MAXIMIZANDO***********************"<<endl;
    
    for(int i=0; i<n_siguientes; i++){ //Calculo el valor máximo que puedo obtener
      eval = podaAlphaBeta(siguientes[i], jugador, profundidad - 1, alpha, beta, false);
      //cout<<"\n1--- maxEval= " << maxEval << " alpha= " << alpha << " eval= " << eval <<endl;
      maxEval = max(maxEval, eval);
      alpha = max(alpha, eval);
      //cout<<"\n2--- maxEval= " << maxEval << " alpha= " << alpha << " eval= " << eval <<endl;
      
      if(beta <= alpha){ //Si se invierten los límites, es que el otro jugador no va a escoger esta opción -> Se poda
	break;
      }
    }

    delete[] siguientes;
    return maxEval;

  } else { //Si soy el jugador que intenta minimizar la función
    minEval = masinf;

    //cout<<"*********************MINIMIZANDO***********************"<<endl;

    for(int i=0; i<n_siguientes; i++){ //Calculo el valor mínimo que puedo obtener
      eval = podaAlphaBeta(siguientes[i], jugador, profundidad - 1, alpha, beta, true);
      //cout<<"\n1--- minEval= " << minEval << " beta= " << beta << " eval= " << eval <<endl;
      minEval = min(minEval, eval);
      beta = min(beta, eval);

      //cout<<"\n2--- minEval= " << minEval << " beta= " << beta << " eval= " << eval <<endl;
      if(beta <= alpha){ //Si se invierten los límites, es que el otro jugador no va a escoger esta opción -> Se poda
	break;
      }
    }

    delete[] siguientes;
    return minEval;
  }

}

