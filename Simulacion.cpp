#include "Simulacion.h"
#include<cmath>

/** Implementacion de los metodos definidos en Simualcion.h*/

bool Simulacion::protoPrueba(){ return true; }

int Simulacion::factorial(int n){
	
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

float Simulacion::P0(int s, int n, float ro){
	
	float result = 0;
	
	for(int i = 0; i < s; i++){
		result+= (pow(ro, i)/factorial(i)) + pow(ro,s)/(factorial(s)*(1 - (ro/s)));
	}
	
	return 1/result;
}

float Simulacion::Pn(int s, int n, float ro){
	
	float result;
	
	if(n >= 0){
		if(n < s){
			result =  (pow(ro, n)/factorial(n));
		}else{
			result = (pow(ro, n)/(factorial(s)*pow(s, n - s)));
		}
		return result*P0(s, n, ro);
	}
	/**si n es negativo, retorna -1*/
	return -1;
}

float Simulacion::Lq(int s, float ro){
	
	float result;
	
	result = (s*ro)/pow(s - ro, 2);
	
	return result*Pn(s, s, ro);
}

float Simulacion::L(int s, float ro){
	
	return Lq(s, ro) + ro;
}

float Simulacion::Wq(int s, float ro, int lambda){
	
	return Lq(s, ro)/lambda;
}

float Simulacion::W(int s, float ro, int lambda, int mu){
	
	return Wq(s, ro, lambda) + 1/mu;
}


