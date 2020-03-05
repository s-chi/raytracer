/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

// This is the basic include file for the global variables in the program.  
// Since all files need access to it, we define EXTERN as either blank or 
// extern, depending on if included in the main program or not.  
#include "basic.h"

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

EXTERN int maxverts;//, objdepth;

enum objtype {triangle, sphere};
enum lighttype {directional, point};

typedef struct myobject object;
typedef struct mylight light;
typedef struct myintlist intlist;
typedef struct mycube cube;
typedef struct mycubelist cubelist;


struct myobject {
  objtype type;
 
  double normal[3];
  double bborigin[3];
  double bbopposite[3];
  
  double transform[4][4];
  double invtransform[4][4];
  double ambient[3] ; 
  double diffuse[3] ; 
  double specular[3] ;
  double emission[3] ; 
  double shininess ;
};

struct mylight {
  lighttype type;  
    
  double position[3];
  
  double color[3] ; 
  double attenuation[3] ;
};

struct myintlist {
  int index;
  struct myintlist *next;
};

struct mycube {
  intlist *objs;
  cubelist *filledSubCubes;
  double origin[3];
  double opposite[3];
};

struct mycubelist {
  cube *Cube;
  struct mycubelist *next;
};

#ifdef MAINPROGRAM 

int width = 160;
int heigth = 120;
int raydepth = 5;
int numverts = 0;
int numlights = 0;
int numobj = 0;
double fovy = 90.0;
double shininess = 0.0;
double ambient[3] = {0.2, 0.2, 0.2} ;
double diffuse[3] = {0.0, 0.0, 0.0};
double specular[3] = {0.0, 0.0, 0.0};
double emission[3] = {0.0, 0.0, 0.0};
double attenuation[3] = {1.0, 0.0, 0.0};

string output = "image.png";

vec3 eye(0.0, 0.0, 5.0);
vec3 up(0.0, 1.0, 0.0);
vec3 center(0.0, 0.0, 0.0);

double *vertices = NULL;
object *objects = NULL;
light *lights = NULL;

cube *grid = NULL;
int deeper = 0;

#else 

EXTERN int width, heigth, raydepth, numverts, numlights, numobj;

EXTERN double fovy, shininess;
EXTERN double ambient[3];
EXTERN double diffuse[3];
EXTERN double specular[3];
EXTERN double emission[3];
EXTERN double attenuation[3];

EXTERN string output;

EXTERN vec3 eye, up, center;

EXTERN double *vertices;
EXTERN object *objects;
EXTERN light *lights;

EXTERN cube *grid;
EXTERN int deeper;

#endif 
