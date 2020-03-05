
double intersectTriangle(const int i, double* rayorigin, double* rayvec);
double intersectSphere(const int i, double* rayorigin, double* rayvec);
double intersectObj(const int i, double* rayorigin, double* rayvec);
bool isVisible(double* lightdir, double* hitpoint, const double maxdist);
void shootRay(double* rayorigin, double* rayvec, const int depth, double* rayColor);
