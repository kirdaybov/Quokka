#pragma once 

namespace quokka
{
	struct Color
	{
		Color(float a_r = 0.f, float a_g = 0.f, float a_b = 0.f, float a_a = 1.f) : 
			r(a_r), 
			g(a_g), 
			b(a_b), 
			a(a_a)
		{}

		float r, g, b, a;
	};
}