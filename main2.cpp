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


// Se define estructura tabla de la simulacion
struct tabla {
 	float tiempo, rllegada, sig_tiempo_llegada,  sig_llegada, sig_salida; 
	int nclientes, s_ocupados;
	list<float> rservicio, sig_tiempo_salida;
	char sig_evento; // L: llegada y S: salida
};

void generarModeloTeorico(int numeroModelo, Simulacion s, FILE *myfile); //Modelo teorico completo...
void generarTablaSimulacion(list<tabla> simu, FILE *myfiletabla);
int numerosEnteros(int mod_m);
float numerosUniformes(int num_e, int mod_m);
float observacionAleatoria(float r, float media);
void simulacion(Simulacion s, list<tabla>  &simu_tabla, int mod_m);

//Funcion principal del proyecto.
int main(){
	
	// variables de prueba para imprimir..test
	float auxf=0;
	int aux=0;


	list<tabla> simu_tabla;

	const int mod_m=1000;
	FILE *myFile; //Variable para la apertura del archivo de entrada modelo.in.
	FILE *myOtherFile; //Variable para la creacion del archivo de salida performance.out.
	FILE *myFiletabla;
	int n; //Cantidad de modelos a tratar.
	float a, //Parametro que representa la media de llegadas 'a' de personas.
		  b; //Parametro que representa el promedio 'b' de servicio por persona.
    int	  replicas, //Cantidad de replicas.
		  ttransicion, //Tiempo de transicion.
		  tsimulacion; //Tiempo de simulacion.
	myFile = fopen("modelo.in","r"); //Apertura del archivo de entrada modelo.in con permisos de lectura solamente.
	//myFiletabla = fopen("tabla.out","w");
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


	cout << " llego aqui"<< endl;
	fclose(myFile); //Cierre del archivo de entrada modelo.in.
	fclose(myOtherFile); //Cierre del archivo de salida performance.out.	
	//fclose(myFiletabla);


	cout << " llego aqui"<< endl;
	/** Impresiones de prueba */	
	aux = numerosEnteros(mod_m);
	cout << "numero entero en el main: " << aux << endl;
	aux = 0;	
	aux = numerosEnteros(mod_m);
	cout << "numero entero en el main: " << aux << endl;
	aux = numerosEnteros(mod_m);
	cout << "numero entero en el main: " << aux << endl;		
	//cout << "variable aux:" << aux << endl; 
	//cout << "variable uniforme:" << numerosUniformes(aux, mod_m) << endl;

	//auxf = observacionAleatoria(0.2204 , 2);
	//cout << "variable auxf:" << auxf << endl; 

	cout << "antes de entrar a simulacion " << endl;
	//simulacion(s,simu_tabla,mod_m);
	cout << "salio de la simulaion" << endl;
	//generarTablaSimulacion(simu_tabla,myFile2);

	return 0;							//Se le dice al sistema que todo salio bien.
}

void generarModeloTeorico(int numeroModelo, Simulacion s, FILE *myfile){	
			
	fprintf(myfile,"Modelo %d: \n", numeroModelo);
	fprintf(myfile,"                 Utilizacion          L          Lq          W          Wq\n");
	fprintf(myfile,"Simulacion       xx.xx                xx.xx      xx.xx      xx.xx       xx.xx\n");
	fprintf(myfile,"Teorico          %.5f             %.5f    %.5f    %.5f     %.5f\n\n",s.PorcentajeUtilizacion(4,4,s.ro(),s.getA()),s.L(4,s.ro()),s.Lq(4,s.ro()),s.W(4,s.ro(),s.getA(),s.getB()),s.Wq(4,s.ro(),s.getA()));
	
}

void generarTablaSimulacion(list<tabla> simu, FILE *myfiletabla){

	tabla simu_aux;
	//list<tabla>::iterator it;
	//it = simu.begin();

	printf("entro al generarTablaSimulacion \n ");
	fprintf(myfiletabla,"| tiempo t | N(t) | Rllegada | sig_tiempo_llegada | rservicio1 |");
	fprintf(myfiletabla,"| sig_tiempo_salida1 | sig_llegada ");
	fprintf(myfiletabla,"| sig_salida | sig_evento |");

	/*
	cout << "| tiempo t | N(t) | Rllegada | sig_tiempo_llegada | rservicio1 | rservicio2 | rservicio3 | rservicio4 |";
	cout << "| sig_tiempo_salida1 | sig_tiempo_salida2 | sig_tiempo_salida3 | sig_tiempo_salida4 | sig_llegada ";
	cout << "| sig_salida | sig_evento |" << endl;
	*/
	/*
	while ( it != simu.end()){
		simu_aux = simu.front();
		cout << "|" << simu_aux.tiempo <<"|"<< simu_aux.nclientes <<"|"<< simu_aux.rllegada ;
		cout <<"|"<< simu_aux.sig_tiempo_llegada <<"|"<< simu_aux.rservicio <<"|"<< simu_aux.sig_tiempo_salida ;
		cout <<"|"<< simu_aux.sig_llegada <<"|"<< simu_aux.sig_salida <<"|"<< simu_aux.sig_evento << endl; 
		it++;
	}
	*/
}

int numerosEnteros(int mod_m){
	int hora = 0;
	hora = time(NULL);			//Inicializa variable hora con la del sistema
	srand(hora);						//Usa la hora del sistema como semilla
	return (1 + rand() % (mod_m -1) );
}


float numerosUniformes(int num_e, int mod_m) { return ((num_e + 0.5) / mod_m); }

float observacionAleatoria(float r, float media){ return ((- 1 / media) * log(1-r)); }

void simulacion(Simulacion s, list<tabla>  &simu_tabla, int mod_m){

	tabla simu_aux;
	simu_aux.tiempo = 0;
	simu_aux.sig_evento = 'L';
	simu_aux.nclientes = 0;
	simu_aux.s_ocupados = 0;

	int auxi = 0;
 	int j=0;
	//cout << "simu_aux-sig_enveto" << simu_aux.sig_evento << endl;
	//simu_tabla.push_front(simu_aux);

	//list<tabla>::iterator it;
	//it = simu_tabla.begin();

	int i, replicas = s.getReplicas();


	cout << " dentro a la simulacion" << endl;

	for (i = 0; i < replicas; ++i){
		//cout << "paso una replica" << endl;
		
		while(s.getTsimulacion() > simu_aux.tiempo) {
			j++;
			auxi = 0;

			if (simu_aux.sig_evento = 'L'){ // si el evento es una llegada
				
				cout << " Dentro del evento llegada L " << endl;


					auxi = numerosEnteros(mod_m);

					cout << "numero entero generado: " << auxi << endl;
					simu_aux.rllegada = numerosUniformes(auxi,mod_m);
					simu_aux.sig_tiempo_llegada = observacionAleatoria(simu_aux.rllegada,s.getA());
					simu_aux.sig_llegada = simu_aux.sig_tiempo_llegada + simu_aux.tiempo;

				if (simu_aux.nclientes >= 1){ // cuando existen clientes en el sistema

					cout << " Cuando existen mas de 1 cliente en el sistema " << endl;


					if (simu_aux.s_ocupados < 4){ //si hay servidores libres

							cout << " cuando hay servidores libres" <<endl;
						simu_aux.rservicio.push_back(numerosUniformes(numerosEnteros(mod_m),mod_m));
						simu_aux.sig_tiempo_salida.push_back(observacionAleatoria(simu_aux.rllegada,s.getB()));
					
						simu_aux.sig_tiempo_salida.sort();	//ordena los tiempos de salida
						simu_aux.sig_salida = simu_aux.sig_tiempo_salida.front() + simu_aux.tiempo; // le asigna el tiempo mas cercano
						simu_aux.s_ocupados++;

					}else{ //si no hay servidores libres
						
						cout << "cuando no hay servidores libres " << endl;
						simu_aux.sig_salida = simu_aux.sig_tiempo_salida.front();
					}

					// define cual es el proximo evento
					if (simu_aux.sig_llegada < simu_aux.sig_salida){
						simu_aux.sig_evento = 'L';
						simu_aux.tiempo = simu_aux.sig_tiempo_llegada;
					}else{
						simu_aux.sig_evento = 'S';
						simu_aux.tiempo = simu_aux.sig_tiempo_salida.front();
					}	
	

				}else{ // si es el primer cliente del sistema

					cout << " si es el primer cliente " << endl;

					simu_aux.sig_evento = 'L';
					//simu_aux.rservicio.push_back(1000);
					//simu_aux.sig_tiempo_salida.push_back(1000);
					//simu_aux.sig_salida = -1;
					simu_aux.s_ocupados++;
				}

				//simu_tabla.push_back(simu_aux);
				simu_aux.nclientes = simu_aux.nclientes + 1;
				//simu_aux.nclientes++;
				

				cout << " Salio del evento llegada " << endl;

			}else{ // si el evento es una salida

				simu_aux.rservicio.push_back(numerosUniformes(numerosEnteros(mod_m),mod_m));
				simu_aux.sig_tiempo_salida.push_back(observacionAleatoria(simu_aux.rllegada,s.getB()));

				simu_aux.sig_tiempo_salida.sort();	//ordena los tiempos de salida
				simu_aux.sig_salida = simu_aux.sig_tiempo_salida.front() + simu_aux.tiempo; // le asigna el tiempo mas cercano
				simu_aux.s_ocupados++;

				// define cual es el proximo evento
				if (simu_aux.sig_llegada < simu_aux.sig_salida){
					simu_aux.sig_evento = 'L';
					simu_aux.tiempo = simu_aux.sig_tiempo_llegada;
				}else{
					simu_aux.sig_evento = 'S';
					simu_aux.tiempo = simu_aux.sig_tiempo_salida.front();
				}

				//simu_tabla.push_back(simu_aux);
				simu_aux.nclientes = simu_aux.nclientes - 1;

			}	
			// guarda la iteracion en la tabla de simulacion
			simu_tabla.push_back(simu_aux);

			cout << "| tiempo t | N(t) | Rllegada | sig_tiempo_llegada | rservicio1 |";
			cout << "| sig_tiempo_salida1 | sig_llegada ";
			cout << "| sig_salida | sig_evento |" << endl << endl << endl;			

			cout << "|" << simu_aux.tiempo <<"|"<< simu_aux.nclientes -1 <<"|"<< simu_aux.rllegada ;
			cout <<"|"<< simu_aux.sig_tiempo_llegada <<"|"<< simu_aux.rservicio.front() <<"|"<< simu_aux.sig_tiempo_salida.front() ;
			cout <<"|"<< simu_aux.sig_llegada <<"|"<< simu_aux.sig_salida <<"|"<< simu_aux.sig_evento << endl; 

			//cout << endl << endl << " llego al break " << endl;
			//if (j==2) break;

		}// fin del while de la simulacion
	} // fin de la replica
}

