#pragma once

#include "form/form_form.h"

namespace gfw
{
	class Scene
	{
		SHARED_ONLY(Scene);

	protected:
		Scene() = default;
		form::Form::P m_window;
		Graphics::P m_graphics;

	public:
		void setWndGfx(form::Form::P m_window, Graphics::P m_graphics);

		virtual void Start() = 0;
		virtual void Quit() = 0;
		virtual void Render() = 0;
	};
}