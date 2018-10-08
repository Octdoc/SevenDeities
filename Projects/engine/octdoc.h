#pragma once

#include "helpers/hcs_timer.h"
#include "graphics/entity/gfw_controller.h"
#include "graphics/renderers/gfw_simplerenderer.h"
#include "graphics/renderers/gfw_simpleshadowrenderer.h"
#include "graphics/renderers/gfw_colorrenderer.h"
#include "graphics/renderers/gfw_colorshadowrenderer.h"

#include "physics/pfw_collider.h"

#include "form/form_button.h"
#include "form/form_checkbox.h"
#include <forward_list>

namespace octdoc
{
	class Program :public form::FormContainer
	{
		SHARED_ONLY(Program);
		static Program::P m_instance;

		hcs::Timer m_timer;
		hcs::Input m_input;
		bool m_autoUpdate;

		std::forward_list<hcs::MessageHandler::P> m_msgHandlers;
		std::forward_list<hcs::AutoUpdater::P> m_autoUpdaters;

	private:
		Program();

		void MessageHandler();
		void Frame();

	public:
		inline static void Initialize() { m_instance = std::make_shared<Program>(); }
		inline static Program& Instance() { return *m_instance; }
		inline hcs::Input& Input() { return m_input; }

		void RegisterWindow(form::Window::P window);
		void UnregisterWindow(form::Window::P window);
		void CloseAllWindows();
		void SubscribeMessageHandler(hcs::MessageHandler::P msgHandler);
		void UnsubscribeMessageHandler(hcs::MessageHandler::P msgHandler);
		void UnsubscribeAllMessageHandlers();
		void SubscribeAutoUpdater(hcs::AutoUpdater::P autoUpdater);
		void UnsubscribeAutoUpdater(hcs::AutoUpdater::P autoUpdater);
		void UnsubscribeAllAutoUpdaters();

		virtual void AddChild(form::Form::P child) override;
		virtual void RemoveChild(form::Form::P child)  override;
		virtual void RemoveAllChild()  override;
		void CloseWindow(HWND hwnd);

		void ClearAll();

		void Run(bool updatePeriodicly = false);
		void setAutoUpdate(bool autoUpdate);
	};

}