
#include "stereovision.h"


CvPoint get_one_pixel(char * imageFileName)
{
    IplImage * image;
    image = cvLoadImage(imageFileName, -1);

    cvNamedWindow("Select a Point", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("Select a Point", 100, 100);
    cvShowImage("Select a Point", image);
    cvSetMouseCallback("Select a Point", stereoMouseEventHandler, NULL);

    cvWaitKey(0);
    printf("\n x:%d y:%d",pointPixelCoordinates.x,pointPixelCoordinates.y);
    cvReleaseImage(&image);
    return pointPixelCoordinates;
}

// refresh the coordinates on each click
void stereoMouseEventHandler(int event, int x, int y, int flags, void *param)
{
    if(event == CV_EVENT_LBUTTONUP)
    {
        pointPixelCoordinates.x = x;
        pointPixelCoordinates.y = y;
    }
}

// provient de OpenCV sous Matlab
int cvStereoTriangulation(CvPoint P1, CvPoint P2, int display)
{

int i,j;
int N=1;   //Nombre de points =>taille de xL
float xl[3];

// Creation des matrices
CvMat *xL_2D=cvCreateMat(1,3,CV_32FC2);		//0=gauche   1=droite
CvMat *xR_2D=cvCreateMat(1,3,CV_32FC2);

CvMat *intrinseque_droite= (CvMat*)cvLoad("Calibration_TP2//Parameters//Intrinsic_Right_Cam.xml",0,0,0);
CvMat *distortion_droite = (CvMat*)cvLoad("Calibration_TP2//Parameters//Distortion_Right_Cam.xml",0,0,0);
CvMat *intrinseque_gauche= (CvMat*)cvLoad("Calibration_TP2//Parameters//Intrinsic_Left_Cam.xml",0,0,0);
CvMat *distortion_gauche = (CvMat*)cvLoad("Calibration_TP2//Parameters//Distortion_Left_Cam.xml",0,0,0);

CvMat *translation= (CvMat*)cvLoad("Calibration_TP2//Parameters//Translation.xml",0,0,0);
CvMat *rotation = (CvMat*)cvLoad("Calibration_TP2//Parameters//Rotation.xml",0,0,0);

// pas utile ?
// CvMat *Rot = (CvMat*)cvLoad("./calibration/Rot.xml",0,0,0);

//code de triangulation - cf code Matlab

printf("\n\n*** Triangulation ***\n\n");

if(display==1)
{
	printf("\nMatrice des paramètres intrinsèques M1:\n");
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			printf("  %g",CV_MAT_ELEM(*intrinseque_droite,float,i,j));
		}
		printf("\n");
	}
	printf("\nMatrice des paramètres intrinsèques M2:\n");
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			printf("  %g",CV_MAT_ELEM(*intrinseque_gauche,float,i,j));
		}
		printf("\n");
	}

	printf("\nCoefficients de distortion droite:\n");
		for(i=0;i<4;i++){
			printf("  %g",CV_MAT_ELEM(*distortion_droite,float,i,0)); // Attention à l'indice ! vecteur ligne ou colonne !
		}

	printf("\nCoefficients de distortion:\n");
		for(i=0;i<4;i++){
			printf("  %g",CV_MAT_ELEM(*distortion_gauche,float,i,0));   // Attention à l'indice ! vecteur ligne ou colonne !
		}

	//verification des matrices
	/*printf("\nVecteur Rotation:\n");
	printf("\t[%g  %g  %g] \n",CV_MAT_ELEM(*Rot,float,0,0),CV_MAT_ELEM(*Rot,float,0,1),CV_MAT_ELEM(*Rot,float,0,2));*/

	printf("\n R=\n");
	for(i=0;i<3;i++)
	{
		printf("\t[%g  %g  %g] \n",CV_MAT_ELEM(*rotation,float,i,0),CV_MAT_ELEM(*rotation,float,i,1),CV_MAT_ELEM(*rotation,float,i,2));
	}
}


CV_MAT_ELEM(*xL_2D,float,0,0)=(float)(P1.x);
CV_MAT_ELEM(*xL_2D,float,0,1)=(float)(P1.y);

CV_MAT_ELEM(*xR_2D,float,0,0)=(float)(P2.x);
CV_MAT_ELEM(*xR_2D,float,0,1)=(float)(P2.y);


printf("\n Coordonnees pixel ... \n");
printf("xL_2D = [%g  %g  %g] \n",CV_MAT_ELEM(*xL_2D,float,0,0),CV_MAT_ELEM(*xL_2D,float,0,1),CV_MAT_ELEM(*xL_2D,float,0,2));
printf("xR_2D = [%g  %g  %g] \n",CV_MAT_ELEM(*xR_2D,float,0,0),CV_MAT_ELEM(*xR_2D,float,0,1),CV_MAT_ELEM(*xR_2D,float,0,2));

cvUndistortPoints(xL_2D,xL_2D,intrinseque_gauche,distortion_gauche,0,0);
cvUndistortPoints(xR_2D,xR_2D,intrinseque_droite,distortion_droite,0,0);

CvMat* R = cvCreateMat(3,3,CV_32F);
CvMat* R_transpose = cvCreateMat(3,3,CV_32F);
CvMat* T_vect= cvCreateMat(3,1,CV_32F);
CvMat* xt= cvCreateMat(3,1,CV_32F);
CvMat* xtt= cvCreateMat(3,1,CV_32F);
CvMat* u = cvCreateMat(3,1,CV_32F);
CvMat* X1 = cvCreateMat(3,1,CV_32F);
CvMat* X2 = cvCreateMat(3,1,CV_32F);
CvMat* Xtps = cvCreateMat(3,1,CV_32F);
CvMat* Xl = cvCreateMat(3,1,CV_32F);
CvMat* Xr = cvCreateMat(3,1,CV_32F);
CvMat* Coord3D = cvCreateMat(3,1,CV_32F);


for(i = 0; i < 3; i++)
	for(j = 0; j < 3; j++)
		cvSetReal2D(R,i,j,CV_MAT_ELEM(*rotation,float,i,j));

for(i = 0; i < 1; i++)
	for(j = 0; j < 3; j++)
		{
			cvSetReal2D(xt,j,i,CV_MAT_ELEM(*xL_2D,float,i,j));
			cvSetReal2D(xtt,j,i,CV_MAT_ELEM(*xR_2D,float,i,j));
			cvSetReal2D(T_vect,j,i,10*CV_MAT_ELEM(*translation,float,j,i));
		}
cvSetReal2D(xt,2,0,1.);
cvSetReal2D(xtt,2,0,1.);

/*printf("\n Apres Normalisation ... \n");
printf("xL_2D = [%g  %g  %g] \n",cvGetReal2D(xt,0,0),cvGetReal2D(xt,1,0),cvGetReal2D(xt,2,0));
printf("xR_2D = [%g  %g  %g] \n",cvGetReal2D(xtt,0,0),cvGetReal2D(xtt,1,0),cvGetReal2D(xtt,2,0));
*/
cvMatMul(R,xt,u);

/*
printf("\n");
//verification
printf("u = [%g  %g  %g] \n\n",CV_MAT_ELEM(*u,float,0,0),CV_MAT_ELEM(*u,float,1,0),CV_MAT_ELEM(*u,float,2,0));
*/

double n_xt2 ,n_xtt2;
double DD;
double dot_uT,dot_xttT,dot_xttu;
double NN1,NN2;
double Zt,Ztt;


n_xt2=cvDotProduct(xt,xt);
n_xtt2=cvDotProduct(xtt,xtt);

/*printf("n_xt2 = [%g] \n",n_xt2);
printf("n_xtt2 = [%g] \n",n_xtt2);
*/
DD=n_xt2*n_xtt2-cvDotProduct(u,xtt)*cvDotProduct(u,xtt);
//printf("DD = [%g] \n",DD);

dot_uT=cvDotProduct(u,T_vect);
dot_xttT=cvDotProduct(xtt,T_vect);
dot_xttu=cvDotProduct(u,xtt);
//printf("dot_uT = [%g] , dot_xttT = [%g] , dot_uT = [%g] \n",dot_uT,dot_xttT,dot_xttu);

NN1=dot_xttu*dot_xttT-n_xtt2*dot_uT;
NN2=n_xt2*dot_xttT-dot_uT*dot_xttu;
//printf("\nNN1 = [%g] , NN2 = [%g] \n",NN1,NN2);

Zt=NN1/DD;
Ztt=NN2/DD;
//printf("\nZt = [%g] , Ztt = [%g] \n",Zt,Ztt);

cvTranspose(R,R_transpose);
/*printf("\n R transpose=\n");
for(i=0;i<3;i++)
{
	printf("\t[%g  %g  %g] \n",CV_MAT_ELEM(*R_transpose,float,i,0),CV_MAT_ELEM(*R_transpose,float,i,1),CV_MAT_ELEM(*R_transpose,float,i,2));
}*/


for(i=0;i<3;i++)
{
	cvSetReal2D(X1,i,0,cvGetReal2D(xt,i,0)*Zt);
	cvSetReal2D(Xtps,i,0,(Ztt*cvGetReal2D(xtt,i,0)-cvGetReal2D(T_vect,i,0)));
}
cvMatMul(R_transpose,Xtps,X2);

/*printf("\nX1 = [%g  %g  %g] \n",cvGetReal2D(X1,0,0),cvGetReal2D(X1,1,0),cvGetReal2D(X1,2,0));
printf("\nX2 = [%g  %g  %g] \n",cvGetReal2D(X2,0,0),cvGetReal2D(X2,1,0),cvGetReal2D(X2,2,0));
*/
for(i=0;i<3;i++)
	cvSetReal2D(Xl,i,0,(cvGetReal2D(X1,i,0)+cvGetReal2D(X2,i,0))*0.5);

cvMatMulAdd(R,Xl,T_vect,Xr);
/*printf("\nXl = [%g  %g  %g] \n",cvGetReal2D(Xl,0,0),cvGetReal2D(Xl,1,0),cvGetReal2D(Xl,2,0));
printf("\nXr = [%g  %g  %g] \n",cvGetReal2D(Xr,0,0),cvGetReal2D(Xr,1,0),cvGetReal2D(Xr,2,0));
*/
for(i=0;i<3;i++)
	cvSetReal2D(Coord3D,i,0,(cvGetReal2D(Xl,i,0)+cvGetReal2D(Xr,i,0))*0.5);
printf("\nCoord3D = [%g  %g  %g] \n",cvGetReal2D(Coord3D,0,0),cvGetReal2D(Coord3D,1,0),cvGetReal2D(Coord3D,2,0));

// Destruction des images et des fenêtres
cvReleaseMat(&xR_2D);
cvReleaseMat(&xL_2D);
printf("\n*** Fin de Triangulation ***\n\n");
return 0;
}


