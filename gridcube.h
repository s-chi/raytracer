void createGrid(void);
void deleteGrid(cube* Cube);
void appendObj(cube *curCube, const int objindex);
cube* appendSubCube(cube *curCube, int* subcindex);
void setGridObj(const int objindex, cube* curCube);
double intersectGrid(double* rayorigin, double* rayvec, cube* curCube, const double curDist, int* hit);
