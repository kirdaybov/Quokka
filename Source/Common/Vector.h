#pragma once

#include <math.h>

namespace quokka
{
  const float M_PI = 3.1415f;

	struct Vector
	{
		Vector() : x(0.f), y(0.f), z(0.f) {};
		Vector(float Value) : x(Value), y(Value), z(Value) {};
		Vector(float a_x, float a_y, float a_z = 0.f) : x(a_x), y(a_y), z(a_z) {};

		float x, y, z;

		float Size()
		{
			return sqrtf(x*x + y*y + z*z);
		}

		Vector& Normalize() 
		{ 
			float size = Size();
			if (size == 0.f) return (*this);
			x /= size;
			y /= size;
			z /= size;
			return (*this);
		}
	};

	Vector operator+ (Vector& A, Vector& B);
  Vector operator- (Vector& A, Vector& B);
	Vector operator* (Vector& A, float v);
  Vector operator/ (Vector& A, float v);

  struct QRotation : public Vector
  {
    QRotation() { x = 0.f; }
    QRotation(Vector& v) { x = v.x; y = v.y; z = v.z; }

    static QRotation FromDirection(Vector v)
    {
      QRotation r;
      r.x = atan2(v.y, v.x);
      r.Clamp();
      return r;
    }

    void Clamp()
    {
      Clamp(x);
      Clamp(y);
      Clamp(z);
    }

    Vector RotateVector(Vector v)
    {
      Vector res;
      res.x = v.x*cosf(x) - v.y*sinf(x);
      res.y = v.x*sinf(x) + v.y*cosf(x);
      return res;
    }

  private:
    void Clamp(float &v) { while (v > 2 * M_PI) v -= 2 * M_PI; while (v < 0) v += 2 * M_PI; }
  };
}
