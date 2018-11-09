#pragma once

#include "math/mth_position.h"

namespace octdoc
{
	namespace gfx
	{
		class Light :public mth::Position
		{
			mth::float4x4 m_viewMatrices[6];
			mth::float4x4 m_projectionMatrix;
			float m_size;	//fov (perspective), size(ortho)
			float m_screenDepth;
			float m_screenNear;
			bool m_perspective;
			bool m_cube;

		private:
			void UpdateNonCubic();
			void UpdateCubic();

		public:
			void Init(bool perspective, float size, float screenNear = 0.1f, float screenDepth = 1000.0f);
			void InitCube(float screenNear = 0.1f, float screenDepth = 1000.0f);
			void Update();
			void SetSize(float size);
			float getSize();
			mth::float4x4 getViewMatrix(unsigned int index = 0);
			mth::float4x4 getProjectionMatrix();
			mth::float4x4 GetLightMatrix(unsigned int index = 0);
		};
	}
}