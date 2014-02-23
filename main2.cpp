/*
 * Proyecto 2: Simulacion
 * 
 * Integrantes:
 * Ada Parra       19842103
 * Eliecer Uribe   20181085
 * Gabriel Nuñez   19843489
 * Roynny Zambrano 22210443
 * Tulio M. Loreto 20261035
 * 
	Se desea simular el siguiente sistema:
	
	A la sección de Atención al Cliente del banco X llegan personas con un tiempo entre
	llegadas exponencial con media de a minutos. Con la finalidad de dar una buena
	atención al cliente, el banco cuenta con cuatro operadores entrenados en los
	procesos del banco, los cuales tardan en promedio b minutos por persona, tiempo
	que sigue una distribución exponencial.
	
	Se desea conocer el desempeño del sistema en cuanto a:
	
	a)Utilización promedio del sistema.
	b)Cantidad promedio de clientes en atención al cliente.
	c)Cantidad promedio de clientes en la cola de atención al cliente.
	d)Tiempo promedio que pasan los clientes en el banco.
	e)Tiempo promedio que esperan los clientes en la cola de atención al cliente. 
 */

/*
 * 
 * El modelo de colas a simular sera:
 * Modelo(M/M/s):(GD/~/~)
 * Multiples servidores prestan servicio en paralelo.
 * 
*/

#include "Simulacion.h"
#include<iostream> //Libreria standard.
#include<fstream>  //Libreria de manejo de archivos.
#include<cstdlib>
#include<list>	//Libreria de lista
#include<time.h> //Libreria para usar hora del sistema (semilla).
#include<math.h> //Para calcular ln(r)

//Para no incluir todo el espacio de nombres (using namespace std;) se incluye solo lo que se utilizara... 
using std::cin; //Para entradas.
using std::cout; //Para salidas.
using std::endl; //Para saltos de linea.
using std::list; //Para la lista.


void generarModeloTeorico(int numeroModelo, Simulacion s, FILE *myfile); //Modelo teorico completo...
int numerosEnteros(int mod_m);
float numerosUniformes(int num_e, int mod_m);
float observacionAleatoria(float r, float media);
void simulacion(Simulacion s);

//Funcion principal del proyecto.
int main(){
	
	// variables de prueba para imprimir..test
	float auxf=0;
	int aux=0;


	const int mod_m=1000;
	FILE *myFile; //Variable para la apertura del archivo de entrada modelo.in.
	FILE *myOtherFile; //Variable para la creacion del archivo de salida performance.out.
	int n; //Cantidad de modelos a tratar.
	float a, //Parametro que representa la media de llegadas 'a' de personas.
		  b; //Parametro que representa el promedio 'b' de servicio por persona.
    int	  replicas, //Cantidad de replicas.
		  ttransicion, //Tiempo de transicion.
		  tsimulacion; //Tiempo de simulacion.
	myFile = fopen("modelo.in","r"); //Apertura del archivo de entrada modelo.in con permisos de lectura solamente.
	myOtherFile = fopen("performance.out","w");	//Apertura del archivo de salida performance.out con permisos de escritura solamente.
	Simulacion s; //Variable de clase Simulacion con la cual se va a interactuar en las simulaciones.
	
	
	fscanf(myFile, "%d", &n); //Lectura de la cantidad n de modelos a tratar.
	cout << "La cantidad de simulaciones a realizar son: " << n << endl; //Prueba.
	for(int i = 0; i < n; i++){ //Se lee cada uno de los modelos a tratar.
		fscanf(myFile, "%f %f %d %d %d", &a, &b, &replicas, &ttransicion, &tsimulacion); //Lectura de datos de entrada.
		
		/**Carga de cada simulacion en la variable s*/
		s.setA(a);
		s.setB(b);
		s.setReplicas(replicas);
		s.setTtransicion(ttransicion);
		s.setTsimulacion(tsimulacion);
		
		cout << "Parametro a: " << s.getA() << endl; //Prueba.
		cout << "Parametro b: " << s.getB() << endl; //Prueba.
		cout << "Numero de replicas: " << s.getReplicas() << endl; //Prueba.
		cout << "Tiempo de transicion: " << s.getTtransicion() << endl; //Prueba.
		cout << "Tiempo de simulacion: " << s.getTsimulacion() << endl; //Prueba.
		cout << "Presione enter para continuar..." << endl; //Prueba.
		
		generarModeloTeorico(i+1,s,myOtherFile); //Llamada para generar archivo de salida performance.out.
		
		getchar(); //Prueba.
	}
	
	
	fclose(myFile); //Cierre del archivo de entrada modelo.in.
	fclose(myOtherFile); //Cierre del archivo de salida performance.out.

	/** Impresiones de prueba */	
	//aux = numerosEnteros(mod_m);		
	//cout << "variable aux:" << aux << endl; 
	//cout << "variable uniforme:" << numerosUniformes(aux, mod_m) << endl;

	//auxf = observacionAleatoria(0.2204 , 2);
	//cout << "variable auxf:" << auxf << endl; 

	simulacion(s);
	return 0;							//Se le dice al sistema que todo salio bien.
}

void generarModeloTeorico(int numeroModelo, Simulacion s, FILE *myfile){	
			
	fprintf(myfile,"Modelo %d: \n", numeroModelo);
	fprintf(myfile,"                 Utilizacion          L          Lq          W          Wq\n");
	fprintf(myfile,"Simulacion       xx.xx                xx.xx      xx.xx      xx.xx       xx.xx\n");
	fprintf(myfile,"Teorico          %.5f             %.5f    %.5f    %.5f     %.5f\n\n",s.PorcentajeUtilizacion(4,4,s.ro(),s.getA()),s.L(4,s.ro()),s.Lq(4,s.ro()),s.W(4,s.ro(),s.getA(),s.getB()),s.Wq(4,s.ro(),s.getA()));
	
}

int numerosEnteros(int mod_m){
	int hora = time(NULL);			//Inicializa variable hora con la del sistema
	srand(hora);						//Usa la hora del sistema como semilla
	
	return (rand() % mod_m);
}

float numerosUniformes(int num_e, int mod_m) { return ((num_e + 0.5) / mod_m); }

float observacionAleatoria(float r, float media){ return ((- 1 / media) * log(1-r)); }

void simulacion(Simulacion s){

	int replicas = s.getReplicas();

	for (int i = 0; i < replicas; ++i){
		//cout << "paso una replica" << endl;
	}

}












