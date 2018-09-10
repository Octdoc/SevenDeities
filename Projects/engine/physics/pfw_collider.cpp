#include "pfw_collider.h"
#include <iostream>

namespace pfw
{

#pragma region Collider

	bool Collider::CollidesWithPlayer(std::shared_ptr<Player>& player, CollisionData& collData)
	{
		bool collision = false;
		BV_AABB boundingBox;
		boundingBox.size = m_boundingBox.size;
		boundingBox.position = m_boundingBox.position + position;
		if (!boundingBox.Intersects(player->GetBoundingBox(collData.collisionTime)))
			return false;
		mth::float3 playerPosition = player->position / player->scale;
		mth::float3 playerVelocity = player->velocity / player->scale;

		for (Triangle& tri : m_mesh)	//for each triangle
		{
			//transform triangle to eSpace
			Triangle t;
			t.vertices[0] = (tri.vertices[0] + position) / player->scale;
			t.vertices[1] = (tri.vertices[1] + position) / player->scale;
			t.vertices[2] = (tri.vertices[2] + position) / player->scale;
			t.normal = (tri.normal * player->scale).Normalized();
			t.distance = t.normal.Dot(t.vertices[0]);

			float startDist = t.normal.Dot(playerPosition) - t.distance;	//signed distance from triangle's plain
			float walkDist = t.normal.Dot(playerVelocity);	//signed distance taken away from trinagle's plain
			float t0 = fminf((1.0f - startDist) / walkDist, ((-1.0f - startDist) / walkDist));	//time to reach plain
			if (t0 < collData.collisionTime)	//if collision happens before the most recent detected
			{
				if (t0 > 0.0f)
				{
					mth::float3 vecs[3] = {
						t.vertices[1] - t.vertices[0],
						t.vertices[2] - t.vertices[0],
						playerPosition + playerVelocity * t0 - t.normal - t.vertices[0] };
					float dot00 = vecs[0].Dot(vecs[0]);
					float dot01 = vecs[0].Dot(vecs[1]);
					float dot02 = vecs[0].Dot(vecs[2]);
					float dot11 = vecs[1].Dot(vecs[1]);
					float dot12 = vecs[1].Dot(vecs[2]);
					float denom = dot00 * dot11 - dot01 * dot01;
					float u = (dot11 * dot02 - dot01 * dot12) / denom;
					float v = (dot00 * dot12 - dot01 * dot02) / denom;
					if (u >= 0.0f && v >= 0.0f && u + v <= 1.0f)	//collided with triangle face
					{
						collData.collisionTime = t0;
						collData.normal = t.normal;
						collision = true;
						continue;
					}
				}
				//collide with vertices
				for (int i = 0; i < 3; i++)
				{
					mth::float3 w = playerPosition - t.vertices[i];
					float a = playerVelocity.LengthSquare();
					float b = 2.0f * playerVelocity.Dot(w) / a;
					float c = (w.LengthSquare() - 1) / a;
					float d = b * b - 4 * c;
					if (d >= 0.0f)
					{
						float t0 = (-b - sqrtf(d))*0.5f;
						if (t0 > 0.0f && t0 < collData.collisionTime)	//collided with vertex
						{
							collData.collisionTime = t0;
							collData.normal = ((playerPosition + playerVelocity * t0) - t.vertices[i]);
							collision = true;
						}
					}
				}
				//collide with edges
				for (int i = 0; i < 3; i++)
				{
					mth::float3 p1 = t.vertices[i], p2 = t.vertices[(i + 1) % 3];
					mth::float3 edge = p2 - p1;
					mth::float3 baseToVertex = p1 - playerPosition;
					float edge_2 = edge.LengthSquare();
					float ev = edge.Dot(playerVelocity);
					float eb = edge.Dot(baseToVertex);

					float a = edge_2 * (-1.0f)*playerVelocity.LengthSquare() + ev * ev;
					float b = (edge_2 * 2.0f*playerVelocity.Dot(baseToVertex) - 2.0f*ev*eb) / a;
					float c = (edge_2*(1.0f - baseToVertex.LengthSquare()) + eb * eb) / a;
					float d = b * b - 4 * c;
					if (d >= 0.0f)
					{
						float t0 = (-b - sqrtf(d))*0.5f;
						if (t0 > 0.0f && t0 < collData.collisionTime)	//collided with edge line
						{
							float x = (ev*t0 - eb) / edge_2;
							if (x > 0.0f && x < 1.0f)	//collision point with line is on the edge
							{
								collData.normal = playerPosition - (p1 + edge * x);
								collData.collisionTime = t0;
								collision = true;
							}
						}
					}
				}
			}
		}
		return collision;
	}

	Collider::Collider(gfw::ModelLoader& modelLoader)
	{
		CreateCollider(modelLoader);
	}
	Collider::Collider(gfw::ModelLoader& modelLoader, mth::float3 position) :position(position)
	{
		CreateCollider(modelLoader);
	}
	Collider::P Collider::Create(gfw::ModelLoader& modelLoader)
	{
		return std::make_shared<Collider>(modelLoader);
	}
	Collider::P Collider::Create(gfw::ModelLoader& modelLoader, mth::float3 position)
	{
		return std::make_shared<Collider>(modelLoader, position);
	}
	void Collider::CreateCollider(gfw::ModelLoader& modelLoader)
	{
		if (modelLoader.getIndexCount() > 0)
		{
			m_mesh.resize(modelLoader.getIndexCount() / 3);
			UINT vertexSize = modelLoader.getVertexSizeInFloats();
			for (int i = 0; i < m_mesh.size(); i++)
			{
				m_mesh[i].vertices[0].x = modelLoader.getVertices()[modelLoader.getIndices()[i * 3 + 0] * vertexSize + 0];
				m_mesh[i].vertices[0].y = modelLoader.getVertices()[modelLoader.getIndices()[i * 3 + 0] * vertexSize + 1];
				m_mesh[i].vertices[0].z = modelLoader.getVertices()[modelLoader.getIndices()[i * 3 + 0] * vertexSize + 2];
				m_mesh[i].vertices[1].x = modelLoader.getVertices()[modelLoader.getIndices()[i * 3 + 1] * vertexSize + 0];
				m_mesh[i].vertices[1].y = modelLoader.getVertices()[modelLoader.getIndices()[i * 3 + 1] * vertexSize + 1];
				m_mesh[i].vertices[1].z = modelLoader.getVertices()[modelLoader.getIndices()[i * 3 + 1] * vertexSize + 2];
				m_mesh[i].vertices[2].x = modelLoader.getVertices()[modelLoader.getIndices()[i * 3 + 2] * vertexSize + 0];
				m_mesh[i].vertices[2].y = modelLoader.getVertices()[modelLoader.getIndices()[i * 3 + 2] * vertexSize + 1];
				m_mesh[i].vertices[2].z = modelLoader.getVertices()[modelLoader.getIndices()[i * 3 + 2] * vertexSize + 2];
				m_mesh[i].normal = (m_mesh[i].vertices[1] - m_mesh[i].vertices[2]).Cross(m_mesh[i].vertices[1] - m_mesh[i].vertices[0]).Normalized();
				m_mesh[i].distance = m_mesh[i].normal.Dot(m_mesh[i].vertices[0]);
			}
			mth::float3 maxpos(modelLoader.getVertices()[0], modelLoader.getVertices()[1], modelLoader.getVertices()[2]);
			mth::float3 minpos = maxpos;
			for (UINT i = 1; i < modelLoader.getVertexCount(); i++)
			{
				mth::float3 v(
					modelLoader.getVertices()[i * vertexSize + 0],
					modelLoader.getVertices()[i * vertexSize + 1],
					modelLoader.getVertices()[i * vertexSize + 2]);
				if (v.x < minpos.x)	minpos.x = v.x;
				if (v.y < minpos.y)	minpos.y = v.y;
				if (v.z < minpos.z)	minpos.z = v.z;
				if (v.x > maxpos.x)	maxpos.x = v.x;
				if (v.y > maxpos.y)	maxpos.y = v.y;
				if (v.z > maxpos.z)	maxpos.z = v.z;
			}
			m_boundingBox.position = minpos;
			m_boundingBox.size = maxpos - minpos;
		}
	}

#pragma endregion

#pragma region Player

	Player::Player() :speed(5), canJump(false), jumpStrength(5.0f) {}
	Player::P Player::Create()
	{
		return std::make_shared<Player>();
	}
	BV_AABB* Player::GetBoundingBox(float deltaTime)
	{
		m_boundingBox.position = position + velocity * deltaTime - scale;
		m_boundingBox.size = scale * 2;
		return &m_boundingBox;
	}
	float Player::Update(CollisionData& collData, float deltaTime, bool collision)
	{
		canJump = false;
		if (collision)
		{
			//normal to original space
			collData.normal /= scale;
			collData.normal.Normalize();
			//position += collData.normal * BIAS;	//push player back a little so it won't stuck
			position += velocity * (collData.collisionTime - BIAS);	//apply speed, but a little less
			//position += velocity * collData.collisionTime;	//apply speed, but a little less
			velocity -= collData.normal * velocity.Dot(collData.normal);	//adjust velocity
			deltaTime -= collData.collisionTime;
			if (collData.normal.y > 0.5f)
				canJump = true;
		}
		else
		{
			position += velocity * collData.collisionTime;	//apply speed
															//no collision happened, all done
			return 0.0f;
		}
		if (fabsf(velocity.x) < BIAS && fabsf(velocity.y) < BIAS && fabsf(velocity.z) < BIAS)
			return 0.0f;
		return deltaTime;
	}
	void Player::Control_FreeMove(hcs::Input& input)
	{
		float deltax = 0.0f, deltay = 0.0f, deltaz = 0.0f;
		if (input.isPressed(VK_UP))
			deltaz += speed;
		if (input.isPressed(VK_DOWN))
			deltaz -= speed;
		if (input.isPressed(VK_RIGHT))
			deltax += speed;
		if (input.isPressed(VK_LEFT))
			deltax -= speed;
		if (input.isPressed(VK_PRIOR))
			deltay += speed;
		if (input.isPressed(VK_NEXT))
			deltay -= speed;

		velocity.x = cosf(-rotation.y)*deltax - sinf(-rotation.y)*deltaz;
		velocity.y = deltay;
		velocity.z = sinf(-rotation.y)*deltax + cosf(-rotation.y)*deltaz;
	}
	void Player::Control_Gravity(hcs::Input& input)
	{
		float deltax = 0.0f, deltay = 0.0f, deltaz = 0.0f;
		if (input.isPressed(VK_UP))
			deltaz += speed;
		if (input.isPressed(VK_DOWN))
			deltaz -= speed;
		if (input.isPressed(VK_RIGHT))
			deltax += speed;
		if (input.isPressed(VK_LEFT))
			deltax -= speed;

		velocity.x = cosf(-rotation.y)*deltax - sinf(-rotation.y)*deltaz;
		velocity.z = sinf(-rotation.y)*deltax + cosf(-rotation.y)*deltaz;
		if (canJump && input.isPressed(VK_PRIOR))
			velocity.y += jumpStrength;
	}

#pragma endregion

#pragma region CollisionArea

	CollisionArea::CollisionArea() :m_player(NULL), gravity(9.81f) {}
	CollisionArea::P CollisionArea::Create()
	{
		return std::make_shared<CollisionArea>();
	}
	void CollisionArea::setPlayer(Player::P player)
	{
		m_player = player;
	}
	void CollisionArea::AddCollider(Collider::P collider)
	{
		m_colliders.push_back(collider);
	}
	void CollisionArea::RemoveCollider(Collider::P collider)
	{
		for (auto c = m_colliders.begin(); c != m_colliders.end(); c++)
		{
			if (*c == collider)
			{
				m_colliders.erase(c);
				return;
			}
		}
	}
	void CollisionArea::RemoveAllColliders()
	{
		m_colliders.clear();
	}
	void CollisionArea::Update(float deltaTime)
	{
		int counter = 0;
		CollisionData collData;
		bool collision;

		m_player->velocity.y -= gravity * deltaTime;

		while (deltaTime > BIAS)
		{
			collData.collisionTime = deltaTime;
			collision = false;

			for (auto& c : m_colliders)
				collision |= c->CollidesWithPlayer(m_player, collData);

			deltaTime = m_player->Update(collData, deltaTime, collision);

			if (counter++ == 5)	//probably stuck looping forever, let it be (this is just a number, could be else)
			{
				m_player->velocity = mth::float3();
				std::wcout << L"Stuck" << std::endl;
				return;
			}
		}
	}

#pragma endregion

}
