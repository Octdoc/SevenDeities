#pragma once

#include "helpers/hcs_input.h"
#include "math/mth_position.h"

namespace octdoc
{
	namespace gfx
	{
		class Camera :public mth::Position
		{
			mth::float4x4 m_viewMatrix;
			mth::float4x4 m_projectionMatrix;
			float m_screenAspect;
			float m_fieldOfView;
			float m_screenDepth;
			float m_screenNear;

		public:
			Camera();

			void Init(float screenAspect = 16.0f / 9.0f, float fov = mth::pi / 4.0f, float screenNear = 0.1f, float screenDepth = 1000.0f);

			void SetFOV(float fov);
			float getFOV();
			void SetScreenAspect(float screenAspect);

			void Update();
			mth::float4x4 getViewMatrix();
			mth::float4x4 getProjectionMatrix();
			mth::float4x4 GetCameraMatrix();
		};
	}
}