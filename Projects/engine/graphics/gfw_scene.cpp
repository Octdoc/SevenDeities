#include "gfw_scene.h"

namespace gfw
{
	void Scene::setWndGfx(form::Form::P window, Graphics::P graphics)
	{
		m_window = window;
		m_graphics = graphics;
	}
}
