/* Oscilador armónico bajo la radiación de Punto Cero con pulso excitatorio*/
/* 30 Abril 2015 */
/* Versión con parámetros correctos */
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
double e1sav[200000],e2sav[200000];
double a2, a3, a4, a5, a6, b21, b31, b32, b41, b42, b43, b51, b52, b53, b54, b61, b62, b63, b64, b65, c1s, c2s, c3s, c4s, c5s, c6s;
double c1,c2,c3,c4,c5,c6;
double D0;
double D1[2],x,v,t,xn_1,vn_1,xs_1,vs_1,h,z;
int n,i,N_w;
double dxdt, dvdt, dydx[2];
//Parámetros
double E_const, omega_0,c,e,m,omega_p,A_0,delta_t,ks,e_0,h_bar,V,V_k,kappa_0,d_omega,tint,hinit;
double E_vaco, E_temp, E_puls;
// Espectro
double energia;
double energia_promedio;
//Variables de graficación, manejo y procesamiento de datos
int histograma[200000];
//Funciones
double f(double ti, double xi, double vi, int n);
double modos();
double E_vac(double tp);
void derivs(float t,float y[],float dydx[]);
double A_pulso(double tp, double angulo, double xp, double zp);
double E_pulso(double tp, double angulo, double xp, double zp);

/****************** Ecuación diferencial a resolver *****************/
double f(double ti, double xi, double vi, int n) 
{
	dxdt = vi;    		    // dx/dt = v
	dvdt = -omega_0*omega_0*xi-bgamma*omega_0*omega_0*vi+(e/m)*E_temp; // dv/dt = -kx-bv+E
	
	dydx[0] = dxdt;
	dydx[1] = dvdt;
		
	return dydx[n];
}

// Campo de vacío en la componente x
double E_vac(double tp)
{

    E_vaco=0;
    for(i=0; i<N_w;i++)
    {
	//printf("%d",i);
	E_vaco = E_vaco+E_const*(e1sav[i]*cos(omega_0*tp-phase1)+e2sav[i]*cos(omega_0*tp-phase2));
	//printf("%f\n",E_vaco);
    }
	return E_vaco;
}

//Campo del pulso en la componente x

double A_pulso(double tp, double angulo, double xp, double zp)
{
	//E_puls = A_0*sin(omega_p/c*(sin(angulo)*xp+cos(angulo)*zp)-omega_p*tp)*exp(-pow((sin(angulo)*xp+cos(angulo)*zp-t)/delta_t,2))*omega_p*cos(angulo);//+A_0*cos(sin(angulo)*xp+cos(angulo)*zp-omega_p*tp)*exp(-pow((sin(angulo)*xp+cos(angulo)*zp-t)/delta_t,2))*((sin(angulo)*xp+cos(angulo)*zp-t)/delta_t)*(1/delta_t)*cos(angulo);
	double A_puls;
	A_puls = A_0*cos(omega_p/c*(sin(angulo)*xp+cos(angulo)*zp)-omega_p*tp)*exp(-pow((sin(angulo)*xp+cos(angulo)*zp-tp)/delta_t,2));
	return A_puls;
}

double E_pulso(double tp, double angulo, double xp, double zp)
{
	double dt=0.00000000001;
	double E_puls;
	E_puls=(A_pulso(tp+dt,angulo,xp,zp)-A_pulso(tp,angulo,xp,zp))/dt;
	return -E_puls;
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

	  phase1 = 2*(M_PI)*(rand()/((double)RAND_MAX+1));
	  phase2 = 2*(M_PI)*(rand()/((double)RAND_MAX+1)); 
}

int main(int argc, char *argv[]) 
{
  
  if(argc < 2)
	{
		printf("No hay suficientes argumentos\n");
		printf("Sintaxis:\noscilador [frecuencia del pulso] ");	
                // Si se desea generalizar a rectangular agregue [pared derecha] [pared  iquierda] [pared arriba] [pared abajo]
		exit(0);
	}
// Parámetros
	delta = 220*bgamma*omega_0*omega_0;
	
	
	A_0=1.5e-9;
	e_0=8.854e-12;
	c=3e8;
	m=9.11e-35;
	e=1.60e-19;		
	omega_0 = 10e16;
	bgamma = (2.0*e*e/(3.0*m*c*c*c))*(1.0/(4.0*M_PI*e_0));
	//printf("%.50f ", bgamma);
	h_bar=6.626e-34;
	ks=omega_0*omega_0*m;
	delta_t=10e-14;
	N_w=500;
	delta = 220.0*bgamma*omega_0*omega_0;
	//printf("%f ", delta);
	V_k = (4.0*M_PI/3.0)*powf((omega_0+delta/2.0)/c,3.0)-(4.0*M_PI/3.0)*powf((omega_0-delta/2.0)/c,3.0);
	//printf("v_k:%f ", V_k);
	V=powf(2.0*M_PI,3.0)*(N_w/V_k);
	//printf("v:%1.50f ", V);
	deltakappa = (pow(omega_0+delta/2.0,3.0)/(3.0*pow(c,3))-pow(omega_0-delta/2.0,3.0)/(3.0*pow(c,3)) )/(N_w-1);
	//printf("%f\n",deltakappa);
	E_const = sqrt(h_bar/(e_0*V));
	omega_p=atof(argv[1])*omega_0;
	kappa_0=(1/3.0)*powf(omega_0/c,3.0);
	d_omega=c*pow(3.0*kappa_0,1/3)/(3.0*(deltakappa/kappa_0));
	
	//printf("%f %f %f\n",deltakappa,omega_p,kappa_0,d_omega); 
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
//Condiciones iniciales
	hinit=(1.0/20.0)*(2.0*M_PI/omega_0);
	h=hinit;
	tint=2.0*M_PI/d_omega;
	//printf("%1.50f %1.50f %f1.50\n", hinit,tint,tint/hinit);
	t=0;
	x=0;
	v=1;
	E_temp=0;
	//printf("%f\n",x);

	D0=1.0/10e10; //Precisión asegurada hasta 10 dígitos
	int steps;	
	steps=1000000000;
	int l;
	
	//Calcular los modos del campo
	
	
	for(i=0;i<N_w;i++)
	{
	  modos(i);
	  e1sav[i]=e1[0];
	  e2sav[i]=e2[0];
	}
	
	
	while(t<tint) //Número de puntos por correr 
	{
		//Correr pasos e ir adaptando hasta la precisión deseada
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
		z=0;
		t=t+h;
		h=hinit;
		double polarizacion;
		polarizacion=M_PI/4.;
		//printf("%f\n", E_temp);
		E_temp=E_vac(t);//+E_pulso(t,polarizacion,x,z);
		/******Graficación de trayectorias y distribuciones******/
		//printf("%.15f\n",E_pulso(t,polarizacion,x,z));
		double sc;
		sc=x*100000000000000000;
		double index = (sc+1)*100000.;
		//printf("%d\n",(int)floor(index));
		if ((int)floor(index)>0 && (int)floor(index)<200000)
		  histograma[(int)floor(index)]=histograma[(int)floor(index)]+1;
		//printf("tiempo:%1.50f pos:%1.50f vel:%1.50f\n",t,x,v);
		energia=m*v*v/2 + ks*x*x/2+energia;
		count++;
	}
	//printf("Pasos:%f\n",count);	
	energia_promedio=energia/count;
	for(i=0;i<200000;i++)
	 printf("%1.20f, %d\n",i/100000.-1,histograma[i]);
	//printf("%1.50f\n", energia_promedio);
	
	return 0;
}
