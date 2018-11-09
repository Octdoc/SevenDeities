#pragma once

#include "graphics/gfw_graphics.h"
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
		virtual std::shared_ptr<Form> ToForm() = 0;
		virtual void AddChild(std::shared_ptr<Form> child) = 0;
		virtual void RemoveChild(std::shared_ptr<Form> child) = 0;
		virtual void RemoveAllChild() = 0;
	};

	class Form :public FormContainer
	{
		SHARED_ONLY(Form);

	protected:
		static size_t m_autoIDdistributor;

		Form::W m_self;
		FormContainer::P m_parent;

		HWND m_hwnd;
		RECT m_boundingbox;
		std::wstring m_windowName;

	private:
		void ApplyWindowSize();

	protected:
		Form(FormContainer::P parent);

	public:
		virtual ~Form();
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
		void setText(const WCHAR text[]);
		const std::wstring& getText();

		virtual Form::P ToForm() override;
		virtual void AddChild(Form::P child) override;
		virtual void RemoveChild(Form::P child)  override;
		virtual void RemoveAllChild()  override;

		virtual void Close();
		bool MessageHandlerCaller(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		void FrameCaller(float deltaTime);
		virtual void MessageHandler(UINT msg, WPARAM wparam, LPARAM lparam);
		virtual void Frame(float deltaTime);
	};
}