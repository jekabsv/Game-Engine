#pragma once
#include "Tools.hpp"



namespace game
{
	class EntitiesUpdate
	{
	public:
		void UpdateVillager(mesh& Villager, vec3d &vcamera, std::vector <mesh> &OtherMeshes);
	};
	
}