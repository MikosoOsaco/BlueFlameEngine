#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <SDL/SDL.h>
#include <iostream>
#include <vector>
#include "GameObject.h"
#include "Light.h"
#include "../Camera/Camera.h"
#include "../Rendering/2D/UIObject.h"
#include "../Rendering/3D/Skybox.h"
#include "../InputHandling/InputManager.h"

namespace ENGINE {

	// Base Scene Class
	class Scene
	{
	public:
		// CONSTRUCTORS
		Scene() {};
		virtual ~Scene() {};
		//
		// REQUIRED FUNCTIONS
		virtual bool Initialize() = 0;
		virtual void Update(const float deltaTime) = 0;
		virtual void FixedUpdate(const float deltaTime) = 0;
		virtual void HandleEvents(SDL_Event events) = 0;
		virtual void HandleStates(const Uint8 *state) = 0;
		//
		// NOT REQUIRED FUNCTIONS
		virtual void PreUpdate(const float deltaTime)
		{
			this->deltaTime = deltaTime;

			if (objectList.size() != NULL) {
				for (size_t i = 0; i < objectList.size(); ++i) {
					objectList.at(i)->Update(deltaTime);
				}
			}

			if (uiObjectList.size() != NULL) {
				for (size_t i = 0; i < uiObjectList.size(); ++i) {
					uiObjectList.at(i)->Update(deltaTime);
				}
			}
		};
		virtual void LateUpdate(const float deltaTime)
		{

		};
		virtual void UpdateState() 
		{
			if (objectList.size() != NULL) {
				for (size_t i = 0; i < objectList.size(); ++i) {
					objectList.at(i)->UpdateState();
				}
			}
		}
		virtual void PreFixedUpdate(const float deltaTime)
		{

			this->fixedDeltaTime = deltaTime;

			if (objectList.size() != NULL) {
				for (size_t i = 0; i < objectList.size(); ++i) {
					objectList.at(i)->FixedUpdate(deltaTime);
				}
			}
		};
		virtual void LateFixedUpdate(const float deltaTime)
		{
			if (objectList.size() != NULL) {
				for (size_t i = 0; i < objectList.size(); ++i) {
					if (objectList.at(i)->deleted) {
						RemoveObject(objectList.at(i));
					}
				}
			}
		};
		// ADD LIST FUNCTIONS
		virtual void AddObject(GameObject* c) { objectList.push_back(c); };
		virtual void AddPhysicsObject(GameObject* c) { physicsObjectList.push_back(c); objectList.push_back(c); };
		virtual void AddLightObject(Light* c) 
		{ 
			if (c->lightComponent->GetLightType() == LightComponent::Light_Type::DIRECTIONAL) {
				dirLightList.push_back(c); objectList.push_back(c);
			}
			else if (c->lightComponent->GetLightType() == LightComponent::Light_Type::POINTLIGHT) {
				pointLightList.push_back(c); objectList.push_back(c);
			}
			else if (c->lightComponent->GetLightType() == LightComponent::Light_Type::SPOTLIGHT) {
				spotLightList.push_back(c); objectList.push_back(c);
			}
		};
		virtual void AddUIObject(UIObject* c) { uiObjectList.push_back(c); };
		//
		// REMOVE LIST FUNCTIONS
		virtual void RemoveObject(GameObject* c)
		{
			objectList.erase(std::remove(objectList.begin(), objectList.end(), c), objectList.end());
			if (c->physicsComponent != NULL) {
				physicsObjectList.erase(std::remove(physicsObjectList.begin(), physicsObjectList.end(), c), physicsObjectList.end());
			}
			delete c;
			c = nullptr;
		};
		virtual void RemoveObject(Light* c)
		{
			if (c->lightComponent->GetLightType() == LightComponent::Light_Type::DIRECTIONAL) {
				dirLightList.erase(std::remove(dirLightList.begin(), dirLightList.end(), c), dirLightList.end());
			}
			else if (c->lightComponent->GetLightType() == LightComponent::Light_Type::POINTLIGHT) {
				pointLightList.erase(std::remove(pointLightList.begin(), pointLightList.end(), c), pointLightList.end());
			}
			else if (c->lightComponent->GetLightType() == LightComponent::Light_Type::SPOTLIGHT) {
				spotLightList.erase(std::remove(spotLightList.begin(), spotLightList.end(), c), spotLightList.end());
			}
			delete c;
			c = nullptr;
		};
		// LIST GETTERS
		virtual std::vector<GameObject*> GetObjectList() { return objectList; };
		virtual std::vector<GameObject*> GetPhysicsObjectList() { return physicsObjectList; };
		virtual std::vector<Light*> GetDirLightList() { return dirLightList; };
		virtual std::vector<Light*> GetPointLightList() { return pointLightList; };
		virtual std::vector<Light*> GetSpotLightList() { return spotLightList; };
		virtual std::vector<UIObject*> GetUIObjectList() { return uiObjectList; };
		virtual std::vector<Camera*> GetCameraList() { return cameraList; };
		// SKYBOX GETTER
		virtual Skybox* GetSkybox() { return skybox; };

	protected:
		std::vector<GameObject*> objectList;
		std::vector<GameObject*> physicsObjectList;
		std::vector<Light*> dirLightList;
		std::vector<Light*> pointLightList;
		std::vector<Light*> spotLightList;
		std::vector<UIObject*> uiObjectList;
		std::vector<Camera*> cameraList = {
			new Camera(glm::vec3()),
			new Camera(glm::vec3())
		};
		Skybox* skybox;
		float deltaTime;
		float fixedDeltaTime;
		double interpolation;
	};
}
#endif