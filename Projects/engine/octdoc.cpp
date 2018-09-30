#include "octdoc.h"
#include <iostream>

namespace octdoc
{
	Program::P Program::m_instance;

	Program::Program() :m_autocUpdate(false), m_input() {}
	void Program::MessageHandler(MSG& msg)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		m_input.HandleMessage(msg);
		for (hcs::MessageHandler::W msgHandler : m_msgHandlers)
			msgHandler.lock()->HandleMessage(m_input);
	}
	void Program::Frame()
	{
		m_timer.Update();
		float delta = (float)m_timer.GetTimeDelta();
		float total = (float)m_timer.GetTimeTotal();
		for (auto au : m_autoUpdaters)
			au.lock()->Update(delta, total);
	}
	void Program::RegisterWindow(form::Window::P window)
	{
		AddChild(window);
	}
	void Program::UnregisterWindow(form::Window::P window)
	{
		RemoveChild(window);
	}
	void Program::CloseAllWindows()
	{
		RemoveAllChild();
	}
	void Program::SubscribeMessageHandler(hcs::MessageHandler::W msgHandler)
	{
		m_msgHandlers.push_front(msgHandler);
	}
	void Program::UnsubscribeMessageHandler(hcs::MessageHandler::W msgHandler)
	{
		m_msgHandlers.remove_if([msgHandler](hcs::MessageHandler::W msgh)->bool {return msgHandler.lock() == msgh.lock(); });
	}
	void Program::UnsubscribeAllMessageHandlers()
	{
		m_msgHandlers.clear();
	}
	void Program::SubscribeAutoUpdater(hcs::AutoUpdater::W autoUpdater)
	{
		m_autoUpdaters.push_front(autoUpdater);
	}
	void Program::UnsubscribeAutoUpdater(hcs::AutoUpdater::W autoUpdater)
	{
		m_autoUpdaters.remove_if([autoUpdater](hcs::AutoUpdater::W au)->bool {return autoUpdater.lock() == au.lock(); });
	}
	void Program::UnsubscribeAllAutoUpdaters()
	{
		m_autoUpdaters.clear();
	}
	void Program::AddChild(form::Form::P child)
	{
		m_children.push_front(child);
		child->setParent(m_instance);
	}
	void Program::RemoveChild(form::Form::P child)
	{
		m_children.remove(child);
		if (m_children.empty())
			PostQuitMessage(0);
	}
	void Program::RemoveAllChild()
	{
		m_children.clear();
		PostQuitMessage(0);
	}
	void Program::CloseWindow(HWND hwnd)
	{
		for (auto child : m_children)
			if (child->CloseWindow(hwnd))
				break;
	}
	void Program::ClearAll()
	{
		RemoveAllChild();
		UnsubscribeAllMessageHandlers();
		UnsubscribeAllAutoUpdaters();
	}
	void Program::Run(bool updatePeriodicly)
	{
		MSG msg{};
		m_autocUpdate = updatePeriodicly;
		m_timer.Reset();
		while (msg.message != WM_QUIT)
		{
			if (m_autocUpdate)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
					MessageHandler(msg);
				else
					Frame();
			}
			else
			{
				if (GetMessage(&msg, NULL, 0, 0))
					MessageHandler(msg);
			}
		}
	}
	void Program::setAutoUpdate(bool autoUpdate)
	{
		m_autocUpdate = autoUpdate;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_DESTROY)
	{
		octdoc::Program::Instance().CloseWindow(hwnd);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
