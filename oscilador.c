/* Oscilador armónico bajo la radiación de Punto Cero */
/* 15 Septiembre 2016 */
/* v0.2.1.1 */
/* - Parámetros según la tesis de Wayne 
   - Campo de punto cero apagado 
   - Corregido problema con la bgamma */

/* Luis Gregorio Navarro Rodríguez */

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define N 2
#define N_k 500
// Variables de generación del campo
double kappa,nu,phi,delta,bgamma,deltakappa,deltanu,deltaphi,random1,random2,random3,phase1,phase2;
double thetai,ki,phii,xi;
float R;
int i,j,n;
double k[3],e1[3],e2[3];
double modos_guardados[7][N_k];
double omega;
//int N_k, N_n, N_p, N_o; //valores máximos de kappa,nu, phi y omega respectivamente
//Variables de Runge-Kutta
double k1[2], k2[2], k3[2], k4[2], k5[2], k6[2];
double a2, a3, a4, a5, a6, b21, b31, b32, b41, b42, b43, b51, b52, b53, b54, b61, b62, b63, b64, b65, c1s, c2s, c3s, c4s, c5s, c6s;
double c1,c2,c3,c4,c5,c6;
double D0;
double D1[2],x,v,t,xn_1,vn_1,xs_1,vs_1,h;
int n,i;
double dxdt, dvdt, dydx[2];
//Constantes físicas
double c,e,m_e,hbar,epsilon_0;
//Parámetros
double E_const, omega_0,vol,V_k,m;
double E_vaco, E_temp;
//Variables de graficación, manejo y procesamiento de datos
int histograma[200000];
//Funciones
double f(double ti, double xi, double vi, int n);
double modos(int i);
double E_vac(double tp);
double norma();

/****************** Ecuación diferencial a resolver *****************/
double f(double ti, double xi, double vi, int n) 
{
	//omega_0=1;
	//bgamma=0.1;
	dxdt = vi;    		  // dx/dt = v
	dvdt = -omega_0*omega_0*xi-bgamma*omega_0*omega_0*vi;//+(e/m)*E_temp; // dv/dt = -kx-bv+E
	
	dydx[0] = dxdt;
	dydx[1] = dvdt;
		
	return dydx[n];
}


// Campo en la componente x
double E_vac(double tp)
{

	modos(1);
	omega=c*ki;
	//printf("%12e\n",omega);
	E_const=sqrt(hbar*omega/(epsilon_0*vol));
	E_vaco = E_const*(e1[0]*cos(omega*tp-phase1)+e2[0]*cos(omega*tp-phase2));
	//printf("%12e\n",E_vaco);
	return E_vaco;
}

double modos(int i)
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

	deltakappa=(pow((omega_0+delta/2.0),3.0)/(3.0*pow(c,3.0))-pow((omega_0-delta/2.0),3.0)/(3.0*pow(c,3.0)))/(N_k-1);

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
	xi = random3;

	e1[0] = cos(thetai)*cos(phii)*cos(xi) - sin(phii)*sin(xi);
	e1[1] = cos(thetai)*sin(phii)*cos(xi) + cos(phii)*sin(xi);
	e1[2] = -sin(thetai)*cos(xi);

	e2[0] = -cos(thetai)*cos(phii)*cos(xi) - sin(phii)*sin(xi);
	e2[1] = -cos(thetai)*sin(phii)*cos(xi) + cos(phii)*sin(xi);
	e2[2] = sin(thetai)*cos(xi);

	phase1 = 2*(M_PI)*(rand()/((double)RAND_MAX+1));
	phase2 = 2*(M_PI)*(rand()/((double)RAND_MAX+1)); 

	modos_guardados[0][i]=k[0];
	modos_guardados[1][i]=k[1];
	modos_guardados[2][i]=k[2];
	modos_guardados[3][i]=e1[0];
	modos_guardados[4][i]=e2[0];
	modos_guardados[5][i]=phase1;
	modos_guardados[6][i]=phase2;
}

main()
{
// Constantes físicas
	e=1.6021E-19;
	m_e=9.1093E-31;
	c=2.9979E8;
	hbar=1.0545E-34;
	epsilon_0=8.8541E-12;
	
	
	//printf("%12e\n",bgamma);
// Parámetros
	m=(10E-4)*m_e;
	bgamma=(2.0*e*e/(3.0*m*pow(c,3.0)))*(1.0/(4.0*M_PI*epsilon_0));
	omega_0=10E16;
	delta=220*bgamma*omega_0*omega_0;
	
	V_k=4*M_PI*pow(omega_0,4.0)*bgamma/pow(c,3.0);
	vol=pow(2.0*M_PI,3.0)*(N_k/V_k);

	//printf("%12e\n",bgamma*omega_0);
	//printf("%12e\n",V_k);
	//printf("%12e\n",vol);
// Aleatorios
	srand(time(NULL));
	int contador = 1;
 			
	

/******Cash Sharp Runge-Kutta orden 5, paso variable******/

//Coeficientes de Cash-Karp
	
	a2=1.0/5.0; a3=3.0/10.0; a4=3.0/5.0; a5=1.0; a6=7.0/8.0; b21=1.0/5.0; b31=3.0/40.0; b32=9.0/40.0; b41=3.0/10.0; b42=-9.0/10.0; b43=6.0/5.0; b51=-11.0/54.0; b52=5.0/2.0; b53=-70.0/27.0; b54=35.0/27.0; b61=1631.0/55296.0; b62=175.0/512.0; b63=575.0/13824.0; b64=44275.0/110592.0; b65=253.0/4096.0; 
	c1=37.0/378.0; c2=0; c3=250.0/621.0; c4=125.0/594.0; c5=0; c6=512.0/1771.0;
	c1s=2825.0/27648.0; c2s=0; c3s=18575.0/48384.0; c4s=13525.0/55296.0; c5s=277.0/14336.0; c6s=1.0/4.0;

	//printf("c1:%f \n",c1);
	double count=0;
	t=0;
	x=0;
	v=0.01;
	E_temp=0;
	//printf("%f\n",f(x,y));
	double hinit=0.1*(1.0/20.0)*(2.0*M_PI/omega_0);
	h=hinit;
	//printf("%12e\n",h);
	D0=1.0/10e10; //Precisión asegurada hasta 10 dígitos
	while(count<10000) //Número de puntos por correr 
	{
		//Correr pasos e ir adaptando hasta la precisión deseada
		E_temp=E_vac(t);
		do
		{
		
			k1[0]=h*f(t,x,v,0);
			k1[1]=h*f(t,x,v,1);
			//printf("c1*k1:%e\n",c1*k1);
			k2[0]=h*f(t+a2*h,x+b21*k1[0],v+b21*k1[1],0);
			k2[1]=h*f(t+a2*h,x+b21*k1[0],v+b21*k1[1],1);
			k3[0]=h*f(t+a3*h,x+b31*k1[0]+b32*k2[0],v+b31*k1[1]+b32*k2[1],0);
			k3[1]=h*f(t+a3*h,x+b31*k1[0]+b32*k2[0],v+b31*k1[1]+b32*k2[1],1);
			k4[0]=h*f(t+a4*h,x+b41*k1[0]+b42*k2[0]+b43*k3[0],v+b41*k1[1]+b42*k2[1]+b43*k3[1],0);
			k4[1]=h*f(t+a4*h,x+b41*k1[0]+b42*k2[0]+b43*k3[0],v+b41*k1[1]+b42*k2[1]+b43*k3[1],1);
			k5[0]=h*f(t+a5*h,x+b51*k1[0]+b52*k2[0]+b53*k3[0]+b54*k4[0],v+b51*k1[1]+b52*k2[1]+b53*k3[1]+b54*k4[1],0);
			k5[1]=h*f(t+a5*h,x+b51*k1[0]+b52*k2[0]+b53*k3[0]+b54*k4[0],v+b51*k1[1]+b52*k2[1]+b53*k3[1]+b54*k4[1],1);
			k6[0]=h*f(t+a6*h,x+b61*k1[0]+b62*k2[0]+b63*k3[0]+b64*k4[0]+b65*k5[0],v+b61*k1[1]+b62*k2[1]+b63*k3[1]+b64*k4[1]+b65*k5[1],0);
			k6[1]=h*f(t+a6*h,x+b61*k1[0]+b62*k2[0]+b63*k3[0]+b64*k4[0]+b65*k5[0],v+b61*k1[1]+b62*k2[1]+b63*k3[1]+b64*k4[1]+b65*k5[1],1);
			xn_1=x+c1*k1[0]+c2*k2[0]+c3*k3[0]+c4*k4[0]+c5*k5[0]+c6*k6[0];
			vn_1=v+c1*k1[1]+c2*k2[1]+c3*k3[1]+c4*k4[1]+c5*k5[1]+c6*k6[1];
			xs_1=x+c1s*k1[0]+c2s*k2[0]+c3s*k3[0]+c4s*k4[0]+c5s*k5[0]+c6s*k6[0];
			vs_1=v+c1s*k1[1]+c2s*k2[1]+c3s*k3[1]+c4s*k4[1]+c5s*k5[1]+c6s*k6[1];
			D1[0]=xn_1-xs_1;
			D1[1]=vn_1-vs_1;
			//printf("Error:%e, Paso:%f\n",D1,h);
			for (i=0; i<2; i++)
			// Variación del paso 
			{	
				if(D1[i]>D0)
					if (powf(fabs(D1[i]/D0),0.2)!=0)
						h=h*pow(fabs(D1[i]/D0),0.2);
					//printf("pow:%e\n",powf(fabs(D1[i]/D0),0.2));
					//printf("D[%d]=%f, newh=%e\n",i,D1[i],h);
			}	
		}
		while((D1[0]>D0) && (D1[1]>D0) && ((D1[0]!=0) || (D1[1]!=0)) )	;
		x=xn_1; 
		v=vn_1;
		t=t+h;
		h=hinit;
		//printf("%f\n", E_temp);
		/******Graficación de trayectorias y distribuciones******/
		printf("%12e\n",x);
		//printf("x:%12e v:%12e\n",x,v);
		//printf("omega_0^2*x:%12e bgamma*omega_0^2*v:%12e (e/m)*E_temp:%12e\n",omega_0*omega_0*x, bgamma*omega_0*omega_0*v,(e/m)*E_temp);
		double index = (x+1)*100000.;
		//printf("%d\n",(int)floor(index));
		//histograma[(int)floor(index)]=histograma[(int)floor(index)]+1;
		//printf("tiempo:%f pos:%f vel:%f\n",t,x,v);
		count++;
	}
	//for(i=0;i<200000;i++)
	//	printf("%f, %d\n",i/100000.-1,histograma[i]);
	//printf("%f\n",c);
	//printf("%f\n",kappa);
	//printf("%12e\n",bgamma);
	return 0;
}
