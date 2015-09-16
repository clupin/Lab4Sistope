#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

void instrucciones();
float *calcularMatriz(float a, float b, float c, float d, float muestreo);
float *cuad(float *p);
float *sumCR(float *p, float *r);
float mod(float *zn);

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
        instrucciones();
        exit(EXIT_FAILURE);
    }

    if (cortar == 1) {
        printf("Se desea cortar el archivo: %s en: %i partes\n", nombre, cantidadArchivos);
        
        if(!fork()){
            char * args[]= {"cut.o",nombre,numero};
            int result = execve("cut.o", args, NULL);
            if (result < 0) {
                perror("error xor ");
            }
        }
        wait();
    }
    if (juntar == 1) {
        printf("Se desea unir el archivo: %s\n", nombre);
        if(!fork()){
            char * args[]= {"join.o",nombre};
            int result = execve("cut.o", args, NULL);
            if (result < 0) {
                perror("error JOIN ");
            }
        }
        wait();
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

float *calcularMatriz(float a, float b, float c, float d, float muestreo){
    float* xy=malloc(2*sizeof(float));
    xy[1] = (c-a)/muestreo+1;
    xy[0] = (d-b)/muestreo+1;

    return xy;
}

float *cuad(float *p){
    float* aux=malloc(2*sizeof(float));
    float real = p[0];
    float imag = p[1];
    aux[0] = real*real + (-1*imag*imag);
    aux[1] = real*imag + (mag*real);
    return aux;
}

float *sumCR(float *p, float *r){
    float* aux=malloc(2*sizeof(float));
    aux[0]=p[0]+r[0];
    aux[1]=p[1]+r[1];
    return aux;
}

float mod(float *zn){
    float a2 = zn[0]*zn[0];
    float b2 = zn[1]*zn[1];
    float aux = math.sqrt(a2+b2);
    return aux;
}

def fun():
    for y in range(int(t[1])):
        for x in range(int(t[0])):
            z0 = 0
            n = 1
            X = a+x*muestreo
            Y = b+y*muestreo
            zn = [X,Y]
            c = [X,Y]
            while mod_(zn)<2 and n < depth:
                zn = sumCR(c,cuad(zn))
                n=n+1
            Matrix[y][x]=math.log(n)