#include "basic.h"

class Transform  
{
public:
	Transform();
	virtual ~Transform();
	static mat4 lookAt(const vec3& eye, const vec3 &center, const vec3& up);
	static mat4 lookAtInv(const vec3 &eye, const vec3 &center, const vec3 &up);
        static mat3 rotate(const double degrees, const vec3& axis) ;
        static mat4 scale(const double &sx, const double &sy, const double &sz) ; 
        static mat4 translate(const double &tx, const double &ty, const double &tz);
	static mat4 shear(const double &factor, const int &shearedAxis, const int &shearingAxis);
};
