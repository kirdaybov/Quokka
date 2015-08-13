#include "Vector.h"

namespace quokka
{
	Vector operator+ (Vector& A, Vector& B)
	{
		return Vector(A.x + B.x, A.y + B.y, A.z + B.z);
	}

  Vector operator- (Vector& A, Vector& B)
  {
    return Vector(A.x - B.x, A.y - B.y, A.z - B.z);
  }
	
	Vector operator* (Vector& A, float v)
	{
		return Vector(A.x*v, A.y*v, A.z*v);
	}

  Vector operator/ (Vector& A, float v)
  {
    return Vector(A.x/v, A.y/v, A.z/v);
  }
}