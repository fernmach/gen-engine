// #include "physics.h"

// bool AABBvsAABB( AABBColliderShape a, AABBColliderShape b )
// {
//   // Exit with no intersection if found separated along an axis 
//   if(a.max.x < b.min.x || a.min.x > b.max.x) return false;
//   if(a.max.y < b.min.y || a.min.y > b.max.y) return false;
//   // No separating axis found, therefor there is at least one overlapping axis 
//   return true;
// }

// bool CirclevsCircle( CircleColliderShape a, CircleColliderShape b )
// {
//   u16 r = a.radius + b.radius;
//   r *= r;
//   return r < ((a.position.x + b.position.x)^2) + ((a.position.y + b.position.y)^2);
// }