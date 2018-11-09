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
		for (auto c : m_children)
			if (c->MessageHandlerCaller(m_input.getMSG().hwnd, m_input.getMSG().message, m_input.getMSG().wParam, m_input.getMSG().lParam))
				return;
	}
	void Program::CloseWindow(HWND hwnd)
	{
		for (auto c : m_children)
			if (c->MessageHandlerCaller(hwnd, WM_DESTROY, (WPARAM)0, (LPARAM)0))
				return;
	}
	void Program::Frame(float deltaTime)
	{
		for (auto c : m_children)
			c->FrameCaller(deltaTime);
	}
	LRESULT Program::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (msg == WM_DESTROY)
			m_instance->CloseWindow(hwnd);
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
					MessageHandler();
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
					MessageHandler();
			}
		}
	}
	void Program::setAutoUpdate(bool autoUpdate)
	{
		m_autoUpdate = autoUpdate;
	}
}
