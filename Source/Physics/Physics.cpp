#include "Game/Game.h"
#include "PhysicsComponent.h"

namespace quokka
{

  void Physics::Tick()
  {    
    for (int i = 0; i < PhysObjects.size(); i++)
    {      
      if (PhysObjects[i]->Type != PhysObjType::Sphere) continue;
      SpherePhysObject* Ph1 = dynamic_cast<SpherePhysObject*>(PhysObjects[i]);      
      for (int j = i + 1; j < PhysObjects.size(); j++)
      {
        if (PhysObjects[j]->Type != PhysObjType::Sphere) continue;
        SpherePhysObject* Ph2 = dynamic_cast<SpherePhysObject*>(PhysObjects[j]);        
        if (GGame()->GetPhysics()->IsIntersect(Ph1, Ph2))
        {
          Vector PDif = Ph1->Position - Ph2->Position;
          float Disp = Ph1->Radius + Ph2->Radius - PDif.Size();
          
          {             
            float PDifX = Disp * abs(PDif.x) / PDif.Size() / 2;
            Ph1->Position.x += PDif.x > 0 ? PDifX : -PDifX;
            Ph2->Position.x -= PDif.x > 0 ? PDifX : -PDifX;
          }          
          {
            float PDifY = Disp * abs(PDif.y) / PDif.Size() / 2;
            Ph1->Position.y += PDif.y > 0 ? PDifY : -PDifY;
            Ph2->Position.y -= PDif.y > 0 ? PDifY : -PDifY;
          }
        }
      }
    }
  }

}