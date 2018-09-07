#pragma once

#include "helpers/mth_position.h"

namespace pfw
{
	class BoundingVolume;
	class BV_AABB;
	class BV_Sphere;

	class BoundingVolume
	{
	public:
		mth::float3 position;

	public:
		BoundingVolume();
		BoundingVolume(mth::float3 position);
		virtual bool Intersects(BoundingVolume* h) = 0;
		virtual bool Intersects(BV_AABB* h) = 0;
		virtual bool Intersects(BV_Sphere* h) = 0;
	};

	class BV_AABB :public BoundingVolume
	{
	public:
		mth::float3 size;

	public:
		BV_AABB();
		BV_AABB(mth::float3 size);
		BV_AABB(mth::float3 position, mth::float3 size);

		virtual bool Intersects(BoundingVolume* h) override;
		virtual bool Intersects(BV_AABB* h) override;
		virtual bool Intersects(BV_Sphere* h) override;
	};

	class BV_Sphere :public BoundingVolume
	{
	public:
		float radius;

	public:
		BV_Sphere();
		BV_Sphere(float r);
		BV_Sphere(mth::float3 position, float r);

		virtual bool Intersects(BoundingVolume* h) override;
		virtual bool Intersects(BV_AABB* h) override;
		virtual bool Intersects(BV_Sphere* h) override;
	};
}