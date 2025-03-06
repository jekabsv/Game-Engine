#include "EntitiesUpdate.hpp"
#include "Game.hpp"


namespace game
{
	void EntitiesUpdate::UpdateVillager(mesh& Villager, vec3d& vCamera, std::vector<mesh>& OtherMeshes)
	{
		vec3d vectortoCameraFromVillager = { Villager.ObjPos.x - vCamera.x, Villager.ObjPos.y - vCamera.y, Villager.ObjPos.z - vCamera.z };
		
		//vectortoCameraFromVillager = _data.tools.vectorArithmetic.normalizeVector(vectortoCameraFromVillager);
		Villager.ObjPos.x -= vectortoCameraFromVillager.x / 1.0f;
		Villager.ObjPos.z -= vectortoCameraFromVillager.z / 1.0f;
		Villager.Rotation.y += 0.01f;
		Villager.Rotation.x = -PI / 2;
		float tfYaw;
		float tfPitch;
		//_data->tools.utility.LookAtCamera(Villager.ObjPos, vCamera, tfYaw, tfPitch);
	}
}