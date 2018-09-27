#pragma once

#include "graphics/gfw_window.h"
#include <forward_list>

namespace form
{
	class Form;

	class FormContainer
	{
		SHARED_ONLY(FormContainer);

	protected:
		std::forward_list<std::shared_ptr<Form>> m_children;

	protected:
		FormContainer() = default;

	public:
		virtual void AddChild(std::shared_ptr<Form> child) = 0;
		virtual void RemoveChild(std::shared_ptr<Form> child) = 0;
		virtual void RemoveAllChild() = 0;
	};

	class Form :public FormContainer
	{
		SHARED_ONLY(Form);
	protected:
		Form::W m_self;
		FormContainer::W m_parent;

		HWND m_hwnd;
		RECT m_boundingbox;

	public:
		std::wstring windowName;

	private:
		void ApplyWindowSize();

	protected:
		Form() = default;

	public:
		HWND getHWND();
		void setPosition(int x, int y);
		void setX(int x);
		void setY(int y);
		int getX();
		int getY();
		void setSize(int w, int h);
		void setW(int w);
		void setH(int h);
		int getW();
		int getH();
		void setRect(int x, int y, int w, int h);

		virtual void AddChild(Form::P child) override;
		virtual void RemoveChild(Form::P child)  override;
		virtual void RemoveAllChild()  override;
		bool CloseWindow(HWND hwnd);
		void Destroy();
		void setParent(FormContainer::W parent);
	};
}