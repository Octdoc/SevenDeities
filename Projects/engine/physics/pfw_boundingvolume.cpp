#include "pfw_boundingvolume.h"

namespace pfw
{

#pragma region Intersection

	bool Intersects_PointCuboid(mth::float3 p1, mth::float3 p2min, mth::float3 p2max)
	{
		return
			p2min.x < p1.x && p1.x < p2max.x &&
			p2min.y < p1.y && p1.y < p2max.y &&
			p2min.z < p1.z && p1.z < p2max.z;
	}
	bool Intersects_PointSphere(mth::float3 p1, mth::float3 p2, float r)
	{
		return (p1 - p2).LengthSquare() < r*r;
	}

	bool Intersects(BV_AABB* h1, BV_AABB* h2)
	{
		return Intersects_PointCuboid(h1->position, h2->position - h1->size, h2->position + h2->size);
	}
	bool Intersects(BV_AABB* h1, BV_Sphere* h2)
	{
		mth::float3 point;
		point.x = fmaxf(h1->position.x, fminf(h2->position.x, h1->position.x + h1->size.x));
		point.y = fmaxf(h1->position.y, fminf(h2->position.y, h1->position.y + h1->size.y));
		point.z = fmaxf(h1->position.z, fminf(h2->position.z, h1->position.z + h1->size.z));
		return Intersects_PointSphere(point, h2->position, h2->radius);
	}
	bool Intersects(BV_Sphere* h1, BV_Sphere* h2)
	{
		return Intersects_PointSphere(h1->position, h2->position, h1->radius + h2->radius);
	}

#pragma endregion


	BoundingVolume::BoundingVolume() {}
	BoundingVolume::BoundingVolume(mth::float3 pos) :position(pos) {}

	BV_AABB::BV_AABB() {}
	BV_AABB::BV_AABB(mth::float3 s) : size(s) {}
	BV_AABB::BV_AABB(mth::float3 pos, mth::float3 s) : BoundingVolume(pos), size(s) {}
	bool BV_AABB::Intersects(BoundingVolume* h)
	{
		return h->Intersects(this);
	}
	bool BV_AABB::Intersects(BV_AABB* h)
	{
		return pfw::Intersects(this, h);
	}
	bool BV_AABB::Intersects(BV_Sphere* h)
	{
		return pfw::Intersects(this, h);
	}

	BV_Sphere::BV_Sphere() :radius(0.0f) {}
	BV_Sphere::BV_Sphere(float r) : radius(r) {}
	BV_Sphere::BV_Sphere(mth::float3 pos, float r) : BoundingVolume(pos), radius(r) {}
	bool BV_Sphere::Intersects(BoundingVolume* h)
	{
		return h->Intersects(this);
	}
	bool BV_Sphere::Intersects(BV_AABB* h)
	{
		return pfw::Intersects(h, this);
	}
	bool BV_Sphere::Intersects(BV_Sphere* h)
	{
		return pfw::Intersects(this, h);
	}

}