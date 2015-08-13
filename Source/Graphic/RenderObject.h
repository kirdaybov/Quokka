#pragma once

#include "Common/Vector.h"
#include "Common/Color.h"

namespace quokka
{
	struct RenderObject
	{
		Vector Position;
    QRotation Rotation;
		Vector Size;
		Color Color;
	};
}