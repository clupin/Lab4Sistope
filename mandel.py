
#parametros
#espacio de muestreo [a,b]X[c,d]
import math
import ctypes
a = 0
b = -1
c = 1
d = 0
muestreo = .01
depth=500
def calcularMatriz(a,b,c,d,muestreo):
	Ysize = (c-a)/muestreo+1
	Xsize = (d-b)/muestreo+1
	return Ysize,Xsize
def cuad(p):
	real=p[0]
	imag=p[1]
	p1 = real*real#r
	p2 = real*imag#i 
	p3 = imag*real#i
	p4 = -1*imag*imag#r
	return p1+p4 , p2+p3

def sumCR(p,r):
	return p[0]+r[0] , p[1]+r[1]
def mod_(zn):
	a2=zn[0]*zn[0]
	b2=zn[1]*zn[1]
	return(math.sqrt(a2+b2))

t = calcularMatriz(a,b,c,d,muestreo)
print int(t[0])

Matrix = [[0 for x in range(int(t[0]))] for x in range(int(t[1]))]


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


fun()
from array import array
file = open("output.raw", "wb+")


for y in range(int(t[0])):
		for x in range(int(t[1])):
			float_array = array('f', [Matrix[x][y]])
			float_array.tofile(file)
file.close()
print t
print 'finalizado'
