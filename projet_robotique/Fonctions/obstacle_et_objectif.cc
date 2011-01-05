#define USAGE \

#include <libplayerc++/playerc++.h>
#include <iostream>
#include <math.h>
#include <sys/time.h>

#include "args.h"



// Calcul de la valeur flous de la distance
/*
float petit(float distance)
{
	float valeur_petit;
	
	if (distance <= 1) valeur_petit = 1;
	if ((distance > 1) && (distance < 3)) valeur_petit = 1.5-(distance/2);
	if (distance >= 3) valeur_petit = 0;

	return valeur_petit;	
}

float grand(float distance)
{
	float valeur_grand;
	
	if (distance <= 1) valeur_grand = 0;
	if ((distance > 1) && (distance < 3)) valeur_grand = (distance/2)-0.5;
	if (distance >= 3) valeur_grand = 1;

	return valeur_grand;
}
*/
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

float vitesse_gauche (float distance_ga, float distance_dr , float distance_de , float distance_ar)
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

	float num, den;

	num = regle1 * grand_positif +
	regle2 * grand_positif + 
	regle3 * moyen_negatif + 
	regle4 * moyen_negatif + 
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

float vitesse_droite (float distance_ga, float distance_dr , float distance_de , float distance_ar)
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

	float num, den;

	num = regle1 * grand_positif +
	regle2 * grand_positif +
	regle3 * moyen_positif + 
	regle4 * moyen_positif + 
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
	if ((vecteur_r > 0.1) && (vecteur_r < 0.4)) val_attraction = (4/3)-(vecteur_r/0.3);
	if (vecteur_r >= 0.4) val_attraction = 0;

	return val_attraction;	
}

float repulsion(float vecteur_r)
{
	float val_repulsion;
	
	if (vecteur_r <= 0.1) val_repulsion = 0;
	if ((vecteur_r > 0.1) && (vecteur_r < 0.4)) val_repulsion = -(1/3)+(vecteur_r/0.3);
	if (vecteur_r >= 0.4) val_repulsion = 1;

	return val_repulsion;	
}



// MAIN

int main(int argc, char **argv)
{

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

    // Definition de l'objectif

    float objectif_x = 20.0;
    float objectif_y = 20.0;

    float newspeed=0.0;
    float newturnrate=0.0;
    float pi = 3.14159265;
    float position_x = 0.0;
    float position_y = 0.0;
    float position_angle = 0.0;

    float distance = sqrt(objectif_x*objectif_x + objectif_y*objectif_y);
   // float angle = atan(objectif_y / objectif_x);
    
    timeval tim;
    double t1,t2;
    gettimeofday(&tim, NULL);
    t1=tim.tv_sec+(tim.tv_usec/1000000.0);

    
    for(;;)				//Boucle infinie
    {

	//Lecture des informations en provenance du robot

      robot.Read();
      
      float distance_ga;
      float distance_dr;
      float distance_de;
      float distance_ar;
      
      // Récupération des données capteurs
      
      distance_de= (sp[0]+sp[1]+sp[3]+spi[0]+spi[1])/5;
      distance_ga= (sp[2]+sp[5])/2;
      distance_dr= (sp[4]+sp[6])/2;
      distance_ar= spa[0];

      float vitesse_roue_gauche;
      float vitesse_roue_droite;
      float d = 0.20;			// distance entre les roues
      float distance,distance_parcouru,angle_parcouru,deplacement_x,deplacement_y,objectif_angle;
      float vecteur_a,vecteur_r,speed_r;
      double temps;

	// calcul de la vitesse des roues	

      vitesse_roue_gauche=vitesse_gauche(distance_ga, distance_dr , distance_de , distance_ar);
      vitesse_roue_droite=vitesse_droite(distance_ga, distance_dr , distance_de , distance_ar);


	// calcul de la nouvelle position

	//int i;
	//for ( long i = 0; i < 100000000; ++i ); // 100000000
	gettimeofday(&tim, NULL);
	t2=tim.tv_sec+(tim.tv_usec/(double)1000000.0);
	temps =  ((double)t2-(double)t1);
	float k,l;	// constantes relatives a la simu
	k=0.9336;
	l=0.9355;
	distance_parcouru = temps *newspeed*k;
	angle_parcouru = temps *newturnrate*l;
	gettimeofday(&tim, NULL);
	t1=tim.tv_sec+(tim.tv_usec/1000000.0);

	position_angle = (position_angle + angle_parcouru);
	if(position_angle > (2*pi)) position_angle = position_angle - (2*pi);
	if(position_angle < -(2*pi)) position_angle = position_angle + (2*pi);
	deplacement_x = distance_parcouru * cos(position_angle);
	deplacement_y = distance_parcouru * sin(position_angle);
	position_x = position_x + deplacement_x;
	position_y = position_y + deplacement_y;
	distance = sqrt(((objectif_y - position_y)*(objectif_y - position_y))+((objectif_x - position_x)*(objectif_x - position_x)));

	// calcul du vecteur attractif (angle)
	
	objectif_angle = atan((objectif_y - position_y) / (objectif_x - position_x));
	if(objectif_x < 0) objectif_angle = objectif_angle + pi;
	vecteur_a = objectif_angle - position_angle;
	if(vecteur_a < -pi) vecteur_a = pi-(abs(vecteur_a)-pi);
	if(vecteur_a > pi) vecteur_a = -(pi-(abs(vecteur_a)-pi));

	// calcul du vecteur repulsif (angle)
	
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

	newspeed=speed*2;
	newturnrate=turnrate;

	// condition d'arret

	if (distance <= 0.1)
	{
		newspeed = 0;
		newturnrate = 0;
	}
//std::cout << "distance" << "   " << distance << std::endl;

	// commander le robot 

	pp.SetSpeed(newspeed, newturnrate);

    }
  }
  catch (PlayerCc::PlayerError e)
  {
    std::cerr << e << std::endl;
    return -1;
  }
}

