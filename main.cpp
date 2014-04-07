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
#include<list>	//Libreria de lista.
#include<time.h> //Libreria para usar hora del sistema (semilla).
#include<math.h> //Para calcular ln(r).

//Para no incluir todo el espacio de nombres (using namespace std;) se incluye solo lo que se utilizara... 
using std::cin; //Para entradas.
using std::cout; //Para salidas.
using std::endl; //Para saltos de linea.
using std::list; //Para la lista.


// Se define estructura tabla de la simulacion.
struct tabla {
 	float tiempo, rllegada, sig_tiempo_llegada,  sig_llegada, sig_salida; 
	int nclientes, s_ocupados;
	list<float> rservicio, sig_tiempo_salida;
	char sig_evento; // L: llegada y S: salida.
};

void impresionPerformance(int numeroModelo, Simulacion s, FILE *myfile); //Modelo teorico completo...
int numerosEnteros(int mod_m);
float numerosUniformes(int num_e, int mod_m);
float observacionAleatoria(float r, float media);
void simulacion(Simulacion s, list<tabla>  &simu_tabla, int mod_m, FILE *myfiletabla);

//Funcion principal del proyecto.
int main(){
	//Se utiliza la hora del sistema como semilla...
	srand(time(NULL)); //Inicializacion del tiempo para generar numeros aleatorios...
					
	// variables de prueba para imprimir..test
	float auxf=0;
	int aux=0;


	list<tabla> simu_tabla;

	const int mod_m = 1000;
	const int MAX = 1000000;
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
	myFiletabla = fopen("tabla.out","w");
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
		
		impresionPerformance(i+1,s,myOtherFile); //Llamada para generar archivo de salida performance.out.
		
		getchar(); //Prueba.
	}

	fclose(myFile); //Cierre del archivo de entrada modelo.in.
	fclose(myOtherFile); //Cierre del archivo de salida performance.out.	
	
	simulacion(s,simu_tabla,mod_m,myFiletabla);
	fclose(myFiletabla);

	return 0;							//Se le dice al sistema que todo salio bien.
}

void impresionPerformance(int numeroModelo, Simulacion s, FILE *myfile){	
			
	fprintf(myfile,"Modelo %d: \n", numeroModelo);
	fprintf(myfile,"                 Utilizacion          L          Lq          W          Wq\n");
	fprintf(myfile,"Simulacion       xx.xx                xx.xx      xx.xx      xx.xx       xx.xx\n");
	fprintf(myfile,"Teorico          %.5f             %.5f    %.5f    %.5f     %.5f\n\n",s.PorcentajeUtilizacion(4,4,s.ro(),s.getA()),s.L(4,s.ro()),s.Lq(4,s.ro()),s.W(4,s.ro(),s.getA(),s.getB()),s.Wq(4,s.ro(),s.getA()));
	
}

//Generacion aleatoria de numeros enteros.
int numerosEnteros(int mod_m){	return (rand() % mod_m ); } 

//Generacion de numeros uniformes mediante el metodo aditivo.
float numerosUniformes(int num_e, int mod_m) { return ((num_e + 0.5) / mod_m); }

//Generacion de la observacion aleatoria mediante el metodo inverso.
float observacionAleatoria(float r, float media){ return ((- 1 / media) * log(1-r)); }

//Simulacion del sistema de colas para reunir observaciones estadisticas mediante el metodo de replicas.
void simulacion(Simulacion s, list<tabla>  &simu_tabla, int mod_m, FILE *myfiletabla){

	tabla simu_aux; //Tabla auxiliar.
	
	//Inicializacion de la tabla.
	simu_aux.tiempo = 0;
	simu_aux.sig_llegada = 0;
	simu_aux.sig_tiempo_llegada = 0;
	simu_aux.sig_evento = 'L';
	simu_aux.nclientes = 0;
	simu_aux.s_ocupados = 0;

	list<float>::iterator it; //Iterator para recorrer las listas.
	int i, j=0, replicas = s.getReplicas();

	//Impresion de las cabeceras del modelo de salida.
	fprintf(myfiletabla,"| tiempo|N(t)|  rl  | sig_tl|  rs1 |  rs2 |  rs3 |  rs4 | s_ts1| s_ts2");
	fprintf(myfiletabla,"| s_ts3| s_ts4| s_ll | s_sl | s_eve | \n");

	for (i = 0; i < replicas; i++){ //Ciclo de replicas a realizar.
		
		while(s.getTsimulacion() > simu_aux.tiempo) { //Mientras no haya transcurrido el tiempo de la simulacion.

			/* ////////////////////////////////////////////////////////////////////////////////////////////////////*/
			/* 												COMIENZA entrada                                       */
			/* ////////////////////////////////////////////////////////////////////////////////////////////////////*/

				//cout << "simu_aux.sig_evento:" << simu_aux.sig_evento << endl;//Prueba.
			if (simu_aux.sig_evento == 'L'){ // Si el evento es una llegada.
				
					//cout << " Dentro del evento llegada L " << endl;//Prueba.
					
					//cout << "simu_aux.sig_llegada" << simu_aux.sig_llegada << endl;//Prueba.

					simu_aux.tiempo = simu_aux.sig_llegada;
					cout << "En llegada: simu_aux.tiempo:" << simu_aux.tiempo << endl;//Prueba.
					simu_aux.rllegada = numerosUniformes(numerosEnteros(mod_m),mod_m);
					simu_aux.sig_tiempo_llegada = observacionAleatoria(simu_aux.rllegada,s.getA());
					simu_aux.sig_llegada = simu_aux.sig_tiempo_llegada + simu_aux.tiempo;

				if (simu_aux.nclientes >= 1){ // Cuando existen clientes en el sistema.
					cout << "Numero de clientes en el sistema: " << simu_aux.nclientes << endl;//Prueba.
					getchar();//Prueba.
					//cout << " Cuando existen mas de 1 cliente en el sistema " << endl;

					if (simu_aux.s_ocupados < 4){ //Si hay servidores libres.

						cout << "Servidores ocupados: " << simu_aux.s_ocupados <<endl;//Prueba.
						simu_aux.rservicio.push_front(numerosUniformes(numerosEnteros(mod_m),mod_m));//Se generan los numeros enteros uniformes.
						simu_aux.sig_tiempo_salida.push_front(observacionAleatoria(simu_aux.rservicio.front(),s.getB()));//Se realiza la observacion aleatoria mediante el metodo inverso.
					
						simu_aux.rservicio.sort(); //Ordena los rservicio.
						simu_aux.sig_tiempo_salida.sort();	//Ordena los tiempos de salida.
						simu_aux.sig_salida = simu_aux.sig_tiempo_salida.front() + simu_aux.tiempo; //Le asigna el tiempo mas cercano.
						simu_aux.s_ocupados++; //Aumenta la cantidad de servidores ocupados.

					}else{ //Si no hay servidores libres.
						cout << "Todos los servidores estan ocupados" << endl;//Prueba.
						simu_aux.sig_salida = simu_aux.sig_tiempo_salida.front() + simu_aux.tiempo;//Se asigna el siguiente tiempo de salida.
					}

					// Define cual es el proximo evento.
					if (simu_aux.sig_llegada < simu_aux.sig_salida){ //Si el evento siguiente es una llegada:
						simu_aux.sig_evento = 'L';
						cout << "El siguiente envento es una llegada" << endl;//Prueba.
						//simu_aux.tiempo = simu_aux.sig_tiempo_llegada;
					}else{ //Si el evento siguiente es una salida.
						simu_aux.sig_evento = 'S';
						cout << "El siguiente evento es una salida" << endl;//Prueba.
						//simu_aux.tiempo = simu_aux.sig_tiempo_salida.front();
					}	
	

				}else{ // Si es el primer cliente del sistema...

					cout << "Primer cliente en el sistema" << endl; //Prueba.
					simu_aux.sig_evento = 'L'; //Siguiente evento: Llegada.
					simu_aux.s_ocupados++; //Servidores ocupados: 1.
				}

				simu_aux.nclientes = simu_aux.nclientes + 1; //Aumenta la cantidad de clientes en el sistema.
				
				//cout << " Salio del evento llegada " << endl;//Prueba.

			/* ////////////////////////////////////////////////////////////////////////////////////////////////////*/
			/* 												COMIENZA salida                                        */
			/* ////////////////////////////////////////////////////////////////////////////////////////////////////*/				

			}else{ // Si el evento es una salida.

				simu_aux.tiempo = simu_aux.sig_salida;
				cout << "En salida: simu_aux.tiempo:" << simu_aux.tiempo << endl;//Prueba.
				simu_aux.nclientes = simu_aux.nclientes - 1;

				if (simu_aux.nclientes >= 1){
					
					simu_aux.rservicio.push_front(numerosUniformes(numerosEnteros(mod_m),mod_m));
					simu_aux.sig_tiempo_salida.push_front(observacionAleatoria(simu_aux.rservicio.front(),s.getB()));
					
					simu_aux.rservicio.sort();
					simu_aux.sig_tiempo_salida.sort();	//ordena los tiempos de salida
					simu_aux.sig_salida = simu_aux.sig_tiempo_salida.front() + simu_aux.tiempo; // le asigna el tiempo mas cercano
					simu_aux.s_ocupados--;

					// define cual es el proximo evento
					if (simu_aux.sig_llegada < simu_aux.sig_salida){
						simu_aux.sig_evento = 'L';
						simu_aux.tiempo = simu_aux.sig_tiempo_llegada;
					}else{
						simu_aux.sig_evento = 'S';
						simu_aux.tiempo = simu_aux.sig_tiempo_salida.front() + simu_aux.tiempo;
					}
				}else{
					cout << "el sistema se quedo sin clientes en la cola. break()" << endl; 
					break;
				} 
				
			}	
			/* ////////////////////////////////////////////////////////////////////////////////////////////////////*/
			/* 												FIN DE LA ITERACION                                    */
			/* ////////////////////////////////////////////////////////////////////////////////////////////////////*/			

			/* ////////////////////////////////////////////////////////////////////////////////////////////////////*/
			/* 												IMPRESION DE RESULTADOS                                */
			/* ////////////////////////////////////////////////////////////////////////////////////////////////////*/

			
			//cout << endl << endl << " llego al break " << endl;
			//if (j==10) break;
//fprintf(myfiletabla,"replica %d",i);
			fprintf(myfiletabla, "| %.3f | %d  |%.3f |%.3f  ",simu_aux.tiempo,simu_aux.nclientes-1,simu_aux.rllegada,simu_aux.sig_tiempo_llegada );
			
			j=0;
			for (it = simu_aux.rservicio.begin(); it != simu_aux.rservicio.end(); ++it) {
    			fprintf(myfiletabla, "|%.3f ",*it);
    			j++;
    			//cout << "simu_aux.rservicio.front:" << *it << endl ;
			}
			while (j<4){
				fprintf(myfiletabla,"| %c    ",'-');
				j++;
			}
			
		//	fprintf(myfiletabla,"%d",j);

			j=0;	
			for (it = simu_aux.sig_tiempo_salida.begin(); it != simu_aux.sig_tiempo_salida.end(); ++it) {
    			fprintf(myfiletabla, "|%.3f ",*it);
    			j++;
    			//cout << "simu_aux.rservicio.front:" << *it << endl ;
			}
				while (j<4){
					fprintf(myfiletabla, "| %c    ",'-');
					j++;
				}

			fprintf(myfiletabla, "| %.3f| %.3f|   %c   |\n",simu_aux.sig_llegada,simu_aux.sig_salida,simu_aux.sig_evento);

			simu_tabla.push_back(simu_aux);
		}// fin del while de la simulacion		
	} // fin del for (replicas)
}// fin simulacion
