#include "gfx_light.h"

namespace octdoc
{
	namespace gfx
	{
		void Light::UpdateNonCubic()
		{
			mth::float3 up(0.0f, 1.0f, 0.0f), lookAt(0.0f, 0.0f, 1.0f);
			mth::float4x4 rotationMatrix;

			rotationMatrix = mth::float4x4::Rotation(rotation);

			lookAt = rotationMatrix * lookAt;
			up = rotationMatrix * up;

			lookAt += position;

			m_viewMatrices[0] = mth::float4x4::LookAt(position, lookAt, up);
		}
		void Light::UpdateCubic()
		{
			mth::float3 lookAt, up;

			lookAt = mth::float3(1.0f, 0.0f, 0.0f) + position;
			up = mth::float3(0.0f, 1.0f, 0.0f);
			m_viewMatrices[0] = mth::float4x4::LookAt(position, lookAt, up);

			lookAt = mth::float3(-1.0f, 0.0f, 0.0) + position;
			up = mth::float3(0.0f, 1.0f, 0.0f);
			m_viewMatrices[1] = mth::float4x4::LookAt(position, lookAt, up);

			lookAt = mth::float3(0.0f, 1.0f, 0.0f) + position;
			up = mth::float3(0.0f, 0.0f, -1.0f);
			m_viewMatrices[2] = mth::float4x4::LookAt(position, lookAt, up);

			lookAt = mth::float3(0.0f, -1.0f, 0.0f) + position;
			up = mth::float3(0.0f, 0.0f, 1.0f);
			m_viewMatrices[3] = mth::float4x4::LookAt(position, lookAt, up);

			lookAt = mth::float3(0.0f, 0.0f, 1.0f) + position;
			up = mth::float3(0.0f, 1.0f, 0.0f);
			m_viewMatrices[4] = mth::float4x4::LookAt(position, lookAt, up);

			lookAt = mth::float3(0.0f, 0.0f, -1.0f) + position;
			up = mth::float3(0.0f, 1.0f, 0.0f);
			m_viewMatrices[5] = mth::float4x4::LookAt(position, lookAt, up);
		}
		void Light::Init(bool perspective, float size, float screenNear, float screenDepth)
		{
			m_cube = false;
			m_perspective = perspective;
			m_size = size;
			m_screenNear = screenNear;
			m_screenDepth = screenDepth;
			if (m_perspective)
				m_projectionMatrix = mth::float4x4::PerspectiveFOV(m_size, 1.0f, m_screenNear, m_screenDepth);
			else
				m_projectionMatrix = mth::float4x4::Orthographic(m_size, m_size, m_screenNear, m_screenDepth);
		}
		void Light::InitCube(float screenNear, float screenDepth)
		{
			m_cube = true;
			m_perspective = true;
			m_size = mth::pi / 2.0f;
			m_screenNear = screenNear;
			m_screenDepth = screenDepth;
			m_projectionMatrix = mth::float4x4::PerspectiveFOV(m_size, 1.0f, m_screenNear, m_screenDepth);
		}
		void Light::Update()
		{
			if (m_cube)
				UpdateCubic();
			else
				UpdateNonCubic();
		}
		void Light::SetSize(float size)
		{
			m_size = size;
			if (m_perspective)
				m_projectionMatrix = mth::float4x4::PerspectiveFOV(m_size, 1.0f, m_screenNear, m_screenDepth);
			else
				m_projectionMatrix = mth::float4x4::Orthographic(m_size, m_size, m_screenNear, m_screenDepth);
		}
		float Light::getSize()
		{
			return m_size;
		}
		mth::float4x4 Light::getViewMatrix(unsigned int index)
		{
			return m_viewMatrices[index];
		}
		mth::float4x4 Light::getProjectionMatrix()
		{
			return m_projectionMatrix;
		}
		mth::float4x4 Light::GetLightMatrix(unsigned int index)
		{
			return m_projectionMatrix * m_viewMatrices[index];
		}
	}
}