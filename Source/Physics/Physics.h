#pragma once 

#include <vector>

#include "Common/Vector.h"

namespace quokka
{
  enum class PhysObjType
  {
    None, Rect, Sphere
  };

	struct PhysObject
	{
    virtual ~PhysObject() {};
		Vector Position;		
    PhysObjType Type = PhysObjType::None;
	};

  struct RectPhysObject : public PhysObject
  {
    RectPhysObject() { Type = PhysObjType::Rect; }
    Vector Size;
  };
  
  struct SpherePhysObject : public PhysObject
  {
    SpherePhysObject() { Type = PhysObjType::Sphere; }
    float Radius;
  };

	class QPhysics
	{
	public:

    void Tick();

		void MoveObject(PhysObject* Tested, Vector NewPosition)
		{
			Vector OldPosition = Tested->Position;
			Tested->Position = NewPosition;

      return;
			for(PhysObject* PhysObject : PhysObjects)
			{
				if(PhysObject != Tested)
				{
					if(IsIntersect(PhysObject, Tested))
					{
						Tested->Position = OldPosition;
						return;
					}
				}
			}
		}

		bool IsIntersect(PhysObject* A, PhysObject* B)
		{
      if (B->Type == PhysObjType::Rect) std::swap(A, B);

      if (A->Type == PhysObjType::Rect)
      {
        if (B->Type == PhysObjType::Rect)
          return IsRectIntersect(dynamic_cast<RectPhysObject*>(A), dynamic_cast<RectPhysObject*>(B));
        else
          return false; //TODO:
      }
      else
      {
        return IsSphereIntersect(dynamic_cast<SpherePhysObject*>(A), dynamic_cast<SpherePhysObject*>(B));
      }
		}

    bool IsRectIntersect(RectPhysObject* A, RectPhysObject* B)
    {
      struct FRect
      {
        float left;
        float top;
        float right;
        float bottom;
      };

      FRect rA = { A->Position.x - A->Size.x / 2, A->Position.y - A->Size.y / 2, A->Position.x + A->Size.x / 2, A->Position.y + A->Size.y / 2 };
      FRect rB = { B->Position.x - B->Size.x / 2, B->Position.y - B->Size.y / 2, B->Position.x + B->Size.x / 2, B->Position.y + B->Size.y / 2 };
      
      return (fabsf(rA.left - rB.left) + fabsf(rA.right - rB.right)) < A->Size.x + B->Size.x
        && (fabsf(rA.top - rB.top) + fabsf(rA.bottom - rB.bottom)) < A->Size.y + B->Size.y;
    }

    bool IsSphereIntersect(SpherePhysObject* A, SpherePhysObject* B)
    {
      return (A->Position - B->Position).Size() < (A->Radius + B->Radius);
    }

		RectPhysObject* CreateRectPhysObject(Vector Position, Vector Size)
		{
			RectPhysObject* po = new RectPhysObject();
			po->Position = Position;
			po->Size = Size;

			PhysObjects.push_back(po);

			return po;
		}

    SpherePhysObject* CreateSpherePhysObject(Vector Position, float Radius)
    {
      SpherePhysObject* po = new SpherePhysObject();
      po->Position = Position;
      po->Radius = Radius;

      PhysObjects.push_back(po);

      return po;
    }

    void DeletePhysObject(PhysObject* PO)
    {
      PhysObjects.erase(std::find(PhysObjects.begin(), PhysObjects.end(), PO));
    }
	private:
		std::vector<PhysObject*> PhysObjects;
	};
}