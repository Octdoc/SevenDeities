#pragma once

#include "pfw_boundingvolume.h"
#include "graphics/resources/gfw_modelloader.h"
#include "helpers/hcs_input.h"

namespace pfw
{
	const float BIAS = 1e-3f;

	struct CollisionData
	{
		float collisionTime;
		mth::float3 normal;
	};

	struct Triangle
	{
		mth::float3 vertices[3];
		mth::float3 normal;
		float distance;
	};

	class Player :public mth::Position
	{
	protected:
		BV_AABB m_boundingBox;
		bool canJump;

	public:
		mth::float3 velocity;
		float speed;
		float jumpStrength;

	public:
		Player();
		BV_AABB* GetBoundingBox(float deltaTime);
		float Update(CollisionData& collData, float deltaTime, bool collision);
		void Control_FreeMove(hcs::Input& input);
		void Control_Gravity(hcs::Input& input);
	};

	class Collider
	{
		BV_AABB m_boundingBox;
		std::vector<Triangle> m_mesh;

	public:
		mth::float3 position;

	public:
		Collider();
		Collider(gfw::ModelLoader& modelLoader);
		Collider(gfw::ModelLoader& modelLoader, mth::float3 position);
		static std::shared_ptr<Collider> Create();
		static std::shared_ptr<Collider> Create(gfw::ModelLoader& modelLoader);
		static std::shared_ptr<Collider> Create(gfw::ModelLoader& modelLoader, mth::float3 position);
		void CreateCollider(gfw::ModelLoader& modelLoader);
		
		bool CollidesWithPlayer(std::shared_ptr<Player>& player, CollisionData& collData);
	};

	class CollisionArea
	{
		std::shared_ptr<Player> m_player;
		std::vector<std::shared_ptr<Collider>> m_colliders;

	public:
		float gravity;

	public:
		CollisionArea();

		void setPlayer(std::shared_ptr<Player> player);
		void AddCollider(std::shared_ptr<Collider> collider);
		void RemoveCollider(std::shared_ptr<Collider> collider);
		void RemoveAllColliders();

		void Update(float deltaTime);
	};
}