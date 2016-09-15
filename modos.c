#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "nr.h"
#include "nrutil.h"

#define N 2

double kappa,nu,phi,delta,bgamma,deltakappa,deltanu,deltaphi,random1,random2,random3,phase1,phase2;
double thetai,ki,phii;
float R;
int i,j,n;
double k[3],e1[3],e2[3];
int N_k, N_n, N_p, N_o; //valores máximos de kappa,nu, phi y omega respectivamente

float eps,hdid,hnext,htry,t,*y,*dydx,*dysav,*ysav,*yscal;
double E_const, omega_0,c,e,m;
double E_vaco;

double E_vac(double tp);
void derivs(float t,float y[],float dydx[]);

void derivs(float t,float y[],float dydx[])
{
	dydx[0]= y[1];
	dydx[1]=-m*pow(omega_0,2)*(y[0]+bgamma*y[1])+e*E_vac(t);
	//printf("%f, %f\n",t, E_vac(t));
}



// Campo en la componente x
double E_vac(double tp)
{

	E_vaco = E_const*(e1[0]*cos(omega_0*tp-phase1)+e2[0]*cos(omega_0*tp-phase2));

	return E_vaco;
}


main()
{
// Parámetros
	delta = 0.00001;
	omega_0 = 1;
	E_const = 1;

// Aleatorios
	srand(time(NULL));
	int contador = 10;
	while(contador<=1000)
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
	
	  printf("%f, %f, %f\n",k[0],k[1],k[2]);
	  contador++;
	}
	
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

/******Cash Sharp Runge-Kutta orden 5, paso variable******/

	y=vector(1,N);
	dydx=vector(1,N);
	dysav=vector(1,N);
	ysav=vector(1,N);
	yscal=vector(1,N);
	derivs(t,ysav,dysav);
	for (i=1;i<=N;i++) yscal[i]=1.0;
	htry=0.6;
	for (i=1;i<=15;i++) {
		eps=exp((double) -i);
		t=1.0;
		for (j=1;j<=N;j++) {
			y[j]=ysav[j];
			dydx[j]=dysav[j];
		}
		rkqs(y,dydx,N,&t,htry,eps,yscal,&hdid,&hnext,derivs);
		//printf("%13f %8.2f %14.6f %12.6f \n",eps,htry,hdid,hnext);
		//printf("%f\n", y);
	}

	


/******Graficación de trayectorias y distribuciones******/



return 0;
}

/******Cash Sharp Runge-Kutta orden 5******/


