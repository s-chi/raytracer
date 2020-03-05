
#include <iostream>
using namespace std;
#include "basic.h"
#include "variables.h"
#include "raycast.h"
#include "gridcube.h"

void createGrid(void){
  
  int i, j;//, k;
  double origin[3], opposite[3], center[3], maxdist = 0.0;
  
  for (i = 0; i < 3; i++){
    
    origin[i] = INFINITY;
    opposite[i] = -INFINITY;
      
  }
  
  for (i = 0; i < numobj; i++) {//Find smallest box containing whole scene
    
    for (j = 0; j < 3; j++){
        
      if ((objects[i].bborigin)[j] < origin[j]) origin[j] = (objects[i].bborigin)[j];
      if ((objects[i].bbopposite)[j] > opposite[j]) opposite[j] = (objects[i].bbopposite)[j];
        
    }
      
  }
  
//   cout << "Scenebox found.\n";
  
//****
  
  for (i = 0; i < 3; i++){//Now find smallest cube containing box 
      
    if ((opposite[i] - origin[i]) > maxdist) maxdist = opposite[i] - origin[i];
    
    center[i] = (origin[i] + opposite[i])/2.0;
  }
  
  maxdist /= 2.0;  
  
  grid = new cube;
   
  for (i = 0; i < 3; i++){
    
    grid->origin[i] = center[i] - maxdist;
    grid->opposite[i] = center[i] + maxdist;
      
  }
  
//   cout << "Scenecube has origin " << grid->origin[0] << " " << grid->origin[1] << " " << grid->origin[2] << "\n";
//   cout << "Scenecube has opposite " << grid->opposite[0] << " " << grid->opposite[1] << " " << grid->opposite[2] << "\n";
  
//****
  
//   grid = new cube;
//    
//   for (i = 0; i < 3; i++){
//     
//     grid->origin[i] = origin[i];
//     grid->opposite[i] = opposite[i];
//       
//   }
  
  grid->filledSubCubes = NULL;
  grid->objs = NULL;

  for (i = 0; i < numobj; i++) {
    
//     objdepth = 0;
//     
//     cout << "Setting object " << i + 437661 << "...\n";
//     cout << "BBorigin is " << objects[i].bborigin[0] << " " << objects[i].bborigin[1] << " " << objects[i].bborigin[2] << "\n";
//     cout << "BBopposite is " << objects[i].bbopposite[0] << " " << objects[i].bbopposite[1] << " " << objects[i].bbopposite[2] << "\n";
//     cout << "Transformation matrix:\n";
//     for (j = 0; j < 4; j++){
//       for (k = 0; k < 4; k++){
//         cout << (objects[i].transform)[j][k] << " ";
//       }
//       cout << "\n";
//     }
  
//     deeper = 0;
    setGridObj(i, grid);
    
//     cout << "Object " << i << " set at depth " << objdepth << ".\n";
      
  }
   
  for (i = 0; i < 3; i++){//Now that the grid is populated, substitute bigger cube with original box
    
    grid->origin[i] = origin[i];
    grid->opposite[i] = opposite[i];
      
  }
    
}

void deleteGrid(cube* Cube){
    
    cubelist *cubecursor, *cubecursorprev;
    intlist *cursor, *cursorprev;
    
    
    
    while (Cube->filledSubCubes != NULL) {
        
      cubecursor = Cube->filledSubCubes;
      cubecursorprev = NULL;
      
      while (cubecursor->next != NULL) {
          
          cubecursorprev = cubecursor;
          cubecursor = cubecursor->next;
        
      }
      
      deleteGrid(cubecursor->Cube);
      delete cubecursor;
      if(cubecursorprev != NULL)  
        cubecursorprev->next = NULL;
      else
        Cube->filledSubCubes = NULL;
            
    }
    
    
    while (Cube->objs != NULL) {
        
      cursor = Cube->objs;
      cursorprev = NULL;
      
      while (cursor->next != NULL) {
          
          cursorprev = cursor;
          cursor = cursor->next;
        
      }
      
      delete cursor;
      if(cursorprev != NULL)  
        cursorprev->next = NULL;
      else
        Cube->objs = NULL;
            
    }
    
    delete Cube;
    Cube = NULL;
    
}


void appendObj(cube *curCube, const int objindex){
    
    intlist *cursor;
    
    if (curCube->objs == NULL){
        
      curCube->objs = new intlist;
      curCube->objs->index = objindex;
      curCube->objs->next = NULL;
      
    } else {
        
      cursor = curCube->objs;
      
      while (cursor->next != NULL) {
            
        cursor = cursor->next;
          
      }
      
      cursor->next = new intlist;
      cursor->next->index = objindex;
      cursor->next->next = NULL;
        
    }
    
}


cube* appendSubCube(cube *curCube, int* subcindex){
    
    cubelist *cursor;
    int i;
    double origin[3], opposite[3];
    
//     for (i = 0; i < 3; i++) {
//         
//       origin[i] = ((3 - subcindex[i]) * curCube->origin[i] + (subcindex[i] - 1) * curCube->opposite[i]) / 2.0;
//       opposite[i] = (subcindex[i] * curCube->opposite[i] + (2 - subcindex[i]) * curCube->origin[i]) / 2.0;
//         
//     }
    
//****
    
    for (i = 0; i < 3; i++) {
        
      origin[i] = ((5 - subcindex[i]) * curCube->origin[i] + (subcindex[i] - 1) * curCube->opposite[i]) / 4.0;
      opposite[i] = origin[i] + (curCube->opposite[i] - curCube->origin[i]) / 2.0;
        
    }
    
//****    
    
    if (curCube->filledSubCubes == NULL){
        
      curCube->filledSubCubes = new cubelist;
      curCube->filledSubCubes->Cube = new cube;
      
      for (i = 0; i < 3; i++) {
          
        curCube->filledSubCubes->Cube->origin[i] = origin[i];
        curCube->filledSubCubes->Cube->opposite[i] = opposite[i];
          
      }
      
      curCube->filledSubCubes->next = NULL;
      curCube->filledSubCubes->Cube->objs = NULL;
      curCube->filledSubCubes->Cube->filledSubCubes = NULL;
      
      
      return curCube->filledSubCubes->Cube;
      
      
    } else {
        
      cursor = curCube->filledSubCubes;
      
      if (cursor->Cube->origin[0] == origin[0] && cursor->Cube->origin[1] == origin[1] && cursor->Cube->origin[2] == origin[2])
          return cursor->Cube;
      
      while (cursor->next != NULL) {
            
        cursor = cursor->next;
        
        if (cursor->Cube->origin[0] == origin[0] && cursor->Cube->origin[1] == origin[1] && cursor->Cube->origin[2] == origin[2])
          return cursor->Cube;
          
      }
      
      cursor->next = new cubelist;
      cursor->next->Cube = new cube;
      
      for (i = 0; i < 3; i++) {
          
        cursor->next->Cube->origin[i] = origin[i];
        cursor->next->Cube->opposite[i] = opposite[i];
          
      }
      
      cursor->next->next = NULL;
      cursor->next->Cube->objs = NULL;
      cursor->next->Cube->filledSubCubes = NULL;
      
      return cursor->next->Cube;
        
    }
    
    
}

void setGridObj(const int objindex, cube* curCube){//I already know the obj is fully contained in curCube
    
//     double halfcoords[3];
    
//****
    double sidequarter;
//****
    int subcindex[3] = {0};
    cube* subcube;
    
//****

    sidequarter = ((curCube->opposite)[0] - (curCube->origin)[0]) / 4.0;
    
    if ((objects[objindex].bborigin)[0] > (curCube->origin)[0] + 2.0 * sidequarter) {
        
      subcindex[0] = 3;
      
    } else if ((objects[objindex].bbopposite)[0] < (curCube->origin)[0] + 2.0 * sidequarter) {
        
      subcindex[0] = 1;
        
    } else if ( ((objects[objindex].bborigin)[0] > (curCube->origin)[0] + sidequarter) 
             && ((objects[objindex].bbopposite)[0] < (curCube->origin)[0] + 3.0 * sidequarter) ) {
      
      subcindex[0] = 2;
      
    } 
    
    if ((objects[objindex].bborigin)[1] > (curCube->origin)[1] + 2.0 * sidequarter) {
        
      subcindex[1] = 3;
      
    } else if ((objects[objindex].bbopposite)[1] < (curCube->origin)[1] + 2.0 * sidequarter) {
        
      subcindex[1] = 1;
        
    } else if ( ((objects[objindex].bborigin)[1] > (curCube->origin)[1] + sidequarter) 
             && ((objects[objindex].bbopposite)[1] < (curCube->origin)[1] + 3.0 * sidequarter) ) {
      
      subcindex[1] = 2;
      
    }    
    
    if ((objects[objindex].bborigin)[2] > (curCube->origin)[2] + 2.0 * sidequarter) {
        
      subcindex[2] = 3;
      
    } else if ((objects[objindex].bbopposite)[2] < (curCube->origin)[2] + 2.0 * sidequarter) {
        
      subcindex[2] = 1;
        
    } else if ( ((objects[objindex].bborigin)[2] > (curCube->origin)[2] + sidequarter) 
             && ((objects[objindex].bbopposite)[2] < (curCube->origin)[2] + 3.0 * sidequarter) ) {
      
      subcindex[2] = 2;
      
    } 
    
    if (subcindex[0] * subcindex[1] * subcindex[2] == 0){
        
        appendObj(curCube, objindex);
        
    } else {
        
//       objdepth++;
      
        subcube = appendSubCube(curCube, subcindex);
        setGridObj(objindex, subcube);
        
    }

//****    
    
    
    
//     for (i = 0; i < 3; i++){
//         halfcoords[i] = ((curCube->origin)[i] + (curCube->opposite)[i])/2.0;
//     }
//     
//     if ((objects[objindex].bborigin)[0] > halfcoords[0]) {
//         
//       subcindex[0] = 2;
//       
//     } else if ((objects[objindex].bbopposite)[0] < halfcoords[0]) {
//         
//       subcindex[0] = 1;
//         
//     }
//     
//     if ((objects[objindex].bborigin)[1] > halfcoords[1]) {
//         
//       subcindex[1] = 2;
//       
//     } else if ((objects[objindex].bbopposite)[1] < halfcoords[1]) {
//         
//       subcindex[1] = 1;
//         
//     }
//     
//     if ((objects[objindex].bborigin)[2] > halfcoords[2]) {
//         
//       subcindex[2] = 2;
//       
//     } else if ((objects[objindex].bbopposite)[2] < halfcoords[2]) {
//         
//       subcindex[2] = 1;
//         
//     }
//     
//     if (subcindex[0] * subcindex[1] * subcindex[2] == 0){
//         
//         appendObj(curCube, objindex);
//         
//     } else {
//         
//         subcube = appendSubCube(curCube, subcindex);
//         setGridObj(objindex, subcube);
//         
//     }



    
}


// double intersectGrid(double* rayorigin, double* rayvec, cube* curCube, const double curDist, int* hit){//gridcubes aligned to axes in scene coords
//     
//     double dist, mindist = INFINITY, newdist, intersection[3];
//     int newhit;
//     intlist* objcursor;
//     cubelist* cubecursor;
//     
//     newdist = curDist;
//     
//     if (rayorigin[0] < (curCube->origin)[0] || rayorigin[0] > (curCube->opposite)[0] ||
//         rayorigin[1] < (curCube->origin)[1] || rayorigin[1] > (curCube->opposite)[1] ||
//         rayorigin[2] < (curCube->origin)[2] || rayorigin[2] > (curCube->opposite)[2]) {//if the ray origin is inside the cube we already know that the cube is hit
//     
//       if (rayvec[0] != 0.0){
//         
//         dist = ((curCube->origin)[0] - rayorigin[0])/rayvec[0];
//         intersection[1] = rayorigin[1] + dist * rayvec[1];
//         intersection[2] = rayorigin[2] + dist * rayvec[2];
//       
//         if (dist > 0.0 && dist < mindist && 
//             intersection[1] > (curCube->origin)[1] && intersection[1] < (curCube->opposite)[1] && 
//             intersection[2] > (curCube->origin)[2] && intersection[2] < (curCube->opposite)[2]) {
//           
//           mindist = dist;
//        
//         }
//       
//         dist = ((curCube->opposite)[0] - rayorigin[0])/rayvec[0];
//         intersection[1] = rayorigin[1] + dist * rayvec[1];
//         intersection[2] = rayorigin[2] + dist * rayvec[2];
//       
//         if (dist > 0.0 && dist < mindist && 
//             intersection[1] > (curCube->origin)[1] && intersection[1] < (curCube->opposite)[1] && 
//             intersection[2] > (curCube->origin)[2] && intersection[2] < (curCube->opposite)[2]) {
//           
//           mindist = dist;
//       
//         }
//          
//       }
//     
//       if (rayvec[1] != 0.0){
//         
//         dist = ((curCube->origin)[1] - rayorigin[1])/rayvec[1];
//         intersection[0] = rayorigin[0] + dist * rayvec[0];
//         intersection[2] = rayorigin[2] + dist * rayvec[2];
//       
//         if (dist > 0.0 && dist < mindist && 
//             intersection[0] > (curCube->origin)[0] && intersection[0] < (curCube->opposite)[0] && 
//             intersection[2] > (curCube->origin)[2] && intersection[2] < (curCube->opposite)[2]) {
//           
//           mindist = dist;
//       
//         }
//       
//         dist = ((curCube->opposite)[1] - rayorigin[1])/rayvec[1];
//         intersection[0] = rayorigin[0] + dist * rayvec[0];
//         intersection[2] = rayorigin[2] + dist * rayvec[2];
//       
//         if (dist > 0.0 && dist < mindist && 
//             intersection[0] > (curCube->origin)[0] && intersection[0] < (curCube->opposite)[0] && 
//             intersection[2] > (curCube->origin)[2] && intersection[2] < (curCube->opposite)[2]) {
//           
//           mindist = dist;
//       
//         }
//          
//       }
//     
//       if (rayvec[2] != 0.0){
//         
//         dist = ((curCube->origin)[2] - rayorigin[2])/rayvec[2];
//         intersection[1] = rayorigin[1] + dist * rayvec[1];
//         intersection[0] = rayorigin[0] + dist * rayvec[0];
//       
//         if (dist > 0.0 && dist < mindist && 
//             intersection[1] > (curCube->origin)[1] && intersection[1] < (curCube->opposite)[1] && 
//             intersection[0] > (curCube->origin)[0] && intersection[0] < (curCube->opposite)[0]) {
//           
//           mindist = dist;
//       
//         }
//       
//         dist = ((curCube->opposite)[2] - rayorigin[2])/rayvec[2];
//         intersection[1] = rayorigin[1] + dist * rayvec[1];
//         intersection[0] = rayorigin[0] + dist * rayvec[0];
//       
//         if (dist > 0.0 && dist < mindist && 
//             intersection[1] > (curCube->origin)[1] && intersection[1] < (curCube->opposite)[1] && 
//             intersection[0] > (curCube->origin)[0] && intersection[0] < (curCube->opposite)[0]) {
//           
//           mindist = dist;
//       
//         }
//          
//       }
//     
//       if (mindist > curDist || mindist == INFINITY) return -1.0;
//     
//     }
//     
//     for (objcursor = curCube->objs; objcursor != NULL; objcursor = objcursor->next){
//         
//         dist = intersectObj(objcursor->index, rayorigin, rayvec);
//         
//         if (dist > 0.0 && dist < newdist) {
//           
//           newdist = dist;
//           *hit = objcursor->index;
//           
//         }
//             
//             
//         
//     }
//     
//     
//     for (cubecursor = curCube->filledSubCubes; cubecursor != NULL; cubecursor = cubecursor->next){
//         
//         dist = intersectGrid(rayorigin, rayvec, cubecursor->Cube, newdist, &newhit);
//         
//         if (dist > 0.0 && dist < newdist) {
//           
//           newdist = dist;
//           *hit = newhit;
//           
//         }
//         
//     }
//         
//     
//     if (newdist < curDist) return newdist;
//     
//     return -1.0;
//     
// }

double intersectGrid(double* rayorigin, double* rayvec, cube* curCube, const double curDist, int* hit){//gridcubes aligned to axes in scene coords
    
    double dist, mindist = INFINITY, newdist, intersection1[3], intersection2[3];
    int newhit;
    intlist* objcursor;
    cubelist* cubecursor;
    
    newdist = curDist;
    
    if (rayorigin[0] < (curCube->origin)[0] || rayorigin[0] > (curCube->opposite)[0] ||
        rayorigin[1] < (curCube->origin)[1] || rayorigin[1] > (curCube->opposite)[1] ||
        rayorigin[2] < (curCube->origin)[2] || rayorigin[2] > (curCube->opposite)[2]) {//if the ray origin is inside the cube we already know that the cube is hit
    
      if (rayvec[0] != 0.0){
        
        dist = ((curCube->origin)[0] - rayorigin[0])/rayvec[0];
          
        intersection1[1] = rayorigin[1] + dist * rayvec[1];
        intersection1[2] = rayorigin[2] + dist * rayvec[2];
          
        if (dist > 0.0 && dist < mindist && 
            intersection1[1] > (curCube->origin)[1] && intersection1[1] < (curCube->opposite)[1] && 
            intersection1[2] > (curCube->origin)[2] && intersection1[2] < (curCube->opposite)[2]) {
          
          mindist = dist;
       
        }
      
        dist = ((curCube->opposite)[0] - rayorigin[0])/rayvec[0];
                  
        intersection2[1] = rayorigin[1] + dist * rayvec[1];
        intersection2[2] = rayorigin[2] + dist * rayvec[2];
          
        if ((intersection1[1] < (curCube->origin)[1]   && intersection2[1] < (curCube->origin)[1]  ) ||
            (intersection1[1] > (curCube->opposite)[1] && intersection2[1] > (curCube->opposite)[1]) ||
            (intersection1[2] < (curCube->origin)[2]   && intersection2[2] < (curCube->origin)[2]  ) ||
            (intersection1[2] > (curCube->opposite)[2] && intersection2[2] > (curCube->opposite)[2])  ){
          
          return -1.0;
          
        }
        
        if (dist > 0.0 && dist < mindist && 
            intersection2[1] > (curCube->origin)[1] && intersection2[1] < (curCube->opposite)[1] && 
            intersection2[2] > (curCube->origin)[2] && intersection2[2] < (curCube->opposite)[2]) {
          
          mindist = dist;
      
        }
                 
      }
    
      if (rayvec[1] != 0.0){
        
        dist = ((curCube->origin)[1] - rayorigin[1])/rayvec[1];
          
        intersection1[0] = rayorigin[0] + dist * rayvec[0];
        intersection1[2] = rayorigin[2] + dist * rayvec[2];
          
        if (dist > 0.0 && dist < mindist && 
            intersection1[0] > (curCube->origin)[0] && intersection1[0] < (curCube->opposite)[0] && 
            intersection1[2] > (curCube->origin)[2] && intersection1[2] < (curCube->opposite)[2]) {
          
          mindist = dist;
       
        }
      
        dist = ((curCube->opposite)[1] - rayorigin[1])/rayvec[1];
                  
        intersection2[0] = rayorigin[0] + dist * rayvec[0];
        intersection2[2] = rayorigin[2] + dist * rayvec[2];
          
        if ((intersection1[0] < (curCube->origin)[0]   && intersection2[0] < (curCube->origin)[0]  ) ||
            (intersection1[0] > (curCube->opposite)[0] && intersection2[0] > (curCube->opposite)[0]) ||
            (intersection1[2] < (curCube->origin)[2]   && intersection2[2] < (curCube->origin)[2]  ) ||
            (intersection1[2] > (curCube->opposite)[2] && intersection2[2] > (curCube->opposite)[2])  ){
          
          return -1.0;
          
        }
        
        if (dist > 0.0 && dist < mindist && 
            intersection2[0] > (curCube->origin)[0] && intersection2[0] < (curCube->opposite)[0] && 
            intersection2[2] > (curCube->origin)[2] && intersection2[2] < (curCube->opposite)[2]) {
          
          mindist = dist;
      
        }
                 
      }
      
      if (rayvec[2] != 0.0){
        
        dist = ((curCube->origin)[2] - rayorigin[2])/rayvec[2];
          
        intersection1[1] = rayorigin[1] + dist * rayvec[1];
        intersection1[0] = rayorigin[0] + dist * rayvec[0];
          
        if (dist > 0.0 && dist < mindist && 
            intersection1[1] > (curCube->origin)[1] && intersection1[1] < (curCube->opposite)[1] && 
            intersection1[0] > (curCube->origin)[0] && intersection1[0] < (curCube->opposite)[0]) {
          
          mindist = dist;
       
        }
      
        dist = ((curCube->opposite)[2] - rayorigin[2])/rayvec[2];
                  
        intersection2[1] = rayorigin[1] + dist * rayvec[1];
        intersection2[0] = rayorigin[0] + dist * rayvec[0];
          
        if ((intersection1[1] < (curCube->origin)[1]   && intersection2[1] < (curCube->origin)[1]  ) ||
            (intersection1[1] > (curCube->opposite)[1] && intersection2[1] > (curCube->opposite)[1]) ||
            (intersection1[0] < (curCube->origin)[0]   && intersection2[0] < (curCube->origin)[0]  ) ||
            (intersection1[0] > (curCube->opposite)[0] && intersection2[0] > (curCube->opposite)[0])  ){
          
          return -1.0;
          
        }
        
        if (dist > 0.0 && dist < mindist && 
            intersection2[1] > (curCube->origin)[1] && intersection2[1] < (curCube->opposite)[1] && 
            intersection2[0] > (curCube->origin)[0] && intersection2[0] < (curCube->opposite)[0]) {
          
          mindist = dist;
      
        }
                 
      }
    
      if (mindist > curDist || mindist == INFINITY) return -1.0;
    
    }
    
    for (objcursor = curCube->objs; objcursor != NULL; objcursor = objcursor->next){
        
        dist = intersectObj(objcursor->index, rayorigin, rayvec);
        
        if (dist > 0.0 && dist < newdist) {
          
          newdist = dist;
          *hit = objcursor->index;
          
        }
            
            
        
    }
    
    
    for (cubecursor = curCube->filledSubCubes; cubecursor != NULL; cubecursor = cubecursor->next){
        
        dist = intersectGrid(rayorigin, rayvec, cubecursor->Cube, newdist, &newhit);
        
        if (dist > 0.0 && dist < newdist) {
          
          newdist = dist;
          *hit = newhit;
          
        }
        
    }
        
    
    if (newdist < curDist) return newdist;
    
    return -1.0;
    
}

