#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>

using namespace std;
#include "basic.h"
#include "variables.h"
#include "Transform.h"
#include "readfile.h"


bool readvals(stringstream &s, const int numvals, double* values) 
{
  for (int i = 0; i < numvals; i++) {
    s >> values[i]; 
    if (s.fail()) {
      cout << "Failed reading value " << i << " will skip\n"; 
      return false;
    }
  }
  return true; 
}


void readfile(const char* filename) 
{
  string str, cmd; 
  ifstream in;
  in.open(filename); 
  if (in.is_open()) {

    stack <mat4> transfstack;
    stack <mat4> invtransfstack;
    transfstack.push(mat4(1.0));
    invtransfstack.push(mat4(1.0));
    vec3 trOrigin, trV1, trV2;
    double trNorm1, trNorm2, trShear, trScale, norm;
    int v[3], curvert = 0, curobj = 0, curlight = 0;
//     double position[3];
    mat4 temptransf, invtemptransf;
    vec4 tempvec[8];


// Read the file a first time to find out the total number of vertices, objects and lights
    getline (in, str);
    while (in) {
      if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
	stringstream s(str);
        s >> cmd;
	
	if (cmd == "vertex")
	  numverts++;
	else if (cmd == "tri" || cmd == "sphere")
	  numobj++;
	else if (cmd == "point" || cmd == "directional")
	  numlights++;
	
      }
      getline (in, str);
    }
    
    
    cout << "There are " << numverts << " vertices, " << numobj << " objects, and " << numlights << " lights.\n";
    
    //Allocate the right amount of memory
    vertices = new double[3 * numverts];
    objects = new object[numobj];
    lights = new light[numlights + 1];
    
    //Go back to beginning of file    
    in.clear();
    in.seekg(0, ios::beg);
    
    //Now read the file again loading the values in memory
    getline (in, str); 
    while (in) {
      if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
        // Ruled out comment and blank lines 

        stringstream s(str);
        s >> cmd; 
        int i, j; 
        double values[10]; // Position and color for light, colors for others
        // Up to 10 params for cameras.  
        bool validinput; // Validity of input 

        
        if (cmd == "size") {
	  validinput = readvals(s, 2, values); 
          if (validinput) {
	    
	    width = values[0];	    
	    heigth = values[1];
	    
	  }
	}
	
	else if (cmd == "maxdepth") {
	  validinput = readvals(s, 1, values); 
          if (validinput) {
	    
	    raydepth = values[0];
	    
	  }
	}
	
	else if (cmd == "output") {
	  
	  s >> output;
	  
	}
	
	else if (cmd == "camera") {
          validinput = readvals(s,10,values);
          if (validinput) {
	    
            for (i = 0; i < 3; i++) {
              eye[i] = values[i]; 
	      center[i] = values[i + 3];
	      up[i] = values[i + 6];
            }
            
            fovy = values[9];

          }
        }
        
	else if (cmd == "maxverts") {//This is not used
	  validinput = readvals(s, 1, values); 
          if (validinput) {
	    
	    maxverts = values[0];
	    
	  }
	}
	
	else if (cmd == "vertex") {
	  validinput = readvals(s, 3, values); 
          if (validinput) {

	    for (j = 0; j < 3; j++)   
	      vertices[3 * curvert + j] = values[j];
	    
	    curvert++;

	  }
	}

	
	else if (cmd == "tri") {
	  validinput = readvals(s, 3, values); 
          if (validinput) {
            
            for (i = 0; i < 3; i++) {
	      (objects[curobj].ambient)[i] = ambient[i]; 
              (objects[curobj].diffuse)[i] = diffuse[i]; 
              (objects[curobj].specular)[i] = specular[i]; 
              (objects[curobj].emission)[i] = emission[i];
            }
	    
	    objects[curobj].shininess = shininess;
	    objects[curobj].type = triangle;
	    
	    for (i = 0; i < 3; i++)
	      v[i] = values[i];
	    
	    trOrigin = vec3(vertices[3 * v[0]], vertices[3 * v[0] + 1], vertices[3 * v[0] + 2]);
	    trV1 = vec3(vertices[3 * v[1]], vertices[3 * v[1] + 1], vertices[3 * v[1] + 2]) - trOrigin;
	    trV2 = vec3(vertices[3 * v[2]], vertices[3 * v[2] + 1], vertices[3 * v[2] + 2]) - trOrigin;
	    trNorm1 = glm::length(trV1);
	    trNorm2 = glm::length(trV2);
	    trShear = glm::dot(trV2, trV1) / trNorm2;
	    trScale = sqrt(pow(trNorm1, 2.0) - pow(trShear, 2.0));            
            
            temptransf = transfstack.top() 
	               * Transform::lookAtInv(trOrigin, trOrigin + glm::normalize(glm::cross(trV2, trV1)), trV2) //cross product "normalized", otherwise can be much smaller than trOrigin
                       * Transform::scale(trScale, 1.0, 1.0)
                       * Transform::shear(trShear, 1, 0)
                       * Transform::scale(1.0, trNorm2, 1.0);
					
	    invtemptransf = Transform::scale( 1.0, 1.0 / trNorm2, 1.0 )
	                  * Transform::shear(- trShear, 1, 0)
			  * Transform::scale(1.0 / trScale, 1.0, 1.0)
			  * Transform::lookAt(trOrigin, trOrigin + glm::cross(trV2, trV1), trV2)
	                  * invtransfstack.top();
                          
            for (i = 0; i < 4; i++){
                for (j = 0; j < 4; j++){
                    (objects[curobj].transform)[i][j] = temptransf[i][j];
                    (objects[curobj].invtransform)[i][j] = invtemptransf[i][j];
                }
            }
            
            for (i = 0; i < 3; i++){
              
              objects[curobj].bborigin[i] = INFINITY;
              objects[curobj].bbopposite[i] = -INFINITY;
                
            }
            
            tempvec[0] = temptransf * vec4(0.0, 0.0, 0.0, 1.0);
            tempvec[1] = temptransf * vec4(1.0, 0.0, 0.0, 1.0);
            tempvec[2] = temptransf * vec4(0.0, 1.0, 0.0, 1.0);
            
            for (i = 0; i < 3; i++){
              for (j = 0; j < 3; j++){
                
                if ( (tempvec[j])[i] < objects[curobj].bborigin[i] ) objects[curobj].bborigin[i] = (tempvec[j])[i];
                if ( (tempvec[j])[i] > objects[curobj].bbopposite[i] ) objects[curobj].bbopposite[i] = (tempvec[j])[i];
                  
              }
            }
            
//             	    if (curobj == 415851) {
//               
//               cout << "For obj 415851:\n";
//               cout << "trOrigin: " << trOrigin[0] << " " <<  trOrigin[1] << " " << trOrigin[2] << "\n";
//               cout << "glm::normalize(glm::cross(trV2, trV1)): " << (glm::normalize(glm::cross(trV2, trV1)))[0] << " " << (glm::normalize(glm::cross(trV2, trV1)))[1] << " " << (glm::normalize(glm::cross(trV2, trV1)))[2] << "\n";
//               cout << "trV2: " << trV2[0] << " " <<  trV2[1] << " " << trV2[2] << "\n";
//               cout << "trV1: " << trV1[0] << " " <<  trV1[1] << " " << trV1[2] << "\n";
//               
//               cout << "transfstack.top():\n";
//               for (i = 0; i < 4; i++){
//                 for (j = 0; j < 4; j++){
//                   cout << (transfstack.top())[i][j] << " ";
//                 }
//                 cout << "\n";
//               }
//               
//               cout << "lookatinv:\n";
//               for (i = 0; i < 4; i++){
//                 for (j = 0; j < 4; j++){
//                   cout << (Transform::lookAtInv(trOrigin, trOrigin + glm::normalize(glm::cross(trV2, trV1)), trV2))[i][j] << " ";
//                 }
//                 cout << "\n";
//               }
//               
//               cout << "scale1:\n";
//               for (i = 0; i < 4; i++){
//                 for (j = 0; j < 4; j++){
//                   cout << (Transform::scale(trScale, 1.0, 1.0))[i][j] << " ";
//                 }
//                 cout << "\n";
//               }              
//               
//               cout << "shear:\n";
//               for (i = 0; i < 4; i++){
//                 for (j = 0; j < 4; j++){
//                   cout << (Transform::shear(trShear, 1, 0))[i][j] << " ";
//                 }
//                 cout << "\n";
//               }
//               
//               cout << "scale2:\n";
//               for (i = 0; i < 4; i++){
//                 for (j = 0; j < 4; j++){
//                   cout << (Transform::scale(1.0, trNorm2, 1.0))[i][j] << " ";
//                 }
//                 cout << "\n";
//               }   
//               
//               cout << "temptransf:\n";
//               for (i = 0; i < 4; i++){
//                 for (j = 0; j < 4; j++){
//                   cout << (temptransf)[i][j] << " ";
//                 }
//                 cout << "\n";
//               }   
//               
//               
// //               exit(0);
//             }
            
            
//****
      
//             for (i = 0; i < 3; i++){
//               for (j = 0; j < 3; j++){
//                 
//                 if ( (tempvec[j])[i] < objects[curobj].bborigin[0] ) objects[curobj].bborigin[0] = (tempvec[j])[i];
//                 if ( (tempvec[j])[i] > objects[curobj].bbopposite[0] ) objects[curobj].bbopposite[0] = (tempvec[j])[i];
//                   
//               }
//             }
//             
//             objects[curobj].bborigin[1] = objects[curobj].bborigin[0];
//             objects[curobj].bborigin[2] = objects[curobj].bborigin[0];
//             objects[curobj].bbopposite[1] = objects[curobj].bbopposite[0];
//             objects[curobj].bbopposite[2] = objects[curobj].bbopposite[0];
            
//****
            
            (objects[curobj].normal)[0] = (objects[curobj].invtransform)[0][2]; //transform the vector 0,0,1
            (objects[curobj].normal)[1] = (objects[curobj].invtransform)[1][2];
            (objects[curobj].normal)[2] = (objects[curobj].invtransform)[2][2];
                                                       
            norm = sqrt((objects[curobj].normal)[0] * (objects[curobj].normal)[0] + 
                        (objects[curobj].normal)[1] * (objects[curobj].normal)[1] + 
                        (objects[curobj].normal)[2] * (objects[curobj].normal)[2]);
                 
            (objects[curobj].normal)[0] /= norm;      
            (objects[curobj].normal)[1] /= norm;
            (objects[curobj].normal)[2] /= norm;
					   
	    curobj++;

	  }
	}
	
	else if (cmd == "sphere") {
	  validinput = readvals(s, 4, values); 
          if (validinput) {

	    for (i = 0; i < 3; i++) {
	      (objects[curobj].ambient)[i] = ambient[i]; 
              (objects[curobj].diffuse)[i] = diffuse[i]; 
              (objects[curobj].specular)[i] = specular[i]; 
              (objects[curobj].emission)[i] = emission[i];
            }
            
            objects[curobj].shininess = shininess;
	    objects[curobj].type = sphere;

	    temptransf = transfstack.top() 
	               * Transform::translate( values[0], values[1], values[2] )
                       * Transform::scale( values[3], values[3], values[3] );
	    invtemptransf = Transform::scale( 1.0/values[3], 1.0/values[3], 1.0/values[3] )
	                  * Transform::translate( - values[0], - values[1], - values[2] ) 
	                  * invtransfstack.top();
                          
            for (i = 0; i < 4; i++){
                for (j = 0; j < 4; j++){
                    (objects[curobj].transform)[i][j] = temptransf[i][j];
                    (objects[curobj].invtransform)[i][j] = invtemptransf[i][j];
                }
            }
            
            for (i = 0; i < 3; i++){
              
              objects[curobj].bborigin[i] = INFINITY;
              objects[curobj].bbopposite[i] = -INFINITY;
                
            }
            
            tempvec[0] = temptransf * vec4(   1.0,   1.0,   1.0, 1.0 );
            tempvec[1] = temptransf * vec4(   1.0,   1.0, - 1.0, 1.0 );
            tempvec[2] = temptransf * vec4(   1.0, - 1.0,   1.0, 1.0 );
            tempvec[3] = temptransf * vec4(   1.0, - 1.0, - 1.0, 1.0 );
            tempvec[4] = temptransf * vec4( - 1.0,   1.0,   1.0, 1.0 );
            tempvec[5] = temptransf * vec4( - 1.0,   1.0, - 1.0, 1.0 );
            tempvec[6] = temptransf * vec4( - 1.0, - 1.0,   1.0, 1.0 );
            tempvec[7] = temptransf * vec4( - 1.0, - 1.0, - 1.0, 1.0 );
            
            for (i = 0; i < 3; i++){
              for (j = 0; j < 8; j++){
                
                if ( (tempvec[j])[i] < objects[curobj].bborigin[i] ) objects[curobj].bborigin[i] = (tempvec[j])[i];
                if ( (tempvec[j])[i] > objects[curobj].bbopposite[i] ) objects[curobj].bbopposite[i] = (tempvec[j])[i];
                  
              }
            }
					   
	    curobj++;

	  }
	}
	
	
	else if (cmd == "translate") {
          validinput = readvals(s,3,values); 
          if (validinput) {
	    
	    transfstack.top() = transfstack.top() * Transform::translate( values[0], values[1], values[2] );
	    invtransfstack.top() = Transform::translate( -values[0], -values[1], -values[2] ) * invtransfstack.top(); 

          }
        }
        
        else if (cmd == "scale") {
          validinput = readvals(s,3,values); 
          if (validinput) {
	    
	    transfstack.top() = transfstack.top() * Transform::scale( values[0], values[1], values[2] );
	    invtransfstack.top() = Transform::scale( 1.0/values[0], 1.0/values[1], 1.0/values[2] ) * invtransfstack.top(); 

          }
        }
        
        else if (cmd == "rotate") {
          validinput = readvals(s,4,values); 
          if (validinput) {
	    
	    vec3 rotaxis = vec3(values[0], values[1], values[2]);
	    transfstack.top() = transfstack.top() * mat4( Transform::rotate(values[3], rotaxis) );
	    invtransfstack.top() = mat4( Transform::rotate(-values[3], rotaxis) ) * invtransfstack.top();

          }
        }

        else if (cmd == "pushTransform") {
	  
          transfstack.push(transfstack.top());
	  invtransfstack.push(invtransfstack.top());
	  
        } 
        
        else if (cmd == "popTransform") {	  
          if (transfstack.size() <= 1) {
            cerr << "Stack has no elements.  Cannot Pop\n"; 
          } else {
            transfstack.pop(); 
	    invtransfstack.pop(); 
          }
        }
	
	else if (cmd == "directional") {
	  validinput = readvals(s, 6, values); 
          if (validinput) {
      
	    for (i = 0; i < 3; i++) {
              (lights[curlight].color)[i] = values[i + 3]; 
              (lights[curlight].attenuation)[i] = 0.0;
            }
            
            (lights[curlight].attenuation)[0] = 1.0;
            
            (lights[curlight].position)[0] = (transfstack.top())[0][0] * values[0] + 
                                             (transfstack.top())[1][0] * values[1] + 
                                             (transfstack.top())[2][0] * values[2];
            (lights[curlight].position)[1] = (transfstack.top())[0][1] * values[0] + 
                                             (transfstack.top())[1][1] * values[1] + 
                                             (transfstack.top())[2][1] * values[2];
            (lights[curlight].position)[2] = (transfstack.top())[0][2] * values[0] + 
                                             (transfstack.top())[1][2] * values[1] + 
                                             (transfstack.top())[2][2] * values[2];
                                             
            lights[curlight].type = directional;
	    
	    curlight++;

	  }
	}
	
	else if (cmd == "point") {
	  validinput = readvals(s, 6, values); 
          if (validinput) {

	    for (i = 0; i < 3; i++) {
              (lights[curlight].color)[i] = values[i + 3]; 
              (lights[curlight].attenuation)[i] = attenuation[i];
            }
            
            (lights[curlight].position)[0] = (transfstack.top())[0][0] * values[0] + 
                                             (transfstack.top())[1][0] * values[1] + 
                                             (transfstack.top())[2][0] * values[2] +
                                             (transfstack.top())[3][0];
            (lights[curlight].position)[1] = (transfstack.top())[0][1] * values[0] + 
                                             (transfstack.top())[1][1] * values[1] + 
                                             (transfstack.top())[2][1] * values[2] +
                                             (transfstack.top())[3][1];
            (lights[curlight].position)[2] = (transfstack.top())[0][2] * values[0] + 
                                             (transfstack.top())[1][2] * values[1] + 
                                             (transfstack.top())[2][2] * values[2] +
                                             (transfstack.top())[3][2];
            
            lights[curlight].type = point;
	    
	    curlight++;

	  }
	}
	
	else if (cmd == "attenuation") {
	  validinput = readvals(s, 3, values); 
          if (validinput)
	    
	   for (i = 0; i < 3; i++)
	    attenuation[i] = values[i]; 
	   
	}
	
	else if (cmd == "ambient") {
	  validinput = readvals(s, 3, values);
          if (validinput)
	    
	    for (i = 0; i < 3; i++) 
	      ambient[i] = values[i];
	    
	}
	
        else if (cmd == "diffuse") {
          validinput = readvals(s, 3, values); 
          if (validinput)
	    
            for (i = 0; i < 3; i++)
              diffuse[i] = values[i];
	    
	}
	
        else if (cmd == "specular") {
          validinput = readvals(s, 3, values); 
          if (validinput)
	    
            for (i = 0; i < 3; i++)
              specular[i] = values[i];
	    
        }
        
        else if (cmd == "emission") {
          validinput = readvals(s, 3, values); 
          if (validinput)
	    
            for (i = 0; i < 3; i++)
              emission[i] = values[i];
	    
        }
        
        else if (cmd == "shininess") {
          validinput = readvals(s, 1, values); 
          if (validinput)
	    
            shininess = values[0];
	  
        }

        else {
          cerr << "Unknown Command: " << cmd << " Skipping \n"; 
        }
      }
      getline (in, str); 
    }

  } 
  else {
    cerr << "Unable to Open Input Data File " << filename << "\n"; 
    throw 2; 
  }
}
