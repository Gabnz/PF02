#ifndef SIMULACION_H_
#define SIMULACION_H_
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

class Simulacion{
	
	/**Atributos*/
	private:
		float a, /**Parametro que representa la media de llegadas 'a' de personas.*/
		b; /**Parametro que representa el promedio 'b' de servicio por persona.*/
		int	replicas, /**Cantidad de replicas.*/
		ttransicion, /**Tiempo de transicion.*/
		tsimulacion; /**Tiempo de simulacion.*/
	
	/**Metodos*/
	public:
	
	/** Constructor con parametros:
	 * Inicializa automaticamente el constructor pasando los atributos de una vez*/
	Simulacion(float auxa, float auxb, int auxreplicas, int auxttransicion, int auxtsimulacion) : a(auxa), b(auxb), replicas(auxreplicas), ttransicion(auxttransicion), tsimulacion(auxtsimulacion){}
	
	/** Constructor sin parametros:
	 * Inicializa el constructor de la clase sin necesidad de ningun parametro*/
	Simulacion() : a(0), b(0), replicas(0), ttransicion(0), tsimulacion(0){}
	
	/** Metodos set*/
	
	void setA(float auxa){ a = auxa; }
	
	void setB(float auxb){ b = auxb; }
	
	void setReplicas(int auxreplicas){ replicas = auxreplicas; }
	
	void setTtransicion(int auxttransicion){ ttransicion = auxttransicion; }
	
	void setTsimulacion(int auxtsimulacion){ tsimulacion = auxtsimulacion; }
	
	/** Metodos get*/
	
	float getA(){ return a; }
	
	float getB(){ return b; }
	
	int getReplicas(){ return replicas; }
	
	int getTtransicion(){ return ttransicion; }
	
	int getTsimulacion(){ return tsimulacion; }
	
	/** Prototipos de las implementaciones en Simulacion.cpp */
	
	bool protoPrueba();
	
	int factorial(int n);/**funcion factorial*/
	
	/**Medidas de desempeno teoricas*/
	float P0(int s, int n, float ro);
	
	float Pn(int s, int n, float ro);
	
	float L(int s, float ro);/**cantidad esperada de clientes en el sistema*/
	
	float Lq(int s, float ro);/**cantidad esperada de clientes en la cola*/
	
	float W(int s, float ro, int lambda, int mu);/**tiempo esperado de permanencia en el sistema*/
	
	float Wq(int s, float ro, int lambda);/**tiempo esperado de permanencia en la cola*/
	
	//Se calcula lambda sombrero...
	float lambdaSombrero(int s, int n, int ro, int lambda); 
	
	//Servidores ocupados (S con raya arriba)... 
	//Esto se calcula con s=lambdaS/miu = media efectiva de llegada (lambda sombrero) / media de servicio (miu = b)...
	float S(int s, int n, int ro, int lambda){ return (lambdaSombrero(s,n,ro,lambda)/getB());}
	
	//Ro se calcula ro = lambda/miu (pag 16, tema teoria de colas)...
	float ro(){return getA()/getB();} 
	
	//Porcentaje de utilizacion del sistema... = servidores ocupados/cantidad de servidores x 100...	
	float PorcentajeUtilizacion(int s, int n, int ro, int lambda){ return (S(s,n,ro,lambda)/4)*100;} 
	
	/**Medidas de desempeno con simulacion*/
	
};

#endif
