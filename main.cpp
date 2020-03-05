#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <FreeImage.h>
#include <omp.h>

using namespace std; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "basic.h"
#include "variables.h" 
#include "Transform.h"
#include "readfile.h"
#include "gridcube.h"
#include "raycast.h"


void cleanup(void){
  delete[] vertices;
  vertices = NULL;
  delete[] objects;
  objects = NULL;
  delete[] lights;
  lights = NULL;
}

int main(int argc, char* argv[]) 
{

  int i, j;
  unsigned long long tempindex;
//   BYTE tempcolor;
  int threads, newperc, perc = 0;
  double rayvec[3], raycolor[3], eyeA[3], sqvec, vecnorm, wA[3], uA[3], vA[3], ucomp, vcomp, wcomp;
  
  if (argc < 2) {
    cerr << "Usage: raytrace scenefile\n"; 
    exit(-1); 
  }

  FreeImage_Initialise();
  
  cout << "Reading input file...\n";
  readfile(argv[1]);
  cout << "Done.\n";
  
  vec3 w = glm::normalize(eye - center);
  vec3 u = glm::normalize(glm::cross(up, w));
  vec3 v = glm::cross(w,u);
  
  uA[0] = u[0]; uA[1] = u[1]; uA[2] = u[2];
  vA[0] = v[0]; vA[1] = v[1]; vA[2] = v[2];
  wA[0] = w[0]; wA[1] = w[1]; wA[2] = w[2];
  eyeA[0] = eye[0]; eyeA[1] = eye[1]; eyeA[2] = eye[2]; 
  
  double focald = 1.0/tan(fovy/2.0 * PI/180.0);
  
  tempindex = 3 * width * heigth;
  BYTE* pixels = new BYTE[tempindex];
  tempindex = 0;
  
  createGrid();
  
  threads = omp_get_num_procs();
  cout << "Using " << threads << " threads\n";
  omp_set_num_threads(threads);
  
  
  wcomp = - focald;
  
  
#pragma omp parallel for private(ucomp, vcomp, j, rayvec, sqvec, vecnorm, tempindex, raycolor, newperc) schedule(dynamic,1)
  for (i = 0; i < heigth; i++) {
      
    vcomp = (2.0 * i + 1.0) / heigth - 1.0;
    

    for (j = 0; j < width; j++) {
      
      ucomp = (2.0 * j + 1.0 - width) / heigth;      
      
      rayvec[0] = ucomp * uA[0] + vcomp * vA[0] + wcomp * wA[0];
      rayvec[1] = ucomp * uA[1] + vcomp * vA[1] + wcomp * wA[1];
      rayvec[2] = ucomp * uA[2] + vcomp * vA[2] + wcomp * wA[2];
      
      sqvec = rayvec[0] * rayvec[0] + rayvec[1] * rayvec[1] + rayvec[2] * rayvec[2];
      vecnorm = sqrt(sqvec);
      
      rayvec[0] /= vecnorm;
      rayvec[1] /= vecnorm;
      rayvec[2] /= vecnorm;      
      
      shootRay(eyeA, rayvec, raydepth, raycolor);
      
// //       tempindex = 3 * width * i + 3 * j;
//       pixels[tempindex++] = min(raycolor[2] * 255.0, 255.0);//BGR format, not RGB
//       
// //       tempindex = 3 * width * i + 3 * j + 1;
//       pixels[tempindex++] = min(raycolor[1] * 255.0, 255.0);
//       
// //       tempindex = 3 * width * i + 3 * j + 2;
//       pixels[tempindex++] = min(raycolor[0] * 255.0, 255.0);
      
      
      tempindex = 3 * width * i + 3 * j;
      pixels[tempindex++] = min(raycolor[2] * 255.0, 255.0);//BGR format, not RGB
      
      pixels[tempindex++] = min(raycolor[1] * 255.0, 255.0);
      
      pixels[tempindex] = min(raycolor[0] * 255.0, 255.0);
      
    }
    
    newperc = (i * 10) / heigth;
    if (newperc > perc) {
      perc = newperc;
      cout << "Image complete at " << perc * 10 << "%\n";      
    }

 }
 
    
    
  cout << "Image complete at 100%\n";
  
  FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, width, heigth, width * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

  cout << "Saving image...\n";

  FreeImage_Save(FIF_PNG, img, output.c_str(), 0);
  
  cout << "Image saved as "<< output.c_str() <<".\n";
  
  delete[] pixels;
  
  deleteGrid(grid);

  FreeImage_DeInitialise();
  cleanup();
  return 0;
}
