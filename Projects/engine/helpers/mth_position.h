#pragma once

#include "mth_math.h"

namespace mth
{
	class Position
	{
	public:
		float3 position;
		float3 rotation;
		float3 scale;

	public:
		Position();

		void MoveForward(float d);
		void MoveBackward(float d);
		void MoveRight(float d);
		void MoveLeft(float d);
		void MoveUp(float d);
		void MoveDown(float d);

		void LookDown(float r);
		void LookUp(float r);
		void TurnRight(float r);
		void TurnLeft(float r);
		void RollRight(float r);
		void RollLeft(float r);

		void ScaleX(float s);
		void ScaleY(float s);
		void ScaleZ(float s);

		float4x4 GetPositionMatrix();
		float4x4 GetRotationMatrix();
		float4x4 GetScaleMatrix();
		float4x4 GetWorldMatrix();
		void Reset();
		float DistanceSquare(Position& other);
		float Distance(Position& other);
	};
}