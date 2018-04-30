#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../../Core/GameObject.h"
#include "../../Core/RenderComponent.h"
#include "../../Math/CollisionComponent.h"
#include "../../Math/PhysicsComponent.h"
#include "../../Math/PhysicsEngine.h"
#include "../../Graphics/Shader.h"
#include "../../Timers/Cooldown.h"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/compatibility.hpp>

namespace ENGINE {

	// Properties of the projectile that is saved and used during gameplay by the projectile manager
	enum PROJECTILE_ELEMENT { WIND, ICE, FIRE, EARTH, LIGHTNING, DEFAULT_ELEMENT };
	enum PROJECTILE_STRENGTH { LIGHT, MEDIUM, HEAVY, SPECIAL, DEFAULT_STRENGTH };
	enum PROJECTILE_CLIP { NO, YES_WALL_PROJECTILE, YES_WALL_PLAYER, YES_PLAYER_PROJECTILE, YES_PLAYER, YES_WALL, YES_PROJECTILE, YES };

	enum PROJECTILE_MESH {
		CUBE,
		NORM_FIRE, FIRE_METEOR,
		NORM_EARTH, EARTH_STALAGMITES, EARTH_ULT,
		NORM_ICE, ICE_RAMP, ICE_WALL, ICE_ULT,
		NORM_ELEC, ELEC_DISC,
		NORM_WIND, WIND_DISC, WIND_WALL
	};

	enum PROJECTILE_PLAYER { PLAYER1, PLAYER2, PLAYER3, PLAYER4, NONE };
	enum PROJECTILE_TEAM { TEAM1, TEAM2, TEAM0 };

	class Projectile : public GameObject {
	private:

		// Force used on the projectile at the beginning of motion
		glm::vec3 impulseForce;

		// Force acting on the projectile during motion
		glm::vec3 actingForce;

		// Force given to player when projectile makes contact
		glm::vec3 knockbackForce;

		// Angle to the target
		float angle;

		// Direction the player is facing
		int dir;

		// Delay before the projectile begins motion
		float delay;
		float delayTimer;

		// Second delay if desired for cool effect
		float delay2;
		float delayTimer2;

		// Index of the current delay in progress
		int delayIndex;

		// Stun time given to player when projectile makes contact
		float stunTime;

		// Damage given to player when projectile makes contact
		int damage;

		// Scale difference if desired when using delay
		glm::vec3 beginScale;
		glm::vec3 endScale;

		// Position difference if desired when using delay, if delay is not used the variables will do nothing
		glm::vec3 beginPosition;
		glm::vec3 endPosition;

		// Rotation differences if desired when using delay, if delay is not used the variables will do nothing
		glm::quat beginRotation;
		glm::quat endRotation;

		// Position difference if desired when using delay (based off previous delay endPosition)
		glm::vec3 endPosition2;

		// Scale difference if desired when using second delay (based off previous delay endScale)
		glm::vec3 endScale2;

		// Rotation difference if desired when using second delay (based off previous delay endScale)
		glm::quat endRotation2;

		// Enum that defines the element of the projectile
		PROJECTILE_ELEMENT element;

		// Enum that defines the strength of the projectile
		PROJECTILE_STRENGTH strength;

		// Enum that defines what type of clipping the projectile can do
		PROJECTILE_CLIP clipping;

		// Enum that defines the type of mesh attached to the projectile
		PROJECTILE_MESH mesh;

		// Enum that defines what team sent the projectile
		PROJECTILE_TEAM team;

		// Enum that defines what player sent the projectile
		PROJECTILE_PLAYER player;

		// Use the addMaxDistance function to adjust these. if you do, object will not go beyond a certain distance
		bool maximumDistance = false;
		float maxD;

		// Used for lifetime of the projectile
		Cooldown lifetime;

	public:

		// Default Constructor requires the start position, launch force, target angle, and direction
		Projectile(glm::vec3 p, float _angle, int _dir);
		~Projectile();

		// Sets the impulse force on the projectile when starting motion
		void SetImpulseForce(glm::vec3 _force);

		// Sets the acting force on the projectile during motion
		void SetActingForce(glm::vec3 _force);

		// Sets the knockback force applied to the player when hit
		void SetKnockbackForce(glm::vec3 _force);

		// Sets the delay before the projectile is set into motion, and its corresponding changes during that delay
		void SetFirstDelay(float _time, glm::vec3 _endPosition, glm::vec3 _beginScale, glm::vec3 _endScale, glm::quat _endRotation);

		// Sets the second delay before the projectile is set into motion, and its corresponding changes during that delay
		// Second delay is offset off the first delay, if no change in position, scale, or rotation desired, set time in seconds and
		// the rest of the parameters to 0
		void SetSecondDelay(float _time, glm::vec3 _endPosition, glm::vec3 _endScale, glm::quat _endRotation);

		// This method is used to stop all delays and launch the projectile into motion
		void StopDelay();

		// Sets the stun time applied to the player when hit
		void SetStunTime(float time);

		// Sets the damage applied to the player when hit
		void SetDamage(int d);

		// Sets the element of the projectile
		void SetElement(PROJECTILE_ELEMENT _element);

		// Sets the weight/strength of the projectile
		void SetStrength(PROJECTILE_STRENGTH _strength);

		// Sets the clip properties of the projectile
		void SetClipping(PROJECTILE_CLIP _clipping);

		// Sets the mesh type of the projectile
		void SetMesh(PROJECTILE_MESH _mesh);

		// Sets the team of the projectile
		void SetTeam(PROJECTILE_TEAM _team);

		// Sets the player who send the projectile
		void SetPlayer(PROJECTILE_PLAYER _player);

		// Adds a maximum distance that the projectile can go from its initial position 
		void AddMaxDistance(float distance);

		// Gets the lifetime of the projectile in seconds
		void SetLifetime(double seconds);

		// Gets the knockback force applied to the player when hit
		glm::vec3 GetForce();

		// Gets the stun time applied to the player when hit
		float GetStunTime();

		// Gets the damage applied to the player when hit
		int GetDamage();

		// Gets the element of the projectile
		PROJECTILE_ELEMENT GetElement();

		// Gets the weight/strength of the projectile
		PROJECTILE_STRENGTH GetStrength();

		// Gets the clip properties of the projectile
		PROJECTILE_CLIP GetClipping();

		// Gets the mesh type of the projectile
		PROJECTILE_MESH GetMesh();

		// Get the team of the projectile
		PROJECTILE_TEAM GetTeam();

		// Gets the player who send the projectile
		PROJECTILE_PLAYER GetPlayer();

		void Update(const float deltaTime);
		void FixedUpdate(const float deltaTime);
		void Render(Shader* shader, const double _interpolation);

		// Used to create the collision for the projectile
		void CreateCollision(RenderComponent* renderComponent, glm::vec3 p);

		// Enable this bool to create zig zag effects
		// Acting Force will flip in the x axis every flip interval 
		bool canFlipX;
		bool canFlipY;

		// The amount of time between every flip during zig cag effects
		// Measured in seconds
		float flipIntervalX;
		float flipIntervalY;
		float flipTimeX;
		float flipTimeY;
	};

}
#endif
