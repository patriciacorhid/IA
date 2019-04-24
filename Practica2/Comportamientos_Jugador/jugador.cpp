#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>

// Este es el método principal que debe contener los 4 Comportamientos_Jugador
// que se piden en la práctica. Tiene como entrada la información de los
// sensores y devuelve la acción a realizar.
Action ComportamientoJugador::think(Sensores sensores) {
	Action accion = actIDLE;
	// Estoy en el nivel 1
	//sensores.nivel: nivel seleccionado por el usuario
	//sensores.mensaje_: F: fila donde estoy, C: columna donde estoy
	if (sensores.nivel != 4){

	  //Capturar los valores de filas y columnas
	  if (sensores.mensajeF != -1){
	    fil = sensores.mensajeF;
	    col = sensores.mensajeC;
	    ultimaAccion = actIDLE;
	  }

	  //Actualizar el ejecto de la última acción
	  switch(ultimaAccion){
	  case actTURN_R: brujula = (brujula+1)%4;
	    break;
	  case actTURN_L: brujula = (brujula+3)%4;
	    break;
	  case actFORWARD:
	    switch(brujula){
	    case 0: fil--; break;
	    case 1: col++; break;
	    case 2: fil++; break;
	    case 3: col--; break;
	    }
	    break;
	  }

	  //Mirar si ha cambiado el destino
	  if (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna){
	    destino.fila = sensores.destinoF;
	    destino.columna = sensores.destinoC;
	    hayPlan = false;
	  }

	  //Calcular un camino hasta el destino
	  if(!hayPlan){
	    actual.fila = fil;
	    actual.columna = col;
	    actual.orientacion = brujula;
	    hayPlan = pathFinding (sensores.nivel, actual, destino, plan);
	  }

	  Action sigAccion;
	  if(hayPlan and plan.size()>0){ //Hay plan y hay que seguirlo
	    sigAccion = plan.front();
	    plan.erase(plan.begin());
	  } else { //No hay plan y se activa el comportamiento reactivo
	    if (sensores.terreno[2]=='P' or sensores.terreno[2]=='M' or sensores.terreno[2]=='D' or sensores.superficie[2]=='a'){
	      sigAccion = actTURN_R;
	    } else{
	      sigAccion = actFORWARD;
	    }
	  }

	  //Recordar la última accion
	  ultimaAccion = sigAccion;
	  return sigAccion;
	  
	} else {
	   // Estoy en el nivel 2
	  Action sigAccion;

	  //Si no he llegado a una casilla PK y no sé mi posición
	  if(!pos){

	    //Actualizo la brújula
	    switch(ultimaAccion){
	    case actTURN_R: brujula = (brujula+1)%4;
	      break;
	    case actTURN_L: brujula = (brujula+3)%4;
	      break;
	    }

	    //Comprobación de plan y sensores
	    cout<<"HayPlan:" << hayPlan <<"\n";

	    for(int i=1; i<16 && hayPlan==false; i++){
	      cout<<"Sensor "<< i << ": " << sensores.terreno[i] <<"\n";
	    }

	    //Si tengo cerca una casilla PK hago un plan para ir a por ella
	    for(int i=1; i<16 && hayPlan==false; i++){
	      cout<<"en el bucle\n";
	      if(sensores.terreno[i]=='K'){
		cout<<"Casilla a la vista\n";
		hayPlan=true;
		switch(i){
		case 1:
		  cout<<"Caso1"<< sensores.terreno[1] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actTURN_L);
		  plan.push_back(actFORWARD);
		  break;
		case 2:
		  cout<<"Caso2"<< sensores.terreno[2] <<"\n";
		  plan.push_back(actFORWARD);
		  break;
		case 3:
		  cout<<"Caso3"<< sensores.terreno[3] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actTURN_R);
		  plan.push_back(actFORWARD);
		  break;
		case 4:
		  cout<<"Caso4"<< sensores.terreno[4] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actTURN_L);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  break;
		case 5:
		  cout<<"Caso5"<< sensores.terreno[5] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actTURN_L);
		  plan.push_back(actFORWARD);
		  break;
		case 6:
		  cout<<"Caso6"<< sensores.terreno[6] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  break;
		case 7:
		  cout<<"Caso7"<< sensores.terreno[7] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actTURN_R);
		  plan.push_back(actFORWARD);
		  break;
		case 8:
		  cout<<"Caso8"<< sensores.terreno[8] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actTURN_R);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  break;
		case 9:
		  cout<<"Caso9"<< sensores.terreno[9] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actTURN_L);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  break;
		case 10:
		  cout<<"Caso10"<< sensores.terreno[10] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actTURN_L);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  break;
		case 11:
		  cout<<"Caso11"<< sensores.terreno[11] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actTURN_L);
		  plan.push_back(actFORWARD);
		  break;
		case 12:
		  cout<<"Caso12"<< sensores.terreno[12] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  break;
		case 13:
		  cout<<"Caso13"<< sensores.terreno[13] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actTURN_R);
		  plan.push_back(actFORWARD);
		  break;
		case 14:
		  cout<<"Caso14"<< sensores.terreno[14] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actTURN_R);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  break;
		case 15:
		  cout<<"Caso15"<< sensores.terreno[15] <<"\n";
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actTURN_R);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  plan.push_back(actFORWARD);
		  break;
		}
	      }
	    }

	    //Si hay plan intento seguirlo, siempre que no haya obstáculos 
	    if (hayPlan and plan.size()>0 and plan.front()==actFORWARD and (sensores.terreno[2]=='P' or sensores.terreno[2]=='M' or sensores.terreno[2]=='D' or sensores.superficie[2]=='a')){ //Obstáculo en el plan
	      cout<<"Obstaculo en el plan\n";
	      if(sensores.superficie[2]=='a'){
		sigAccion = actIDLE;
	      } else{
		hayPlan = false;
		sigAccion = dado_obstaculo();
	      }
	    } else if(hayPlan and plan.size()>0){ //Siguiendo el plan
	      cout<<"Siguiendo el plan\n";
	      sigAccion = plan.front();
	      plan.erase(plan.begin());
	    } else if(hayPlan and plan.size()==0){ //Fin del plan
	      hayPlan = false;
	      cout << "Final plan" <<sensores.terreno[0];
	      sigAccion = actIDLE;
	    } else if(sensores.terreno[2]=='P' or sensores.terreno[2]=='M' or sensores.terreno[2]=='D' or sensores.superficie[2]=='a'){
	      //Sin plan me encuentro un obstáculo
	      cout<<"Obstaculo\n";
	      sigAccion = dado_obstaculo();
	    } else { //Avanzo sin plan
	      cout<<"A lo loco\n";
	      sigAccion = dado();
	    }

	    /*
	    if (sensores.terreno[0]=='K'){
	      cout<<"Casilla PK\n";
	    }
	    */

	    //Si he llegado a una casilla PK, guardo la información
	    if (sensores.mensajeF != -1){
	      cout<<"Se mi posición\n";
	      fil = sensores.mensajeF;
	      col = sensores.mensajeC;
	      actual.fila = fil;
	      actual.columna = col;
	      destino.fila = sensores.destinoF;
	      destino.columna = sensores.destinoC;
	      pos = true;
	    }

	    //Sé mi posición
	  } else {

	    //cout << " Estoy en el else, pos: " << pos << endl;
	    cout << "hayPlan: " << hayPlan << endl;
	    
	    //Actualizo la brújula
	    switch(ultimaAccion){
	    case actTURN_R: brujula = (brujula+1)%4;
	      break;
	    case actTURN_L: brujula = (brujula+3)%4;
	      break;
	    case actFORWARD:
	      switch(brujula){
	      case 0: fil--; break;
	      case 1: col++; break;
	      case 2: fil++; break;
	      case 3: col--; break;
	      }
	      break;
	    }

	    //Actualizo el mapa
	    mapaResultado[fil][col]=sensores.terreno[0];

	    //Compruebo que el destino no lo han cambiado (Los sensores funcionan siempre)
	    if (sensores.destinoF != destino.fila or sensores.destinoC != destino.columna){
	      //cout << "Recalculando objetivo" << endl;
	      destino.fila = sensores.destinoF;
	      destino.columna = sensores.destinoC;
	      hayPlan = false;
	    }

	    //Si no hay plan, calculo uno (búsqueda en anchura)
	    if(!hayPlan){
	      //cout << "Buscando plan"<< endl;
	      actual.fila = fil;
	      actual.columna = col;
	      actual.orientacion = brujula;
	      hayPlan = pathFinding (sensores.nivel, actual, destino, plan);
	    }

	    cout << "Tras buscar plan. hayPlan: " << hayPlan << endl;

	    int aux_fil, aux_col;
	    if(hayPlan and plan.size()>0){ //Hay plan y hay que seguirlo
	      cout<< "Siguiendo el plan hacia el objetivo" <<endl;
	      sigAccion = plan.front();
	      plan.erase(plan.begin());
	      //Si me encuentro con un obstáculo en el plan, giro y descarto el plan
	      if (sigAccion==actFORWARD and (sensores.terreno[2]=='P' or sensores.terreno[2]=='M' or sensores.terreno[2]=='D' or sensores.superficie[2]=='a')){
	        switch(brujula){
		case 0:
		  aux_fil = fil-1;
		  aux_col = col;
		  break;
		case 1:
		  aux_fil = fil;
		  aux_col = col+1;
		  break;
		case 2:
		  aux_fil = fil+1;
		  aux_col = col;
		  break;
		case 3:
		  aux_fil = fil;
		  aux_col = col-1;
		  break;
		}
		mapaResultado[aux_fil][aux_col]=sensores.terreno[2]; //Actualizo el mapa
		cout<<"Obstaculo en la busqueda en anchura\n";
		if (sensores.superficie[2]=='a'){ //Si es aldeano, me espero a que se aparte
		  cout<<"Aldeano\n";
		  sigAccion = actIDLE;
		} else { //Si es muro recalculo el plan
		  hayPlan = false;
		  sigAccion = actTURN_R;
		}
	      }
	    } else { //No hay plan y se activa el comportamiento reactivo
	      hayPlan = false;
	      if (sensores.terreno[2]=='P' or sensores.terreno[2]=='M' or sensores.terreno[2]=='D' or sensores.superficie[2]=='a'){
		//Si me encuentro un obstáculo 
		cout<<"A lo loco encuentro obstáculo";
		switch(brujula){
		case 0:
		  aux_fil = fil-1;
		  aux_col = col;
		  break;
		case 1:
		  aux_fil = fil;
		  aux_col = col+1;
		  break;
		case 2:
		  aux_fil = fil+1;
		  aux_col = col;
		  break;
		case 3:
		  aux_fil = fil;
		  aux_col = col-1;
		  break;
		}
		mapaResultado[aux_fil][aux_col]=sensores.terreno[2]; //Actualizo el mapa 
	        if (sensores.superficie[2]=='a'){ //Si es aldeano, me espero a que se aparte
		  sigAccion = actIDLE;
		} else { //Si es muro recalculo el plan
		  sigAccion = dado_obstaculo();
		}
	      } else{ //Sigo avanzando sin plan
		cout<<"A lo loco";
		sigAccion = dado();
	      }
	    }
	        
	  }

	  ultimaAccion=sigAccion;
	  cout<<"Siguiente Acción"<< sigAccion <<"\n";
	  return sigAccion;
	  
	}
	return accion;
}

//Movimiento aleario cuando no tengo plan
Action ComportamientoJugador::dado(){
  int cual = aleatorio(8);
  Action accion = actIDLE;
  
  switch (cual) {
  case 0: accion = actIDLE; break;
  case 1: accion = actTURN_L; break;
  case 2: accion = actTURN_R; break;
  case 3: case 4: case 5: case 6: case 7: 
    accion = actFORWARD;
    break;
  }

  return accion;
}

//Movimiento aleario cuando no tengo plan y hay obstáculo delante
Action ComportamientoJugador::dado_obstaculo(){
  int cual = aleatorio(2);
  Action accion = actIDLE;
  
  switch (cual) {
  case 0: accion = actTURN_L; break;
  case 1: accion = actTURN_R; break;
  }
  
  return accion;
}

// Llama al algoritmo de busqueda que se usará en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding (int level, const estado &origen, const estado &destino, list<Action> &plan){
  switch (level){
  case 1: cout << "Busqueda en profundad\n";
    return pathFinding_Profundidad(origen,destino,plan);
    break;
  case 2: cout << "Busqueda en Anchura\n";
    return pathFinding_Anchura(origen,destino,plan);
    break;
  case 3: cout << "Busqueda Costo Uniforme\n";
    return pathFinding_CosteUniforme(origen,destino,plan);
    break;
  case 4: cout << "Busqueda para el reto\n";
    return pathFinding_Anchura(origen,destino,plan);
    break;
  }
  cout << "Comportamiento sin implementar\n";
  return false;
}


//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el código en carácter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla){
	if (casilla=='P' or casilla=='M' or casilla =='D')
		return true;
	else
	  return false;
}


// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st){
	int fil=st.fila, col=st.columna;

  // calculo cual es la casilla de delante del agente
	switch (st.orientacion) {
		case 0: fil--; break;
		case 1: col++; break;
		case 2: fil++; break;
		case 3: col--; break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil<0 or fil>=mapaResultado.size()) return true;
	if (col<0 or col>=mapaResultado[0].size()) return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col])){
	  // No hay obstaculo, actualizo el parámetro st poniendo la casilla de delante.
	  st.fila = fil;
	  st.columna = col;
	  return false;
	}
	else{
	  return true;
	}
}


struct nodo{
  estado st;
  list<Action> secuencia;
  int coste;
};

int ComportamientoJugador:: calculaCoste(estado st){
  switch(mapaResultado[st.fila][st.columna]){
  case 'T': return 2;
  case 'B': return 5;
  case 'A': return 10;
  default: return 1;
  }
}

struct CompareCost { 
    bool operator()(nodo const& n1, nodo const& n2) 
    { 
        return n1.coste > n2.coste; 
    } 
};

struct ComparaEstados{
	bool operator()(const estado &a, const estado &n) const{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
	      (a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};


// Implementación de la búsqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
//Se monta árbol de búsqueda y se recorre
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	stack<nodo> pila;											// Lista de Abiertos

  nodo current;
	current.st = origen;
	current.secuencia.empty();

	pila.push(current);

  while (!pila.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		pila.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			pila.push(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			pila.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				pila.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!pila.empty()){
			current = pila.top();
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}

bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	queue<nodo> cola;											// Lista de Abiertos

  nodo current;
	current.st = origen;
	current.secuencia.empty();

	cola.push(current);

  while (!cola.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		cola.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			cola.push(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			cola.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				cola.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!cola.empty()){
			current = cola.front();
		}
	}

  cout << "Terminada la busqueda\n";

	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}


	return false;
}


 bool ComportamientoJugador::pathFinding_CosteUniforme(const estado &origen, const estado &destino, list<Action> &plan){
   //Borro la lista
   cout << "Calculando plan\n";
   plan.clear();
   set<estado,ComparaEstados> generados; // Lista de Cerrados
   priority_queue<nodo, vector<nodo>, CompareCost> cola;											// Lista de Abiertos

   nodo current;
   current.st = origen;
   current.secuencia.empty();
   current.coste = calculaCoste(current.st);
	
   cola.push(current);

   while (!cola.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){
    
     cola.pop();
     generados.insert(current.st);

     // Generar descendiente de girar a la derecha
     nodo hijoTurnR = current;
     hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
     if (generados.find(hijoTurnR.st) == generados.end()){
       hijoTurnR.secuencia.push_back(actTURN_R);
       hijoTurnR.coste += calculaCoste(hijoTurnR.st);
       cola.push(hijoTurnR);

     }

     // Generar descendiente de girar a la izquierda
     nodo hijoTurnL = current;
     hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
     if (generados.find(hijoTurnL.st) == generados.end()){
       hijoTurnL.secuencia.push_back(actTURN_L);
       hijoTurnL.coste += calculaCoste(hijoTurnL.st);
       cola.push(hijoTurnL);
     }

     // Generar descendiente de avanzar
     nodo hijoForward = current;
     if (!HayObstaculoDelante(hijoForward.st)){
       if (generados.find(hijoForward.st) == generados.end()){
	 hijoForward.secuencia.push_back(actFORWARD);
	 hijoForward.coste += calculaCoste(hijoForward.st);
	 cola.push(hijoForward);
       }
     }

     // Tomo el siguiente valor de la pila
     if (!cola.empty()){
       current = cola.top();
     }
   }

   cout << "Terminada la busqueda\n";

   if (current.st.fila == destino.fila and current.st.columna == destino.columna){
     cout << "Cargando el plan\n";
     plan = current.secuencia;
     cout << "Longitud del plan: " << plan.size() << endl;
     PintaPlan(plan);
     // ver el plan en el mapa
     VisualizaPlan(origen, plan);
     return true;
   }
   else {
     cout << "No encontrado plan\n";
   }


   return false;
 }


// Sacar por la términal la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}



void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}


// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}



int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
