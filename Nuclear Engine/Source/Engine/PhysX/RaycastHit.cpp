#include <Engine\PhysX\RaycastHit.h>
namespace NuclearEngine
{
	namespace PhysX
	{
		RaycastHit::RaycastHit()
			: valid(false),	 position(Math::Vector3()) , normal(Math::Vector3()) , distance(0)
		{

		}
		RaycastHit::~RaycastHit()
		{
		}
	}
}