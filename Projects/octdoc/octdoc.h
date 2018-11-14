#pragma once

#include "graphics/entity/gfx_controller.h"
#include "graphics/renderers/gfx_simplerenderer.h"
#include "graphics/renderers/gfx_simpleshadowrenderer.h"
#include "graphics/renderers/gfx_colorrenderer.h"
#include "graphics/renderers/gfx_colorshadowrenderer.h"

#include "physics/physx_collider.h"

#include "form/form_textbox.h"
#include "form/form_label.h"
#include "form/form_button.h"
#include "form/form_checkbox.h"
#include "form/form_listbox.h"
#include <forward_list>
#include <chrono>

namespace octdoc
{
	class Program :public form::FormContainer
	{
		SHARED_ONLY(Program)

		static Program::P m_instance;

		hcs::Input m_input;
		bool m_autoUpdate;

	private:
		Program();
		void MessageDispatcher();
		void MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		void Frame(float deltaTime);

	public:
		inline static void Initialize() { m_instance = std::make_shared<Program>(); }
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		inline static Program& Instance() { return *m_instance; }
		inline static Program::P InstancePtr() { return m_instance; }
		inline hcs::Input& Input() { return m_input; }

		void RegisterWindow(form::Window::P window);
		void UnregisterWindow(form::Window::P window);
		void CloseAllWindows();
		virtual form::Form::P ToForm() override;
		virtual void AddChild(form::Form::P child) override;
		virtual void RemoveChild(form::Form::P child)  override;
		virtual void RemoveAllChild()  override;

		void Run(bool updatePeriodicly = false);
		void setAutoUpdate(bool autoUpdate);
	};

}