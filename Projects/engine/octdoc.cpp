#include "octdoc.h"
#include <iostream>

namespace octdoc
{
	Program::P Program::m_instance;

	Program::Program() :m_autoUpdate(false), m_input() {}
	void Program::MessageHandler()
	{
		TranslateMessage(&m_input.getMSG());
		DispatchMessage(&m_input.getMSG());
		m_input.HandleMessage();
		for (auto msgHandler : m_msgHandlers)
			msgHandler->HandleMessage(m_input);
	}
	void Program::Frame()
	{
		m_timer.Update();
		float delta = (float)m_timer.GetTimeDelta();
		float total = (float)m_timer.GetTimeTotal();
		for (auto au : m_autoUpdaters)
			au->Update(delta, total);
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
	void Program::SubscribeMessageHandler(hcs::MessageHandler::P msgHandler)
	{
		m_msgHandlers.push_front(msgHandler);
	}
	void Program::UnsubscribeMessageHandler(hcs::MessageHandler::P msgHandler)
	{
		m_msgHandlers.remove(msgHandler);
	}
	void Program::UnsubscribeAllMessageHandlers()
	{
		m_msgHandlers.clear();
	}
	void Program::SubscribeAutoUpdater(hcs::AutoUpdater::P autoUpdater)
	{
		m_autoUpdaters.push_front(autoUpdater);
	}
	void Program::UnsubscribeAutoUpdater(hcs::AutoUpdater::P autoUpdater)
	{
		m_autoUpdaters.remove(autoUpdater);
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
		m_autoUpdate = updatePeriodicly;
		m_timer.Reset();
		while (m_input.getMSG().message != WM_QUIT)
		{
			if (m_autoUpdate)
			{
				if (PeekMessage(&m_input.getMSG(), NULL, 0, 0, PM_REMOVE))
					MessageHandler();
				else
					Frame();
			}
			else
			{
				if (GetMessage(&m_input.getMSG(), NULL, 0, 0))
					MessageHandler();
			}
		}
	}
	void Program::setAutoUpdate(bool autoUpdate)
	{
		m_autoUpdate = autoUpdate;
	}
}

void RemoveScene(form::Scene::P scene)
{
	octdoc::Program::Instance().UnsubscribeMessageHandler(scene);
	octdoc::Program::Instance().UnsubscribeAutoUpdater(scene);
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
