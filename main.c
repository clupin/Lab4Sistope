#include <unistd.h>//importante para que funcione pthread
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>

void instrucciones();
//creo que calcula matriz deberia entregar un entero, es el tamaño para crear la matriz
int *calcularMatriz(float a, float b, float c, float d, float m);
float *cuad(float *p);
float *sumCR(float *p, float *r);
float **matrixGen(int size);
float mod(float *zn);
void mandelbrot(float** M,long size, float a , float b, float m, int depth);
void generaArchivo(float** M, char* path, int size);
//implementar una funcion para la generacion de archivos yo creo que sería apropiado


int main(int argc, char *argv[]) {
    int opt= 0;
    /*
    0 = false
    1 = true
    */
    int cortar = 0;
    int juntar = 0;
    int help = 0;
    int cantidadArchivos;
    char *nombre, *numero;
    //struct para las opciones que existen
    /*entradas;
    • -p: depth, o n´umero m´aximo de iteraciones
    • -a: limite inferior del componente real del plano complejo
    • -b: l´ımite inferior componente imaginario
    • -c: l´ımite superior componente real
    • -d: l´ımite superior componente imaginario
    • -s: m
    • -f: archivo de salida
    */
    static struct option long_options[] = {
        {"cut",     required_argument,  0,  'c' },
        {"join",    no_argument,        0,  'j' },
        {"name",    required_argument,  0,  'n' },
        {"help",    no_argument,		0,  'h' },
        {0,         0,                  0,   0  }
    };

    int long_index =0;
    //captura de las posibles respuestas
    while ((opt = getopt_long_only(argc, argv,"", long_options, &long_index )) != -1) {
    //while(1){
        switch (opt) {
             case 'c' : 
                    cortar = 1;
                    numero = optarg;
                    cantidadArchivos = atoi(numero);
                    break;
             case 'j' : 
                    juntar = 1;
                    break;
             case 'n' :
                    nombre = optarg;
                    break;
             case 'h' :
                    help = 1;
                    break;
             default: 
                    instrucciones(); 
                    exit(EXIT_FAILURE);
                    break;
        }
    }
    if (nombre == NULL ) {
        //instrucciones();
        //generaArchivo(NULL,"file.raw",size);
        exit(EXIT_FAILURE);
    }
    if (help == 1){
    	instrucciones();
        exit(EXIT_FAILURE);
    }
    return 0;
}

void instrucciones() {
    printf("Para utilizar el programa, es necesario escribir: \n./nombrePrograma.o --name nombreArchivo --cut cantidadPartesQueDeseaCortar\n");
	printf("O si se desea juntar\n");
	printf("Para utilizar el programa, es necesario escribir: \n./nombrePrograma.o --name nombreArchivo --join\n");
}

int* calcularMatriz(float a, float b, float c, float d, float m){
    int* xy=malloc(2*sizeof(int));
    float xy_0 = (c-a)/m+1;
    float xy_1 = (d-b)/m+1;
    xy[0]=(int) xy_0;
    xy[1]=(int) xy_1;
    return  xy;
}

float* cuad(float *p){
    float* aux=malloc(2*sizeof(float));
    float real = p[0];
    float imag = p[1];
    aux[0] = real*real + (-1*imag*imag);
    aux[1] = 2*real*imag;
    return aux;
}

float* sumCR(float *p, float *r){
    float* aux=malloc(2*sizeof(float));
    aux[0]=p[0]+r[0];
    aux[1]=p[1]+r[1];
    return aux;
}

float mod(float *zn){
    float a2 = zn[0]*zn[0];
    float b2 = zn[1]*zn[1];
    float aux = sqrt(a2+b2);//creo que math hay que importarlo aqui no funciona math.sqrt eso es POO
    return aux;
}
float **matrixGen(int size){
    float** M= malloc(sizeof(float*)*size);
    int i;
    for (i = 0; i < size; ++i)
    {
        M[i]=malloc(sizeof(float)*size);
    }
    return M;
}
void mandelbrot(float** M,long size, float a , float b, float m, int depth){
                        //el size podria ser int... no se ha que hacer las consideraciones necesarias
    int y,x;
    for (y = 0; y < size; ++y)
    {
       for (int i = 0; i < size; ++i)
       {
            float z0 = 0;// para efetos del algoritmo creo que este es irrelevante
            int n = 1;// este calcula las iteraciones necesarias para la cosa
            float X = a+x*m;
            float Y = b+y*m;
            float* zn = malloc(sizeof(float)*2);
            zn[0] = X;
            zn[1] = Y;
            float* c = malloc(sizeof(float)*2);
            c[0] = X;
            c[1] = Y;
            while (mod(zn)<2 && n < depth){
                zn = sumCR(c,cuad(zn));
                n++;
            }
            M[y][x]=log(n);//este tambien hay que ver como se hace con la libreria math
       }
    }
    //no retorna nada porque como va trabajando en la direccion de la matriz del parametro no necesita retornar la matriz modificada    
}
void generaArchivo(float** M, char* path,int size){
    //imprimiendo un archivo de flotantes binarios
    FILE* archivo = fopen(path, "wb+");
    float f=1.5;
    int y,x;
    for (y = 0; x < size; ++y)
    {
        for (x = 0; x < size; ++x)
        {
            f=M[x][y];
            fwrite(&f,sizeof(float) , 1, archivo);//funciona
        }
    }
    fclose(archivo);//importante cerrar el archivo
    printf("-- Archivo generado --\n");
}

