#include <iostream>
using namespace std;
#include "basic.h"
#include "variables.h"
#include "gridcube.h"
#include "raycast.h"


double intersectTriangle(const int i, double* rayorigin, double* rayvec){//calculations simplified since there are no projections
  double rayvecobj[3]; //not normalized ray vector in object frame
  double rayoriginobj[3]; //ray origin in object frame
  double intersection[3]; //intersection point in object frame
  
  rayvecobj[2] = (objects[i].invtransform)[0][2] * rayvec[0] + 
                 (objects[i].invtransform)[1][2] * rayvec[1] + 
                 (objects[i].invtransform)[2][2] * rayvec[2];
  
  if (rayvecobj[2] == 0.0) return -1.0;
  
  rayoriginobj[2] = (objects[i].invtransform)[0][2] * rayorigin[0] + 
                    (objects[i].invtransform)[1][2] * rayorigin[1] + 
                    (objects[i].invtransform)[2][2] * rayorigin[2] +
                    (objects[i].invtransform)[3][2];
  
  double distance = - rayoriginobj[2] / rayvecobj[2]; //not true distance since rayvecobj is not normalized
  
  if (distance > 0.0){
      
    rayvecobj[0] = (objects[i].invtransform)[0][0] * rayvec[0] + 
                   (objects[i].invtransform)[1][0] * rayvec[1] + 
                   (objects[i].invtransform)[2][0] * rayvec[2];
    rayvecobj[1] = (objects[i].invtransform)[0][1] * rayvec[0] + 
                   (objects[i].invtransform)[1][1] * rayvec[1] + 
                   (objects[i].invtransform)[2][1] * rayvec[2];
                   
    rayoriginobj[0] = (objects[i].invtransform)[0][0] * rayorigin[0] + 
                      (objects[i].invtransform)[1][0] * rayorigin[1] + 
                      (objects[i].invtransform)[2][0] * rayorigin[2] +
                      (objects[i].invtransform)[3][0];
    rayoriginobj[1] = (objects[i].invtransform)[0][1] * rayorigin[0] + 
                      (objects[i].invtransform)[1][1] * rayorigin[1] + 
                      (objects[i].invtransform)[2][1] * rayorigin[2] +
                      (objects[i].invtransform)[3][1];               
    
    intersection[0] = rayoriginobj[0] + distance * rayvecobj[0];
    intersection[1] = rayoriginobj[1] + distance * rayvecobj[1];
    
    if (intersection[0] >= 0.0 && intersection[1] >= 0.0 && intersection[0] + intersection[1] <= 1.0)
      return distance; //distance IS the distance in the original frame
  }
  
  return -1.0;
}

double intersectSphere(const int i, double* rayorigin, double* rayvec){
  double dott, discr, minroot, maxroot, sqorig, sqvec;
  double rayvecobj[3];
  double rayoriginobj[3];
  
  rayvecobj[0] = (objects[i].invtransform)[0][0] * rayvec[0] + 
                 (objects[i].invtransform)[1][0] * rayvec[1] + 
                 (objects[i].invtransform)[2][0] * rayvec[2];
  rayvecobj[1] = (objects[i].invtransform)[0][1] * rayvec[0] + 
                 (objects[i].invtransform)[1][1] * rayvec[1] + 
                 (objects[i].invtransform)[2][1] * rayvec[2];
  rayvecobj[2] = (objects[i].invtransform)[0][2] * rayvec[0] + 
                 (objects[i].invtransform)[1][2] * rayvec[1] + 
                 (objects[i].invtransform)[2][2] * rayvec[2];
                 
  rayoriginobj[0] = (objects[i].invtransform)[0][0] * rayorigin[0] + 
                    (objects[i].invtransform)[1][0] * rayorigin[1] + 
                    (objects[i].invtransform)[2][0] * rayorigin[2] +
                    (objects[i].invtransform)[3][0];
  rayoriginobj[1] = (objects[i].invtransform)[0][1] * rayorigin[0] + 
                    (objects[i].invtransform)[1][1] * rayorigin[1] + 
                    (objects[i].invtransform)[2][1] * rayorigin[2] +
                    (objects[i].invtransform)[3][1]; 
  rayoriginobj[2] = (objects[i].invtransform)[0][2] * rayorigin[0] + 
                    (objects[i].invtransform)[1][2] * rayorigin[1] + 
                    (objects[i].invtransform)[2][2] * rayorigin[2] +
                    (objects[i].invtransform)[3][2];
  
  dott = - rayvecobj[0] * rayoriginobj[0] - rayvecobj[1] * rayoriginobj[1] - rayvecobj[2] * rayoriginobj[2];
  sqorig = rayoriginobj[0] * rayoriginobj[0] + rayoriginobj[1] * rayoriginobj[1] + rayoriginobj[2] * rayoriginobj[2];
  sqvec = rayvecobj[0] * rayvecobj[0] + rayvecobj[1] * rayvecobj[1] + rayvecobj[2] * rayvecobj[2];
  discr = dott * dott + (1.0 - sqorig) * sqvec;
  
  if (discr <= 0.0) return -1.0;
  
  minroot = dott - sqrt(discr);
  
  if (minroot > 0.0) return minroot / sqvec;
    
  maxroot = dott + sqrt(discr);
  
  if (maxroot > 0.0) return maxroot / sqvec;
    
  return -1.0;
  
  
}

double intersectObj(const int i, double* rayorigin, double* rayvec){
    
//     ****MAH****begin
    
//     bool hit = false;
//     double intersection1[3], intersection2[3], dist;
//     
//     if (rayvec[0] != 0.0){
//         
//         dist = ((objects[i].bborigin)[0] - rayorigin[0])/rayvec[0];
//         intersection1[1] = rayorigin[1] + dist * rayvec[1];
//         intersection1[2] = rayorigin[2] + dist * rayvec[2];
//       
//         if (intersection1[1] > (objects[i].bborigin)[1] && intersection1[1] < (objects[i].bbopposite)[1] && 
//             intersection1[2] > (objects[i].bborigin)[2] && intersection1[2] < (objects[i].bbopposite)[2]) {
//           
//           hit = true;
//        
//         } else {
//       
//           dist = ((objects[i].bbopposite)[0] - rayorigin[0])/rayvec[0];
//           intersection2[1] = rayorigin[1] + dist * rayvec[1];
//           intersection2[2] = rayorigin[2] + dist * rayvec[2];
//           
//           if ((intersection1[1] < (objects[i].bborigin)[1] && intersection2[1] < (objects[i].bborigin)[1])
//               ||
//               (intersection1[2] < (objects[i].bborigin)[2] && intersection2[2] < (objects[i].bborigin)[2])
//               ||
//               (intersection1[1] > (objects[i].bbopposite)[1] && intersection2[1] > (objects[i].bbopposite)[1])
//               ||
//               (intersection1[2] > (objects[i].bbopposite)[2] && intersection2[2] > (objects[i].bbopposite)[2])) {
//                   
//             return -1.0;    
//               
//             } else if 
//              ((intersection2[1] > (objects[i].bborigin)[1] && intersection2[1] < (objects[i].bbopposite)[1] && 
//                intersection2[2] > (objects[i].bborigin)[2] && intersection2[2] < (objects[i].bbopposite)[2])
//               ||
//               (intersection1[1] < (objects[i].bborigin)[1] && intersection1[2] > (objects[i].bborigin)[2] && intersection1[2] < (objects[i].bbopposite)[2] &&
//                intersection2[1] > (objects[i].bbopposite)[1] && intersection2[2] > (objects[i].bborigin)[2] && intersection2[2] < (objects[i].bbopposite)[2])
//               ||
//               (intersection2[1] < (objects[i].bborigin)[1] && intersection2[2] > (objects[i].bborigin)[2] && intersection2[2] < (objects[i].bbopposite)[2] &&
//                intersection1[1] > (objects[i].bbopposite)[1] && intersection1[2] > (objects[i].bborigin)[2] && intersection1[2] < (objects[i].bbopposite)[2])
//               ||
//               (intersection1[2] < (objects[i].bborigin)[2] && intersection1[1] > (objects[i].bborigin)[1] && intersection1[1] < (objects[i].bbopposite)[1] &&
//                intersection2[2] > (objects[i].bbopposite)[2] && intersection2[1] > (objects[i].bborigin)[1] && intersection2[1] < (objects[i].bbopposite)[1])
//               ||
//               (intersection2[2] < (objects[i].bborigin)[2] && intersection2[1] > (objects[i].bborigin)[1] && intersection2[1] < (objects[i].bbopposite)[1] &&
//                intersection1[2] > (objects[i].bbopposite)[2] && intersection1[1] > (objects[i].bborigin)[1] && intersection1[1] < (objects[i].bbopposite)[1])) {
//           
//             hit = true;
//       
//           }
//           
//         }
//          
//     }
//       
//     if (rayvec[1] != 0.0 && !hit){
//         
//         dist = ((objects[i].bborigin)[1] - rayorigin[1])/rayvec[1];
//         intersection1[0] = rayorigin[0] + dist * rayvec[0];
//         intersection1[2] = rayorigin[2] + dist * rayvec[2];
//       
//         if (intersection1[0] > (objects[i].bborigin)[0] && intersection1[0] < (objects[i].bbopposite)[0] && 
//             intersection1[2] > (objects[i].bborigin)[2] && intersection1[2] < (objects[i].bbopposite)[2]) {
//           
//           hit = true;
//        
//         } else {
//       
//           dist = ((objects[i].bbopposite)[1] - rayorigin[1])/rayvec[1];
//           intersection2[0] = rayorigin[0] + dist * rayvec[0];
//           intersection2[2] = rayorigin[2] + dist * rayvec[2];
//           
//           if ((intersection1[0] < (objects[i].bborigin)[0] && intersection2[0] < (objects[i].bborigin)[0])
//               ||
//               (intersection1[2] < (objects[i].bborigin)[2] && intersection2[2] < (objects[i].bborigin)[2])
//               ||
//               (intersection1[0] > (objects[i].bbopposite)[0] && intersection2[0] > (objects[i].bbopposite)[0])
//               ||
//               (intersection1[2] > (objects[i].bbopposite)[2] && intersection2[2] > (objects[i].bbopposite)[2])) {
//                   
//             return -1.0;    
//               
//             } else if 
//              ((intersection2[0] > (objects[i].bborigin)[0] && intersection2[0] < (objects[i].bbopposite)[0] && 
//                intersection2[2] > (objects[i].bborigin)[2] && intersection2[2] < (objects[i].bbopposite)[2])
//               ||
//               (intersection1[0] < (objects[i].bborigin)[0] && intersection1[2] > (objects[i].bborigin)[2] && intersection1[2] < (objects[i].bbopposite)[2] &&
//                intersection2[0] > (objects[i].bbopposite)[0] && intersection2[2] > (objects[i].bborigin)[2] && intersection2[2] < (objects[i].bbopposite)[2])
//               ||
//               (intersection2[0] < (objects[i].bborigin)[0] && intersection2[2] > (objects[i].bborigin)[2] && intersection2[2] < (objects[i].bbopposite)[2] &&
//                intersection1[0] > (objects[i].bbopposite)[0] && intersection1[2] > (objects[i].bborigin)[2] && intersection1[2] < (objects[i].bbopposite)[2])
//               ||
//               (intersection1[2] < (objects[i].bborigin)[2] && intersection1[0] > (objects[i].bborigin)[0] && intersection1[0] < (objects[i].bbopposite)[0] &&
//                intersection2[2] > (objects[i].bbopposite)[2] && intersection2[0] > (objects[i].bborigin)[0] && intersection2[0] < (objects[i].bbopposite)[0])
//               ||
//               (intersection2[2] < (objects[i].bborigin)[2] && intersection2[0] > (objects[i].bborigin)[0] && intersection2[0] < (objects[i].bbopposite)[0] &&
//                intersection1[2] > (objects[i].bbopposite)[2] && intersection1[0] > (objects[i].bborigin)[0] && intersection1[0] < (objects[i].bbopposite)[0])) {
//           
//             hit = true;
//       
//           }
//           
//         }
//         
//     }
//     
//     if (rayvec[2] != 0.0 && !hit){
//         
//         dist = ((objects[i].bborigin)[2] - rayorigin[2])/rayvec[2];
//         intersection1[1] = rayorigin[1] + dist * rayvec[1];
//         intersection1[0] = rayorigin[0] + dist * rayvec[0];
//       
//         if (intersection1[1] > (objects[i].bborigin)[1] && intersection1[1] < (objects[i].bbopposite)[1] && 
//             intersection1[0] > (objects[i].bborigin)[0] && intersection1[0] < (objects[i].bbopposite)[0]) {
//           
//           hit = true;
//        
//         } else {
//       
//           dist = ((objects[i].bbopposite)[2] - rayorigin[2])/rayvec[2];
//           intersection2[1] = rayorigin[1] + dist * rayvec[1];
//           intersection2[0] = rayorigin[0] + dist * rayvec[0];
//           
//           if ((intersection1[1] < (objects[i].bborigin)[1] && intersection2[1] < (objects[i].bborigin)[1])
//               ||
//               (intersection1[0] < (objects[i].bborigin)[0] && intersection2[0] < (objects[i].bborigin)[0])
//               ||
//               (intersection1[1] > (objects[i].bbopposite)[1] && intersection2[1] > (objects[i].bbopposite)[1])
//               ||
//               (intersection1[0] > (objects[i].bbopposite)[0] && intersection2[0] > (objects[i].bbopposite)[0])) {
//                   
//             return -1.0;    
//               
//             } else if 
//              ((intersection2[1] > (objects[i].bborigin)[1] && intersection2[1] < (objects[i].bbopposite)[1] && 
//                intersection2[0] > (objects[i].bborigin)[0] && intersection2[0] < (objects[i].bbopposite)[0])
//               ||
//               (intersection1[1] < (objects[i].bborigin)[1] && intersection1[0] > (objects[i].bborigin)[0] && intersection1[0] < (objects[i].bbopposite)[0] &&
//                intersection2[1] > (objects[i].bbopposite)[1] && intersection2[0] > (objects[i].bborigin)[0] && intersection2[0] < (objects[i].bbopposite)[0])
//               ||
//               (intersection2[1] < (objects[i].bborigin)[1] && intersection2[0] > (objects[i].bborigin)[0] && intersection2[0] < (objects[i].bbopposite)[0] &&
//                intersection1[1] > (objects[i].bbopposite)[1] && intersection1[0] > (objects[i].bborigin)[0] && intersection1[0] < (objects[i].bbopposite)[0])
//               ||
//               (intersection1[0] < (objects[i].bborigin)[0] && intersection1[1] > (objects[i].bborigin)[1] && intersection1[1] < (objects[i].bbopposite)[1] &&
//                intersection2[0] > (objects[i].bbopposite)[0] && intersection2[1] > (objects[i].bborigin)[1] && intersection2[1] < (objects[i].bbopposite)[1])
//               ||
//               (intersection2[0] < (objects[i].bborigin)[0] && intersection2[1] > (objects[i].bborigin)[1] && intersection2[1] < (objects[i].bbopposite)[1] &&
//                intersection1[0] > (objects[i].bbopposite)[0] && intersection1[1] > (objects[i].bborigin)[1] && intersection1[1] < (objects[i].bbopposite)[1])) {
//           
//             hit = true;
//       
//           }
//           
//         }
//          
//     }
//     
//     
//     if (hit){
//     ****MAH****end
    
    if (objects[i].type == triangle) return intersectTriangle(i, rayorigin, rayvec);
    else if (objects[i].type == sphere) return intersectSphere(i, rayorigin, rayvec);
    

//     } else return -1.0;
}


bool isVisible(double* lightdir, double* hitpoint, const double maxdist){
  int i;
  double dist;
  double temphit[3];
  
  temphit[0] = hitpoint[0] + EPSILON * lightdir[0];// shift hitpoint slightly
  temphit[1] = hitpoint[1] + EPSILON * lightdir[1];
  temphit[2] = hitpoint[2] + EPSILON * lightdir[2];
  
  dist = intersectGrid(temphit, lightdir, grid, maxdist, &i);
  
  if (dist > 0.0 && dist + EPSILON < maxdist) return false;
  
  return true;
  
}


void shootRay(double* rayorigin, double* rayvec, const int depth, double* rayColor){
  double normal[3], hitpoint[3], halfdir[3], lightdir[3], tempcolor[3], shiftedorigin[3], newray[3], tempnormal[3];
  double dist, shiny, atten, diffactor, specfactor, mindist = INFINITY, norm, dott;
  double* diffuse;
  double* specular;
  int j, hit;
  
  shiftedorigin[0] = rayorigin[0] + EPSILON * rayvec[0];// shift origin slightly
  shiftedorigin[1] = rayorigin[1] + EPSILON * rayvec[1];
  shiftedorigin[2] = rayorigin[2] + EPSILON * rayvec[2];
  
  mindist = intersectGrid(shiftedorigin, rayvec, grid, INFINITY, &hit);
  
  if (mindist > 0.0) {
    
    hitpoint[0] = rayorigin[0] + mindist * rayvec[0];
    hitpoint[1] = rayorigin[1] + mindist * rayvec[1];
    hitpoint[2] = rayorigin[2] + mindist * rayvec[2];
    
    
    rayColor[0] = (objects[hit].ambient)[0] + (objects[hit].emission)[0];
    rayColor[1] = (objects[hit].ambient)[1] + (objects[hit].emission)[1];
    rayColor[2] = (objects[hit].ambient)[2] + (objects[hit].emission)[2];
    
    diffuse = objects[hit].diffuse;
    specular = objects[hit].specular;
    shiny = objects[hit].shininess;
    
    if ( ! (diffuse[0] == 0.0 && diffuse[1] == 0.0 && diffuse[2] == 0.0 && specular[0] == 0.0 && specular[1] == 0.0 && specular[2] == 0.0 )) {
    
      if (objects[hit].type == triangle){
        
        normal[0] = objects[hit].normal[0];
        normal[1] = objects[hit].normal[1];
        normal[2] = objects[hit].normal[2];
      
      } else {
    
        tempnormal[0] = (objects[hit].invtransform)[0][0] * hitpoint[0] + 
                        (objects[hit].invtransform)[1][0] * hitpoint[1] + 
                        (objects[hit].invtransform)[2][0] * hitpoint[2] +
                        (objects[hit].invtransform)[3][0];
        tempnormal[1] = (objects[hit].invtransform)[0][1] * hitpoint[0] + 
                        (objects[hit].invtransform)[1][1] * hitpoint[1] + 
                        (objects[hit].invtransform)[2][1] * hitpoint[2] +
                        (objects[hit].invtransform)[3][1]; 
        tempnormal[2] = (objects[hit].invtransform)[0][2] * hitpoint[0] + 
                        (objects[hit].invtransform)[1][2] * hitpoint[1] + 
                        (objects[hit].invtransform)[2][2] * hitpoint[2] +
                        (objects[hit].invtransform)[3][2];
      
        normal[0] = (objects[hit].invtransform)[0][0] * tempnormal[0] + 
                    (objects[hit].invtransform)[0][1] * tempnormal[1] + 
                    (objects[hit].invtransform)[0][2] * tempnormal[2];
        normal[1] = (objects[hit].invtransform)[1][0] * tempnormal[0] + 
                    (objects[hit].invtransform)[1][1] * tempnormal[1] + 
                    (objects[hit].invtransform)[1][2] * tempnormal[2];
        normal[2] = (objects[hit].invtransform)[2][0] * tempnormal[0] + 
                    (objects[hit].invtransform)[2][1] * tempnormal[1] + 
                    (objects[hit].invtransform)[2][2] * tempnormal[2];
                  
        norm = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
                 
        normal[0] /= norm;      
        normal[1] /= norm;
        normal[2] /= norm;
      
      }
      
    
      for (j = 0; j < numlights; j++){
        if (lights[j].type == directional) {          
        
          lightdir[0] = (lights[j].position)[0];
          lightdir[1] = (lights[j].position)[1];
          lightdir[2] = (lights[j].position)[2];
        
          norm = sqrt(lightdir[0] * lightdir[0] + lightdir[1] * lightdir[1] + lightdir[2] * lightdir[2]);
        
          dist = INFINITY;        
        
        } else {
          
          lightdir[0] = (lights[j].position)[0] - hitpoint[0];
          lightdir[1] = (lights[j].position)[1] - hitpoint[1];
          lightdir[2] = (lights[j].position)[2] - hitpoint[2];
        
          norm = sqrt(lightdir[0] * lightdir[0] + lightdir[1] * lightdir[1] + lightdir[2] * lightdir[2]);
        
          dist = norm;
        
        }
      
        lightdir[0] /= norm;
        lightdir[1] /= norm;
        lightdir[2] /= norm;
        
        diffactor = normal[0] * lightdir[0] + normal[1] * lightdir[1] + normal[2] * lightdir[2];
      
        if ( diffactor > 0.0 ) {
          
          if (isVisible(lightdir, hitpoint, dist)) {
            
            halfdir[0] = -rayvec[0] + lightdir[0];
            halfdir[1] = -rayvec[1] + lightdir[1];
            halfdir[2] = -rayvec[2] + lightdir[2];
        
            norm = sqrt(halfdir[0] * halfdir[0] + halfdir[1] * halfdir[1] + halfdir[2] * halfdir[2]);
        
            halfdir[0] /= norm;
            halfdir[1] /= norm;
            halfdir[2] /= norm;        
        
            if (dist < INFINITY)
              atten = (lights[j].attenuation)[0] + (lights[j].attenuation)[1] * dist + (lights[j].attenuation)[2] * dist * dist;
            else
              atten = 1.0;
            
            specfactor = pow( max(normal[0] * halfdir[0] + normal[1] * halfdir[1] + normal[2] * halfdir[2], 0.0), shiny);
            
            rayColor[0] += (lights[j].color)[0] / atten  * (diffuse[0] * diffactor + specular[0] * specfactor);
            rayColor[1] += (lights[j].color)[1] / atten  * (diffuse[1] * diffactor + specular[1] * specfactor);
            rayColor[2] += (lights[j].color)[2] / atten  * (diffuse[2] * diffactor + specular[2] * specfactor);
              
          }
        }
      }
    
      if (depth > 0 && !(specular[0] == 0.0 && specular[1] == 0.0 && specular[2] == 0.0)) {
        
        dott = rayvec[0] * normal[0] + rayvec[1] * normal[1] + rayvec[2] * normal[2];
      
        newray[0] = rayvec[0] - 2.0 * dott * normal[0];
        newray[1] = rayvec[1] - 2.0 * dott * normal[1];
        newray[2] = rayvec[2] - 2.0 * dott * normal[2];
      
        shootRay(hitpoint, newray, depth - 1, tempcolor);
      
        rayColor[0] += tempcolor[0] * specular[0];
        rayColor[1] += tempcolor[1] * specular[1];
        rayColor[2] += tempcolor[2] * specular[2];
      
      }
    }
  } else {
    rayColor[0] = 0.0; rayColor[1] = 0.0; rayColor[2] = 0.0; 
  }
  
}
