/* Verificación de modos del campo */
/* 15 Septiembre 2016 */
/* Luis Gregorio Navarro Rodríguez */

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define N 2

// Variables de generación del campo
double kappa,nu,phi,delta,bgamma,deltakappa,deltanu,deltaphi,random1,random2,random3,phase1,phase2;
double thetai,ki,phii;
float R;
int i,j,n;
double k[3],e1[3],e2[3];
int N_k, N_n, N_p, N_o; //valores máximos de kappa,nu, phi y omega respectivamente
//Variables de Runge-Kutta
double k1[2], k2[2], k3[2], k4[2], k5[2], k6[2];
double a2, a3, a4, a5, a6, b21, b31, b32, b41, b42, b43, b51, b52, b53, b54, b61, b62, b63, b64, b65, c1s, c2s, c3s, c4s, c5s, c6s;
double c1,c2,c3,c4,c5,c6;
double D0;
double D1[2],x,v,t,xn_1,vn_1,xs_1,vs_1,h;
int n,i;
double dxdt, dvdt, dydx[2];
//Parámetros
double E_const, omega_0,c,e,m;
double E_vaco, E_temp;
//Variables de graficación, manejo y procesamiento de datos
int histograma[200000];
//Funciones
double f(double ti, double xi, double vi, int n);
double modos();
double E_vac(double tp);
void derivs(float t,float y[],float dydx[]);

/****************** Ecuación diferencial a resolver *****************/
double f(double ti, double xi, double vi, int n) 
{
	double k=1; 
	dxdt = vi;    		    // dx/dt = v
	dvdt = -k*xi-0.1*vi+E_temp; // dv/dt = -kx-bv+E
	
	dydx[0] = dxdt;
	dydx[1] = dvdt;
		
	return dydx[n];
}

// Campo en la componente x
double E_vac(double tp)
{

	modos();
	E_vaco = E_const*(e1[0]*cos(omega_0*tp-phase1)+e2[0]*cos(omega_0*tp-phase2));
	//printf("%f\n",E_vaco);
	return E_vaco;
}

double modos()
{
	  random1 = 2*(1)*(rand()/((double)RAND_MAX+1))-1;      // Entre -1 y 1
	  random2 = 2*(M_PI)*(rand()/((double)RAND_MAX+1));     // Entre 0 y 2Pi
	  random3 = 2*(M_PI)*(rand()/((double)RAND_MAX+1));     // Entre 0 y 2Pi

// Debug
/*
	printf("%f\n",random1);
	printf("%f\n",random2);
	printf("%f\n",random3);
*/

/******Generación de los modos del campo******/

// Números para muestrear los modos k
	  kappa=(pow((omega_0-delta/2),3))/(3*pow(c,3)) + (i-1)*deltakappa;
	  nu = random1;
	  phi =random2;

	  ki = pow((3*kappa),(1/3));
	  thetai = acos(nu);
	  phii = phi;

// Vector k
	  k[0] = ki*sin(thetai)*cos(phii);
	  k[1] = ki*sin(thetai)*sin(phii);
	  k[2] = ki*cos(thetai);

	  //printf("%f, %f, %f\n",k[0],k[1],k[2]);


// Polarizaciones
	  double xi = random3;

	  e1[0] = cos(thetai)*cos(phii)*cos(xi) - sin(phii)*sin(xi);
	  e1[1] = cos(thetai)*sin(phii)*cos(xi) + cos(phii)*sin(xi);
	  e1[2] = -sin(thetai)*cos(xi);

	  e2[0] = -cos(thetai)*cos(phii)*cos(xi) - sin(phii)*sin(xi);
	  e2[1] = -cos(thetai)*sin(phii)*cos(xi) + cos(phii)*sin(xi);
	  e2[2] = sin(thetai)*cos(xi);

	  double phase1 = 2*(M_PI)*(rand()/((double)RAND_MAX+1));
	  double phase2 = 2*(M_PI)*(rand()/((double)RAND_MAX+1)); 
}
