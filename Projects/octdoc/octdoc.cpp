#include "octdoc.h"
#include <iostream>

namespace octdoc
{
	Program::P Program::m_instance;

	Program::Program() :m_autoUpdate(false), m_input() {}
	void Program::MessageDispatcher()
	{
		TranslateMessage(&m_input.getMSG());
		DispatchMessage(&m_input.getMSG());
		m_input.HandleMessage();
	}
	void Program::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		for (auto c : m_children)
			if (c->MessageHandlerCaller(hwnd, msg, wparam, lparam))
				return;
	}
	void Program::Frame(float deltaTime)
	{
		for (auto c : m_children)
			c->FrameCaller(deltaTime);
	}
	LRESULT Program::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		m_instance->MessageHandler(hwnd, msg, wparam, lparam);
		return DefWindowProc(hwnd, msg, wparam, lparam);
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
	form::Form::P Program::ToForm()
	{
		return nullptr;
	}
	void Program::AddChild(form::Form::P child)
	{
		m_children.push_front(child);
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
	void Program::Run(bool updatePeriodicly)
	{
		m_autoUpdate = updatePeriodicly;
		auto prevTime = std::chrono::steady_clock::now();
		while (m_input.getMSG().message != WM_QUIT)
		{
			if (m_autoUpdate)
			{
				if (PeekMessage(&m_input.getMSG(), NULL, 0, 0, PM_REMOVE))
					MessageDispatcher();
				else
				{
					auto currentTime = std::chrono::steady_clock::now();
					Frame(std::chrono::duration<float>(currentTime - prevTime).count());
					prevTime = currentTime;
				}
			}
			else
			{
				if (GetMessage(&m_input.getMSG(), NULL, 0, 0))
					MessageDispatcher();
			}
		}
	}
	void Program::setAutoUpdate(bool autoUpdate)
	{
		m_autoUpdate = autoUpdate;
	}
}
