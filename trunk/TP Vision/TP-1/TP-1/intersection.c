////////////////////////////////////////////////////////////////////////
//
// hello-world.cpp
//
// This is a simple, introductory OpenCV program. The program reads an
// image from a file and displays the result.
//
////////////////////////////////////////////////////////////////////////
#include "histogramme.h"

// D:\TRAVAIL\M2\Robotique Avancée\Vision\TPs\TP_Vision_1\bin\Debug\TP_Vision_1.exe test.jpg lena.jpg

int main(int argc, char *argv[])
{

  int histo_RGB16_1[48];
  int histo_RGB16_2[48];

  // verification des arguments en ligne de commande
  if(argc<3)
  {
    printf("2 images a mettre en argument");
    exit(0);
  }

  compute_histo_16(argv[1],histo_RGB16_1);
  compute_histo_16(argv[2],histo_RGB16_2);

  intersection_histo(histo_RGB16_1, histo_RGB16_2, 48);

  system("pause");

  return 0;
}
