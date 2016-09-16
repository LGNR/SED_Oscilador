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
double thetai,ki,phii,xi;
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
//Constantes físicas
double c,e,m,epsilon_0;
//Parámetros
double E_const, omega_0;
double E_vaco, E_temp;
//Variables de graficación, manejo y procesamiento de datos
int histograma[200000];
int debug;
//Funciones
double modos(int i);
double E_vac(double tp);

// Campo en la componente x
double E_vac(double tp)
{

	modos(1);
	E_vaco = E_const*(e1[0]*cos(omega_0*tp-phase1)+e2[0]*cos(omega_0*tp-phase2));
	//printf("%12e\n",E_vaco);
	return E_vaco;
}

double modos(int i)
{
	random1 = 2.0*(1.0)*(rand()/((double)RAND_MAX+1))-1;      // Entre -1 y 1
	random2 = 2.0*(M_PI)*(rand()/((double)RAND_MAX+1));     // Entre 0 y 2Pi
	random3 = 2.0*(M_PI)*(rand()/((double)RAND_MAX+1));     // Entre 0 y 2Pi

// Debug
/*
	printf("%12e\n",random1);
	printf("%12e\n",random2);
	printf("%12e\n",random3);
*/

/******Generación de los modos del campo******/

// Números para muestrear los modos k
	deltakappa=(pow((omega_0+delta/2.0),3.0)/(3.0*pow(c,3.0))-pow((omega_0-delta/2.0),3.0)/(3.0*pow(c,3.0)))/(N_k-1);
	
	//Debug	
	if (debug==1)
	printf("deltakappa:%12e\n",deltakappa);

	kappa = (pow((omega_0-delta/2.0),3.0))/(3.0*pow(c,3.0)) + (i-1)*deltakappa;
	nu = random1;
	phi = random2;



	//Debug
	if (debug==1)
	{
	printf("kappa:%12e\n",kappa);
	printf("nu:%12e\n",nu);
	printf("phi:%12e\n",phi);	
	}

	ki = pow(3.0*kappa,1.0/3.0);// pow((3.0*kappa),(1.0/3.0));
	thetai = acos(nu);
	phii = phi;

	//Debug
	if (debug==1)
	{
	printf("ki:%12e\n",ki);
	printf("thetai:%12e\n",thetai);
	printf("phii:%12e\n",phii);
	}

// Vector k
	k[0] = ki*sin(thetai)*cos(phii);
	k[1] = ki*sin(thetai)*sin(phii);
	k[2] = ki*cos(thetai);

	//printf("%12e, %12e, %12e\n",k[0],k[1],k[2]);


// Polarizaciones
	xi = random3;

	e1[0] = cos(thetai)*cos(phii)*cos(xi) - sin(phii)*sin(xi);
	e1[1] = cos(thetai)*sin(phii)*cos(xi) + cos(phii)*sin(xi);
	e1[2] = -sin(thetai)*cos(xi);

	e2[0] = -cos(thetai)*cos(phii)*cos(xi) - sin(phii)*sin(xi);
	e2[1] = -cos(thetai)*sin(phii)*cos(xi) + cos(phii)*sin(xi);
	e2[2] = sin(thetai)*cos(xi);

	printf("%12e, %12e, %12e\n",e1[0],e1[1],e1[2]);	

	phase1 = 2*(M_PI)*(rand()/((double)RAND_MAX+1));
	phase2 = 2*(M_PI)*(rand()/((double)RAND_MAX+1)); 
}

int main()
{
	e=1.6021E-19;
	m=9.1093E-31;
	c=2.9979E8;
	epsilon_0=8.8541E-12;
	bgamma=(2.0*e*e/3.0*m*pow(c,3.0))*(1/4.0*M_PI*epsilon_0);
	// Debe cubrirse la resonancia característica completa: bgamma*omega_0^2 << delta 
	omega_0=10E16;
	delta=220*bgamma*omega_0*omega_0;
	N_k=3000;
	if (debug==1)
	{
	printf("bgamma:%12e\n",bgamma);
	printf("bgamma*omega_0^2:%12e\n",bgamma*omega_0*omega_0);
	}
	
	debug=0;
	
	for(j=1;j<=N_k;j++)
	{		
		modos(j);
		//printf("ki:%12e\n",ki);
	}
	
	return 0;	

}
