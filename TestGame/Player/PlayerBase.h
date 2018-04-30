#pragma once
#ifndef PLAYERBASE_H
#define PLAYERBASE_H

#include <BlueFlameEngine\Engine\Core\GameObject.h>
#include <BlueFlameEngine\Engine\Core\RenderComponent.h>
#include <BlueFlameEngine\Engine\Rendering\3D\Model.h>

using namespace ENGINE;

namespace GAME {

	class PlayerBase : public GameObject
	{
	public:
		PlayerBase();
		PlayerBase(std::string path);
		~PlayerBase();

		void Update(const float deltaTime);
		void Render(Shader* shader, const double _interpolation);

		RenderComponent* renderComponent;
		Model* model;
	};
}

#endif

