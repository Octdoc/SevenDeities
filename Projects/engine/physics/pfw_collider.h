#pragma once

#include "math/mth_geometry.h"
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

	class Player :public mth::Position
	{
		SHARED_ONLY(Player);
	protected:
		BV_AABB m_boundingBox;
		bool canJump;

	public:
		mth::float3 velocity;
		float speed;
		float jumpStrength;

	private:
		Player();

	public:
		static Player::P Create();

		BV_AABB* GetBoundingBox(float deltaTime);
		float Update(CollisionData& collData, float deltaTime, bool collision);
		void Control_FreeMove(hcs::Input& input);
		void Control_Gravity(hcs::Input& input);
	};

	class Collider:public mth::Position
	{
		SHARED_ONLY(Collider);
		BV_AABB m_boundingBox;
		std::vector<mth::Triangle> m_mesh;

	private:
		Collider(gfw::ModelLoader& modelLoader);
		Collider(gfw::ModelLoader& modelLoader, mth::float3 position);

	public:
		static Collider::P Create(gfw::ModelLoader& modelLoader);
		static Collider::P Create(gfw::ModelLoader& modelLoader, mth::float3 position);
		void CreateCollider(gfw::ModelLoader& modelLoader);
		
		bool CollidesWithPlayer(std::shared_ptr<Player>& player, CollisionData& collData);
		std::vector<mth::Triangle>& getMesh();
	};

	class CollisionArea
	{
		SHARED_ONLY(CollisionArea);
		Player::P m_player;
		std::vector<Collider::P> m_colliders;

	public:
		float gravity;

	private:
		CollisionArea();

	public:
		static CollisionArea::P Create();

		void setPlayer(Player::P player);
		void AddCollider(Collider::P collider);
		void RemoveCollider(Collider::P collider);
		void RemoveAllColliders();
		std::vector<Collider::P>& getColliders();

		void Update(float deltaTime);
	};
}