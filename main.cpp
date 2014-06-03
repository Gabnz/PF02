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
#include<time.h> //Libreria para usar la hora del sistema (semilla).
#include<math.h> //Para calcular ln(r).
#include<string>

const int MAX = 10000; 

//Para no incluir todo el espacio de nombres (using namespace std;) se incluye solo lo que se utilizara... 
using std::cin; //Para entradas.
using std::cout; //Para salidas.
using std::endl; //Para saltos de linea.
using std::list; //Para la lista.


//Metodos para la simulacion.
void impresionPerformance(int numeroModelo, Simulacion s, FILE *myfile, float U, float L, float Lq, float W, float Wq); //Modelo teorico completo...
int numerosEnteros(int mod_m);
float numerosUniformes(int num_e, int mod_m);
float observacionAleatoria(float r, float media);
void simulacion(Simulacion s, float *U, float *L, float *Lq, float *W, float *Wq, int mod_m);

//Funcion principal del proyecto.
int main(){
	//Se utiliza la hora del sistema como semilla...
	srand(time(NULL)); //Inicializacion del tiempo para generar numeros aleatorios...
					
	// variables de prueba para imprimir..test
	float auxf=0;
	int aux=0;


	const int mod_m = 1000;
	
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
	myOtherFile = fopen("performance.out","w");	//Apertura del archivo de salida performance.out con permisos de escritura solamente.
	Simulacion s; //Variable de clase Simulacion con la cual se va a interactuar en las simulaciones.	
	float U=0,
		  L=0,
		  Lq=0,
		  W=0,
		  Wq=0; //Variables para el performance de la simulacion.
	
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
		
		simulacion(s,&U,&L,&Lq,&W,&Wq,mod_m); //Llamada al proceso de simulacion.
		impresionPerformance(i+1,s,myOtherFile,U,L,Lq,W,Wq); //Llamada para generar archivo de salida performance.out.
		
		getchar(); //Prueba.
	}

	fclose(myFile); //Cierre del archivo de entrada modelo.in.
	fclose(myOtherFile); //Cierre del archivo de salida performance.out.	
		

	return 0;							//Se le dice al sistema que todo salio bien.
}

//Impresion del archivo performance.out. 
void impresionPerformance(int numeroModelo, Simulacion s, FILE *myfile, float U, float L, float Lq, float W, float Wq){	
			
	fprintf(myfile,"Modelo %d: \n", numeroModelo);
	fprintf(myfile,"                 Utilizacion          L          Lq          W          Wq\n");
	fprintf(myfile,"Simulacion       %.5f             %.5f    %.5f    %.5f     %.5f\n",U,L,Lq,W,Wq);
	fprintf(myfile,"Teorico          %.5f             %.5f    %.5f    %.5f     %.5f\n\n",s.PorcentajeUtilizacion(4,4,s.ro(),s.getA()),s.L(4,s.ro()),s.Lq(4,s.ro()),s.W(4,s.ro(),s.getA(),s.getB()),s.Wq(4,s.ro(),s.getA()));
	
}

//Generacion aleatoria de numeros enteros.
int numerosEnteros(int mod_m){	return (rand() % mod_m ); } 

//Generacion de numeros uniformes mediante el metodo aditivo.
float numerosUniformes(int num_e, int mod_m) { return ((num_e + 0.5) / mod_m); }

//Generacion de la observacion aleatoria mediante el metodo inverso.
float observacionAleatoria(float r, float media){ return ((- 1 / media) * log(1-r)); }

//Simulacion del sistema de colas para reunir observaciones estadisticas mediante el metodo de replicas.
void simulacion(Simulacion s, float *U, float *L, float *Lq, float *W, float *Wq, int mod_m){
	
	int i; // Contador para replicas.
	float tiempo; //Tiempo T.
	char sigEvento = 'L'; //Siguiente evento.	
	float valorR1, valorR2, valorR3, valorR0;
	float valorS1, valorS2, valorS3, valorS0;
	float rLlegada,calculo; 
	float u=0,l=0,lq=0,w=0,wq=0;
	bool banderal=true,banderalq=true,banderaw=true,banderawq=true;
	bool banderaTSalida;	
	float sigTiempo, sigTLlegada, sigTSalida;
	int cantidadClientes;
	int servidoresOcupados;
	list<float>::iterator it;
	list<float> tiemposLlegada;
	FILE *simulationFile;
	simulationFile = fopen("simulacion.out","w");
	for(i = 0; i < s.getReplicas(); i++){
		sigTiempo = 0;
		tiempo = 0;		
		cantidadClientes = 0;
		servidoresOcupados = 0;

		
		float rServicio;
		float aleatorio;
		aleatorio = 0;
		banderaTSalida = true;		
		
		
		
		valorR1 = 0;			
		valorR2 = 0;
		valorR3 = 0;
		valorR0 = 0;
		valorS1 = 0;
		valorS2 = 0;
		valorS3 = 0;
		valorS0 = 0;
		//getchar();
		fprintf(simulationFile,"| tiempo|N(t)|  rl  | sig_tl|  rs1 |  rs2 |  rs3 |  rs4 | s_ts1| s_ts2");
		fprintf(simulationFile,"| s_ts3| s_ts4| s_ll | s_sl | s_eve | \n");
		while(s.getTsimulacion() > tiempo){		
			
				if(sigEvento == 'L'){ //Si es una llegada.
				
					//Calculo del tiempo t(n) y tiempo de la siguiente llegada.
					if(sigTiempo > tiempo)
						tiempo = sigTiempo;
					rLlegada = numerosUniformes(numerosEnteros(mod_m),mod_m);
					sigTLlegada = observacionAleatoria(rLlegada,s.getA());
					sigTiempo = sigTiempo + sigTLlegada;
					
					
					if(cantidadClientes > 0){ //Cuando hay al menos 1 cliente.
						
						//Calculo del tiempo de servicio.
						rServicio = numerosUniformes(numerosEnteros(mod_m),mod_m);
						aleatorio = observacionAleatoria(rServicio,s.getB()); 
						
						if(servidoresOcupados == 0){											
							valorR0 = aleatorio;
							valorS0 = rServicio;												
						}else{
							if(servidoresOcupados == 1){																	
								valorR1 = aleatorio;												
								valorS1 = rServicio;
							}else{
								if(servidoresOcupados == 2){									
									valorR2 = aleatorio;												
									valorS2 = rServicio;
								}else{
									if(servidoresOcupados == 3){														
										valorR3 = aleatorio;												
										valorS3 = rServicio;
									}else{
										servidoresOcupados--;											
										
									} //Fin if SO = 3.
								} //Fin if SO = 2
							}	//Fin if SO = 1
						} //Fin if SO = 0
						servidoresOcupados++;
						
					if(banderaTSalida)	
						sigTSalida = aleatorio + tiempo;
						
					
					tiemposLlegada.push_back(sigTiempo);
					
					if(tiemposLlegada.front() < sigTSalida){
						sigEvento = 'L';
						tiemposLlegada.remove(tiemposLlegada.back());
						banderaTSalida = false;
					}else{
						sigEvento = 'S';		
						banderaTSalida = true;				
					}				
					}//Fin if cantidadClientes.
					
						
					
					cantidadClientes++;
				//	cout << "Tiempo entre llegadas " << sigTLlegada << " tiempo tn" << tiempo << endl;
					//cout << "aleatoriooo" <<  aleatorio << endl;
					//cout << "siguiente salida" << sigTSalida << endl;	
					//cout << "tiemposLlegada.front" << tiemposLlegada.front() << endl;
					//	getchar();
					
				}else{ //Si es una salida.
								
					cantidadClientes--;			
									
					//Genero el nuevo tiempo.
					tiempo = sigTSalida;
				
					//Genero la nueva salida.
					rServicio = numerosUniformes(numerosEnteros(mod_m),mod_m);
					aleatorio = observacionAleatoria(rServicio,s.getB()); 
				
					sigTSalida = tiempo + aleatorio;
				
				
					servidoresOcupados--;
					
					
					
					//Proximo evento.
					if(!tiemposLlegada.empty()){ //Si hay tiempos de llegada.
						if(tiemposLlegada.front() < sigTSalida){
							sigEvento = 'L';	
							tiemposLlegada.remove(tiemposLlegada.back());
							sigTiempo = tiempo;													
						}else{
							sigEvento = 'S';																
						}
					}else{
						sigEvento = 'L';		
						sigTiempo = tiempo;
					}
				} //Fin if sigEvento.
				
				
				fprintf(simulationFile, "| %.3f | %d  |%.3f |%.3f  ",tiempo,cantidadClientes,rLlegada,sigTLlegada );
				
				if(valorS0 == 0){
					fprintf(simulationFile, "|  -   ");
				}else{
					fprintf(simulationFile, "| %.3f",valorS0);
				}
				if(valorS1 == 0){
					fprintf(simulationFile, "|  -   ");
				}else{
					fprintf(simulationFile, "| %.3f",valorS1);
				}
				if(valorS2 == 0){
					fprintf(simulationFile, "|  -   ");
				}else{
					fprintf(simulationFile, "| %.3f",valorS2);
				}
				if(valorS3 == 0){
					fprintf(simulationFile, "|  -   ");
				}else{
					fprintf(simulationFile, "| %.3f",valorS3);
				}
				
				if(valorR0 == 0){
					fprintf(simulationFile, "|  -   ");
				}else{
					fprintf(simulationFile, "| %.3f",valorR0);
				}
				if(valorR1 == 0){
					fprintf(simulationFile, "|  -   ");
				}else{
					fprintf(simulationFile, "| %.3f",valorR1);
				}
				if(valorR2 == 0){
					fprintf(simulationFile, "|  -   ");
				}else{
					fprintf(simulationFile, "| %.3f",valorR2);
				}
				if(valorR3 == 0){
					fprintf(simulationFile, "|  -   ");
				}else{
					fprintf(simulationFile, "| %.3f",valorR3);
				}
				
				fprintf(simulationFile,"| %.3f| %.3f|   %c   |\n",sigTiempo,sigTSalida,sigEvento);	
				valorR1 = 0;			
				valorR2 = 0;
				valorR3 = 0;
				valorR0 = 0;
				valorS1 = 0;
				valorS2 = 0;
				valorS3 = 0;
				valorS0 = 0;
				
			
		}//Fin while.
		
		
		
		
	}//Fin for replicas.
	fclose(simulationFile);
}// fin simulacion

