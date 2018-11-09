#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

namespace octdoc
{
	namespace hcs
	{
		class Exception
		{
			HRESULT m_result;
			std::wstring m_caption;
			std::wstring m_description;

		public:
			Exception();
			Exception(const WCHAR* caption, const WCHAR* description);
			Exception(const std::wstring& caption, const std::wstring& description);
			Exception(const WCHAR* description, HRESULT result);
			Exception(const std::wstring& description, HRESULT result);
			static Exception FileLoadingException(const WCHAR* filename);
			static Exception FileLoadingException(const std::wstring& filename);
			static Exception OutOfMemoryException();

			static const std::wstring HResultToString(HRESULT result);

			std::wstring& Caption();
			std::wstring& Description();
			HRESULT HResult();

			void ShowMessageBox();
		};
	}
}