#include "Transform.h"

mat3 Transform::rotate(const double degrees, const vec3& axis) 
{
  
  mat3 m = mat3(1.0);
  const vec3 naxis = glm::normalize(axis);
  const mat3 axismat = mat3(0.0, naxis[2], -naxis[1], -naxis[2], 0.0, naxis[0], naxis[1], -naxis[0], 0.0);
  const double rad = degrees * PI/180.0;
  
  m += sin(rad) * axismat + (1.0 - cos(rad)) * axismat * axismat;
  
  
  return m;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
  
  const vec3 w = glm::normalize(eye - center);
  const vec3 u = glm::normalize(glm::cross(up, w));
  const vec3 v = glm::cross(w,u);  
  
  const mat4 m = mat4(u.x, v.x, w.x, 0.0, 
		      u.y, v.y, w.y, 0.0, 
		      u.z, v.z, w.z, 0.0, 
		      -glm::dot(u, eye), -glm::dot(v, eye), -glm::dot(w, eye), 1.0);
  return m;
}

mat4 Transform::lookAtInv(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
  
  const vec3 w = glm::normalize(eye - center);
  const vec3 u = glm::normalize(glm::cross(up, w));
  const vec3 v = glm::cross(w,u);  
  
  const mat4 m = mat4(u.x, u.y, u.z, 0.0,
		      v.x, v.y, v.z, 0.0,
		      w.x, w.y, w.z, 0.0, 
		      eye.x, eye.y, eye.z, 1.0);
  return m;
}

mat4 Transform::scale(const double &sx, const double &sy, const double &sz) 
{
  const mat4 m = mat4(  sx, 0.0, 0.0, 0.0,
		       0.0,  sy, 0.0, 0.0,
		       0.0, 0.0,  sz, 0.0,
		       0.0, 0.0, 0.0, 1.0 );

  return m;
}

mat4 Transform::translate(const double &tx, const double &ty, const double &tz) 
{
  const mat4 m = mat4( 1.0, 0.0, 0.0, 0.0,
		       0.0, 1.0, 0.0, 0.0,
		       0.0, 0.0, 1.0, 0.0,
		        tx,  ty,  tz, 1.0 );  

  return m;
}

mat4 Transform::shear(const double &factor, const int &shearedAxis, const int &shearingAxis) 
{
  mat4 m = mat4(1.0);
  m[shearingAxis][shearedAxis] += factor;  

  return m;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
