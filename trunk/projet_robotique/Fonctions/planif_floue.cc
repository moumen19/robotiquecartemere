#define USAGE \

#include <libplayerc++/playerc++.h>
#include <iostream>
#include <math.h>
#include <sys/time.h>

#include "args.h"



// Calcul de la valeur flous de la distance

float petit(float distance)
{
	float valeur_petit;
	
	if (distance <= 1) valeur_petit = 1;
	if ((distance > 1) && (distance < 2)) valeur_petit = 2-(distance);
	if (distance >= 2) valeur_petit = 0;

	return valeur_petit;	
}

float grand(float distance)
{
	float valeur_grand;
	
	if (distance <= 1) valeur_grand = 0;
	if ((distance > 1) && (distance < 2)) valeur_grand = (distance)-1;
	if (distance >= 2) valeur_grand = 1;

	return valeur_grand;
}


// calcul de la valeur minimale

float minf (float val1, float val2, float val3,float val4 )
{
	if((val1 <= val2) && (val1 <= val3) && (val1 <=val4)) return val1;
	if((val2 <= val1) && (val2 <= val3) && (val2 <=val4)) return val2;
	if((val3 <= val1) && (val3 <= val2) && (val3 <=val4)) return val3;
	if((val4 <= val1) && (val4 <= val2) && (val4 <=val3)) return val4;
}


// établissement des règles

// règle 1
float valeur_regle1(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{	
	float regle1;
	regle1 = minf( grand (distance_ga),grand (distance_dr), grand (distance_de), grand(distance_ar));
	return regle1;
	
}

// règle 2
float valeur_regle2(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{	
	float regle2;
	regle2 = minf( grand (distance_ga),grand (distance_dr), grand (distance_de), petit (distance_ar));
	return regle2;	
}

// règle 3
float valeur_regle3(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle3;
	regle3 = minf( grand (distance_ga),grand (distance_dr), petit (distance_de), grand(distance_ar));
	return regle3;
}

// règle 4
float valeur_regle4(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle4;
	regle4 = minf( grand (distance_ga),grand (distance_dr), petit (distance_de), petit(distance_ar));
	return regle4;
}

// règle 5
float valeur_regle5(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle5;
	regle5 = minf( grand (distance_ga),petit (distance_dr), grand (distance_de), grand(distance_ar));
	return regle5;
}

// règle 6
float valeur_regle6(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle6;
	regle6 = minf( grand (distance_ga),petit (distance_dr), grand (distance_de), petit(distance_ar));
	return regle6;
}

// règle 7
float valeur_regle7(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle7;
	regle7 = minf( grand (distance_ga),petit (distance_dr), petit (distance_de), grand(distance_ar));
	return regle7;
}

// règle 8
float valeur_regle8(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle8;
	regle8 = minf( grand (distance_ga),petit (distance_dr), petit (distance_de), petit (distance_ar));
	return regle8;
}

// règle 9
float valeur_regle9(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle9;
	regle9 = minf( petit (distance_ga),grand (distance_dr), grand (distance_de), grand(distance_ar));
	return regle9;
}

// règle 10
float valeur_regle10(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle10;
	regle10 = minf( petit (distance_ga),grand (distance_dr), grand (distance_de), petit(distance_ar));
	return regle10;
}

// règle 11
float valeur_regle11(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle11;
	regle11 = minf( petit (distance_ga),grand (distance_dr), petit (distance_de), grand(distance_ar));
	return regle11;
}

// règle 12
float valeur_regle12(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle12;
	regle12 = minf( petit (distance_ga),grand (distance_dr), petit (distance_de), petit(distance_ar));
	return regle12;
}

// règle 13
float valeur_regle13(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle13;
	regle13 = minf( petit (distance_ga),petit (distance_dr), grand (distance_de), grand(distance_ar));
	return regle13;
}

// règle 14
float valeur_regle14(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle14;
	regle14 = minf( petit (distance_ga),petit (distance_dr), grand (distance_de), petit(distance_ar));
	return regle14;
}

// règle 15
float valeur_regle15(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle15;
	regle15 = minf( petit (distance_ga),petit (distance_dr), petit (distance_de), grand(distance_ar));
	return regle15;
}

// règle 16
float valeur_regle16(float distance_ga, float distance_dr , float distance_de , float distance_ar)
{
	float regle16;
	regle16 = minf( petit (distance_ga),petit (distance_dr), petit (distance_de), petit(distance_ar));
	return regle16;
}

// Vitesse gauche

float vitesse_gauche (float distance_ga, float distance_dr , float distance_de , float distance_ar, float dir)
{
	float vitesse_roue_gauche;
	float grand_negatif=-0.5;
	float grand_positif=0.5;
	float moyen_negatif = -0.3;
	float moyen_positif = 0.3;
	float petit_negatif = -0.1;
	float petit_positif = 0.1;
	
    	float	regle1;
	float   regle2;
	float   regle3;
	float   regle4;
	float   regle5;
	float   regle6;
	float   regle7;
	float   regle8;
	float   regle9;
	float   regle10;
	float   regle11;
	float   regle12;
	float   regle13;
	float   regle14;
	float   regle15;
	float   regle16;
	
	regle1 =valeur_regle1(distance_ga, distance_dr , distance_de , distance_ar);
	regle2 =valeur_regle2(distance_ga, distance_dr , distance_de , distance_ar);
	regle3 =valeur_regle3(distance_ga, distance_dr , distance_de , distance_ar);
	regle4 =valeur_regle4(distance_ga, distance_dr , distance_de , distance_ar);
	regle5 =valeur_regle5(distance_ga, distance_dr , distance_de , distance_ar);
	regle6 =valeur_regle6(distance_ga, distance_dr , distance_de , distance_ar);
	regle7 =valeur_regle7(distance_ga, distance_dr , distance_de , distance_ar);
	regle8 =valeur_regle8(distance_ga, distance_dr , distance_de , distance_ar);
	regle9 =valeur_regle9(distance_ga, distance_dr , distance_de , distance_ar);
	regle10 =valeur_regle10(distance_ga, distance_dr , distance_de , distance_ar);
	regle11 =valeur_regle11(distance_ga, distance_dr , distance_de , distance_ar);
	regle12 =valeur_regle12(distance_ga, distance_dr , distance_de , distance_ar);
	regle13 =valeur_regle13(distance_ga, distance_dr , distance_de , distance_ar);
	regle14 =valeur_regle14(distance_ga, distance_dr , distance_de , distance_ar);
	regle15 =valeur_regle15(distance_ga, distance_dr , distance_de , distance_ar);
	regle16 =valeur_regle16(distance_ga, distance_dr , distance_de , distance_ar);


	//calcul de la règle

	float num,den,vitesse;

	if(dir == 1) vitesse = -0.4;
	else vitesse = 0.4;

	num = regle1 * grand_positif +
	regle2 * grand_positif + 
	regle3 * vitesse + 
	regle4 * vitesse + 
	regle5 * petit_positif + 
	regle6 * petit_positif + 
	regle7 * moyen_negatif + 
	regle8 * moyen_negatif + 
	regle9 * moyen_positif + 
	regle10 * moyen_positif + 
	regle11 * moyen_positif + 
	regle12 * moyen_positif + 
	regle13 * moyen_positif + 
	regle14 * moyen_positif + 
	regle15 * moyen_negatif + 
	regle16 * moyen_negatif;

	den = regle1 + regle2 + regle3 + regle4 + regle5 + regle6 + regle7 + regle8 + regle9 + regle10 + regle11 + regle12 + regle13 + regle14 + regle15 + regle16;
	
	
	vitesse_roue_gauche = num/den;   // A COMPLETER

	return vitesse_roue_gauche;
}

// Vitesse droite

float vitesse_droite (float distance_ga, float distance_dr , float distance_de , float distance_ar,float dir)
{
	float vitesse_roue_droite;
	float grand_negatif=-0.5;
	float grand_positif=0.5;
	float moyen_negatif = -0.3;
	float moyen_positif = 0.3;
	float petit_negatif = -0.1;
	float petit_positif = 0.1;
	
    	float	regle1;
	float   regle2;
	float   regle3;
	float   regle4;
	float   regle5;
	float   regle6;
	float   regle7;
	float   regle8;
	float   regle9;
	float   regle10;
	float   regle11;
	float   regle12;
	float   regle13;
	float   regle14;
	float   regle15;
	float   regle16;
	
	regle1 =valeur_regle1(distance_ga, distance_dr , distance_de , distance_ar);
	regle2 =valeur_regle2(distance_ga, distance_dr , distance_de , distance_ar);
	regle3 =valeur_regle3(distance_ga, distance_dr , distance_de , distance_ar);
	regle4 =valeur_regle4(distance_ga, distance_dr , distance_de , distance_ar);
	regle5 =valeur_regle5(distance_ga, distance_dr , distance_de , distance_ar);
	regle6 =valeur_regle6(distance_ga, distance_dr , distance_de , distance_ar);
	regle7 =valeur_regle7(distance_ga, distance_dr , distance_de , distance_ar);
	regle8 =valeur_regle8(distance_ga, distance_dr , distance_de , distance_ar);
	regle9 =valeur_regle9(distance_ga, distance_dr , distance_de , distance_ar);
	regle10 =valeur_regle10(distance_ga, distance_dr , distance_de , distance_ar);
	regle11 =valeur_regle11(distance_ga, distance_dr , distance_de , distance_ar);
	regle12 =valeur_regle12(distance_ga, distance_dr , distance_de , distance_ar);
	regle13 =valeur_regle13(distance_ga, distance_dr , distance_de , distance_ar);
	regle14 =valeur_regle14(distance_ga, distance_dr , distance_de , distance_ar);
	regle15 =valeur_regle15(distance_ga, distance_dr , distance_de , distance_ar);
	regle16 =valeur_regle16(distance_ga, distance_dr , distance_de , distance_ar);
	
	//calcul de la règle

	float num,den,vitesse;

	if(dir == 1) vitesse = 0.4;
	else vitesse = -0.4;

	num = regle1 * grand_positif +
	regle2 * grand_positif +
	regle3 * vitesse + 
	regle4 * vitesse + 
	regle5 * moyen_positif + 
	regle6 * moyen_positif + 
	regle7 * moyen_positif + 
	regle8 * moyen_positif + 
	regle9 * petit_positif + 
	regle10 * petit_positif + 
	regle11 * moyen_negatif + 
	regle12 * moyen_negatif + 
	regle13 * moyen_positif + 
	regle14 * moyen_positif + 
	regle15 * moyen_positif + 
	regle16 * moyen_positif;

	den = regle1 + regle2 + regle3 + regle4 + regle5 + regle6 + regle7 + regle8 + regle9 + regle10 + regle11 + regle12 + regle13 + regle14 + regle15 + regle16;
		
	vitesse_roue_droite = num/den;
	return vitesse_roue_droite;
}

// Calcul du choix de la commande

float attraction(float vecteur_r)
{
	float val_attraction;
	
	if (vecteur_r <= 0.1) val_attraction = 1;
	if ((vecteur_r > 0.1) && (vecteur_r < 0.4)) val_attraction = (5/4)-(vecteur_r/0.4);
	if (vecteur_r >= 0.4) val_attraction = 0;

	return val_attraction;	
}

float repulsion(float vecteur_r)
{
	float val_repulsion;
	
	if (vecteur_r <= 0.1) val_repulsion = 0;
	if ((vecteur_r > 0.1) && (vecteur_r < 0.4)) val_repulsion = -(1/4)+(vecteur_r/0.4);
	if (vecteur_r >= 0.4) val_repulsion = 1;

	return val_repulsion;	
}

// Planification floue

void floue(float capteurs[10],float *x,float *y,float *position_angle,float objectif_x,float objectif_y,float *vg,float *vd)
{

      float distance_ga;
      float distance_dr;
      float distance_de;
      float distance_ar;
      float vitesse_roue_gauche,vitesse_roue_droite;
      float d = 0.20;			// distance entre les roues
      float distance,distance_parcouru,angle_parcouru,deplacement_x,deplacement_y,objectif_angle;
      float vecteur_a,vecteur_r,speed_r,newspeed,newturnrate,dir;
      float pi = 3.14159265;
      
      // Test des capteurs

	int n=0,m=0,l=0;
      	if (capteurs[0])n++;
        if (capteurs[1])n++;
	if (capteurs[2])m++;
        if (capteurs[3])n++;
	if (capteurs[4])l++;
        if (capteurs[5])m++;
	if (capteurs[6])l++;
	
      // Récupération des données capteurs
      
	if(m && n && l)
	{
	      distance_de= (capteurs[0]+capteurs[1]+capteurs[3])/n;
	      distance_ga= (capteurs[2]+capteurs[5])/m;
	      distance_dr= (capteurs[4]+capteurs[6])/l;
	      distance_ar= capteurs[9];
	}
	else
	{
		*vg=0;
		*vd=0;
		return;
	}

        // distance de l'objectif

      distance = sqrt(((objectif_y - (*y))*(objectif_y - (*y)))+((objectif_x - (*x))*(objectif_x - (*x))));
//std::cout << "distance" << "   " << distance << std::endl;

	// calcul du vecteur attractif (angle)
	
	objectif_angle = atan((objectif_y - *y) / (objectif_x - *x));
	if(objectif_x < *x) objectif_angle = objectif_angle + pi;
	vecteur_a = objectif_angle - *position_angle;
	if(vecteur_a < -pi) vecteur_a = vecteur_a + (2*pi);
	if(vecteur_a > pi) vecteur_a = vecteur_a - (2*pi);
	if(vecteur_a > 0) dir = 1;
	else dir = 0;

	// calcul du vecteur repulsif (angle)
	
	vitesse_roue_gauche=vitesse_gauche(distance_ga,distance_dr,distance_de,distance_ar,dir);
        vitesse_roue_droite=vitesse_droite(distance_ga,distance_dr,distance_de,distance_ar,dir);
	vecteur_r = ((vitesse_roue_droite - vitesse_roue_gauche)/(2*d));
	speed_r = (vitesse_roue_gauche+vitesse_roue_droite)/2;

	// decision de la commande

	float val_attraction,val_repulsion,turnrate,speed;
	val_attraction = attraction(fabs(vecteur_r));
	val_repulsion = repulsion(fabs(vecteur_r));
	if(distance > 1) speed = speed_r;
	else speed = speed_r * distance;
	turnrate = ((val_attraction * vecteur_a/2) + (val_repulsion * vecteur_r));
	

	// calcul de la vitesse et de la rotation

	newspeed=speed;
	newturnrate=turnrate;

	// condition d'arret

	if (distance <= 0.1)
	{
		newspeed = 0;
		newturnrate = 0;
	}

	*vg = 2*(newspeed - (newturnrate *d));
	*vd = 2*(newspeed + (newturnrate *d));
}


// MAIN

int main(int argc, char **argv)
{

    parse_args(argc,argv);

  try
  {
    using namespace PlayerCc;

    PlayerClient robot (gHostname, gPort);
    Position2dProxy pp (&robot, gIndex);
    SonarProxy sp (&robot, 0);
    SonarProxy spi (&robot, 1);
    SonarProxy spa (&robot, 2);

    std::cout << robot << std::endl;

    pp.SetMotorEnable (true);

    // Vitesse roue gauche et roue droite
    float *vg = (float*)malloc(sizeof(float));
    float *vd = (float*)malloc(sizeof(float));
    // Position du robot en x, en y, et l'angle
    float *x = (float*)malloc(sizeof(float));
    float *y = (float*)malloc(sizeof(float));
    float *position_angle = (float*)malloc(sizeof(float));

    float newspeed,newturnrate,d;
    d=0.20;

    *vg = 0.0;
    *vd = 0.0;
    *x=0.0;
    *y=0.0;
    *position_angle = 0.0;
    float capteurs[10];

    // Definition de l'objectif

    float objectif_x = 20.0; // distance de l'objectif devant du robot
    float objectif_y = 20.0; // distance de l'objectif à gauche du robot

std::cout << "objectif x" << std::endl;
std::cin >> objectif_x ;
std::cout << "objectif y" << std::endl;
std::cin >> objectif_y ;
    
    timeval tim;
    double t1,t2;
    gettimeofday(&tim, NULL);
    t1=tim.tv_sec+(tim.tv_usec/1000000.0);
    double temps;

    
    for(;;)				//Boucle infinie
    {

	//Lecture des informations en provenance du robot
	robot.Read();
      
	capteurs[0]=sp[0];
	capteurs[1]=sp[1];
	capteurs[2]=sp[2];
	capteurs[3]=sp[3];
	capteurs[4]=sp[4];
	capteurs[5]=sp[5];
	capteurs[6]=sp[6];
	capteurs[7]=spi[0];
	capteurs[8]=spi[1];
	capteurs[9]=spa[0];

	*x = pp.GetXPos();
	*y = pp.GetYPos();
	*position_angle = pp.GetYaw();

	floue(capteurs,x,y,position_angle,objectif_x,objectif_y,vg,vd);

	// commander le robot 
	newspeed = (*vg + *vd)/2;
	newturnrate = (*vd - *vg)/(2*d);
	pp.SetSpeed(newspeed, newturnrate);

    }
  }
  catch (PlayerCc::PlayerError e)
  {
    std::cerr << e << std::endl;
    return -1;
  }
}

