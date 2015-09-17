#include <unistd.h>//importante para que funcione pthread
#include <stdio.h>
#include <sys/ipc.h>//shared memory
#include <sys/shm.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <time.h>

void instrucciones();
//creo que calcula matriz deberia entregar un entero, es el tamaño para crear la matriz

int *calcularMatriz(double a, double b, double c, double d, double m);
double *cuad(double *p, double *aux);
double *sumCR(double *p, double *r, double *aux);
float *matrixGen(int size);
double mod(double *zn);
void mandelbrot(float* M,long size, double a , double b, double m, int depth);
void generaArchivo(float* M, char* path, int size);
void initMandelbrot(int p, double a,double b,double c,double d,double s,char* f);
//variables globales
key_t key;
int shmid;

int main(int argc, char *argv[]) {
    int opt= 0;
    int correcto=0;
    int help = 0;
    int p;
    double a,b,c,d,s;
    char *f;
    /*entradas;
    • -p: depth, o n´umero m´aximo de iteraciones
    • -a: limite inferior del componente real del plano complejo
    • -b: l´ımite inferior componente imaginario
    • -c: l´ımite superior componente real
    • -d: l´ımite superior componente imaginario
    • -s: m
    • -f: archivo de salida
    */
    while((opt = getopt(argc, argv, "p:a:b:c:d:s:f:")) != -1) {
        switch (opt) {
             case 'p' :
                    p=atoi(optarg);
                    correcto++;
                    printf("p was %d\n", p);
                    break;
             case 'a' : 
                    a=atof(optarg);
                    correcto++;
                    printf("a was %f\n", a);
                    break;
             case 'b' :
                    b=atof(optarg);
                    correcto++;
                    printf("b was %f\n", b);
                    break;
             case 'c' :
                    c=atof(optarg);
                    correcto++;
                    printf("c was %f\n", c);
                    break;
             case 'd' :
                    d=atof(optarg);
                    correcto++;
                    printf("d was %f\n", d);
                    break;
             case 's' :
                    s=atof(optarg);
                    correcto++;
                    printf("s was %f\n", s);
                    break;
             case 'f' :
                    f=optarg;
                    correcto++;
                    printf("f was %s\n", f);
                    break;
             default: 
                    instrucciones(); 
                    exit(EXIT_FAILURE);
                    break;
        }
    }
    if(correcto==7){
        printf("estan todos los datos\n");
        initMandelbrot(p,a,b,c,d,s,f);
        //int* t = calcularMatriz(a,b,c,d,s);
        //printf("accediendo a t[0]:%d\n",t[0] );
    } else {
        printf("Faltan datos que entregar\n");
        printf("Recuerda que la forma correcta es, siguiendo este ejemplo:\n");
        printf("./mandelbrotp -p 500 -a -1 -b -1 -c 1 -d 1 -s 0.001 -f salida.raw\n");
    }
    return 0;
}

void instrucciones() {
    printf("Para utilizar el programa, es necesario escribir: \n./nombrePrograma.o --name nombreArchivo --cut cantidadPartesQueDeseaCortar\n");
    printf("O si se desea juntar\n");
    printf("Para utilizar el programa, es necesario escribir: \n./nombrePrograma.o --name nombreArchivo --join\n");
}

int* calcularMatriz(double a, double b, double c, double d, double m){
    int* xy=malloc(2*sizeof(int));
    double xy_0 = (c-a)/m+1;
    double xy_1 = (d-b)/m+1;
    xy[0]=(int) xy_0;
    xy[1]=(int) xy_1;
    return  xy;
}

double* cuad(double *p, double *aux){
    
    double real = p[0];
    double imag = p[1];
    aux[0] = real*real + (-1*imag*imag);
    aux[1] = 2*real*imag;
    return aux;
}

double* sumCR(double *p, double *r, double *aux){
    
    aux[0]=p[0]+r[0];
    aux[1]=p[1]+r[1];
    return aux;
}

double mod(double *zn){
    double a2 = zn[0]*zn[0];
    double b2 = zn[1]*zn[1];
    double aux = sqrt(a2+b2);//creo que math hay que importarlo aqui no funciona math.sqrt eso es POO
    return aux;
}
float *matrixGen(int size){
    //conexion a memoria virtual
    
    key = ftok("/dev/null", '1');
    shmid = shmget(key, size*size*sizeof(float), IPC_CREAT | 0666);
    printf("%d\n", shmid );
    void* segment=shmat(shmid, NULL, IPC_CREAT | 0666);
    //double** M= malloc(sizeof(double*)*size);
    float* M=(float*) segment;
    return M;
}
void mandelbrot(float* M,long size, double a , double b, double m, int depth){
                        //el size podria ser int... no se ha que hacer las consideraciones necesarias
    int y,x,i;
    double* zn = malloc(sizeof(double)*2);
    double* c = malloc(sizeof(double)*2);
    //se implementan auxiliares usados por las funciones para no reservar memoria innecesaria
    double* auxCuad = malloc(sizeof(double)*2);
    double* auxSum = malloc(sizeof(double)*2);
    //aqui hay que reservar la memoria de los auxiliares que ocupan sumCR y cuad

    for (y = 0; y < size; y++)
    {
       for (x = 0; x < size; x++)
       {
            double z0 = 0;// para efetos del algoritmo creo que este es irrelevante
            int n = 1;// este calcula las iteraciones necesarias para la cosa
            double X = a+y*m;
            double Y = b+x*m;
            zn[0] = X;
            zn[1] = Y;
            c[0] = X;
            c[1] = Y;
            while (mod(zn)<2 && n < depth){
                zn = sumCR(c,cuad(zn,auxCuad),auxSum);
                n++;
            }
            //printf("M[%d][%d]\n",y,x);
            M[y*size+x]=log(n);//este tambien hay que ver como se hace con la libreria math
       }
    }
    //no retorna nada porque como va trabajando en la direccion de la matriz del parametro no necesita retornar la matriz modificada    
}
void generaArchivo(float* M, char* path,int size){
    //imprimiendo un archivo de flotantes binarios
    FILE* archivo = fopen(path, "wb+");
    float f=0.0;
    int y,x;
    for (y = 0; y < size; ++y)
    {
        for (x = 0; x < size; ++x)
        {

            f=M[x+y*size];
            fwrite(&f,sizeof(float) , 1, archivo);//funciona
        }
    }
    fclose(archivo);//importante cerrar el archivo
    printf("-- Archivo generado --\n");
}
void initMandelbrot(int p, double a,double b,double c,double d,double s,char* f){
    //inicio del reloj
    clock_t start = clock();

    int* t = calcularMatriz(a,b,c,d,s);
    printf("[%d|%d]\n",t[0],t[1] );
    if(t[0]==t[1]){ 
        float * M=matrixGen(t[0]);
        mandelbrot(M,t[0],a , b, s, p);
        generaArchivo(M,f,t[0]);
    }
    else{
        printf("la matriz debe ser cuadrada\n");
        exit(EXIT_FAILURE);
    }
    //impresion del tiempo
    printf("Tiempo transcurrido: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);

}
