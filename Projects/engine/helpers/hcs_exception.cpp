#include "hcs_exception.h"

namespace hcs
{
	Exception::Exception()
		: m_result(S_OK), m_caption(L"Unspecified Failure"), m_description(L"An unspecified error occured.") {}

	Exception::Exception(const WCHAR* caption, const WCHAR* description)
		: m_result(S_OK), m_caption(caption), m_description(description) {}

	Exception::Exception(const std::wstring& caption, const std::wstring& description)
		: m_result(S_OK), m_caption(caption), m_description(description) {}

	Exception::Exception(const WCHAR* description, HRESULT result)
		: m_result(result), m_caption(L"DirectX Error"), m_description(description + std::wstring(L": ") + HResultToString(result)) {}

	Exception::Exception(const std::wstring& description, HRESULT result)
		: m_result(result), m_caption(L"DirectX Error"), m_description(description + std::wstring(L": ") + HResultToString(result)) {}

	Exception Exception::FileLoadingException(const WCHAR* filename)
	{
		return Exception(std::wstring(L"File Loading Failure"), std::wstring(L"Failed to load file: " + std::wstring(filename)));
	}

	Exception Exception::FileLoadingException(const std::wstring& filename)
	{
		return Exception(std::wstring(L"File Loading Failure"), std::wstring(L"Failed to load file: " + filename));
	}

	Exception Exception::OutOfMemoryException()
	{
		return Exception(L"Not Enough Memory", L"Purchase and install more RAM to continue.");
	}

	const std::wstring Exception::HResultToString(HRESULT result)
	{
		switch (result)
		{
		case S_OK:
			return L"Ok";
		case E_ABORT:
			return L"Operation Aborted";
		case E_ACCESSDENIED:
			return L"General Access Denied Error";
		case E_FAIL:
			return L"Unspecified Failure";
		case E_HANDLE:
			return L"Invalid Handle";
		case E_INVALIDARG:
			return L"One Or More Arguments Are Not Valid";
		case E_NOINTERFACE:
			return L"Interface Not Supported";
		case E_NOTIMPL:
			return L"Not Implemented";
		case E_OUTOFMEMORY:
			return L"Failed To Allocate Necessary Memory";
		case E_POINTER:
			return L"Pointer Not Valid";
		case E_UNEXPECTED:
			return L"Unexpected Failure";
		}
		return L"Unknown Error";
	}
	std::wstring& Exception::Caption()
	{
		return m_caption;
	}
	std::wstring& Exception::Description()
	{
		return m_description;
	}
	HRESULT Exception::HResult()
	{
		return m_result;
	}
	void Exception::ShowMessageBox()
	{
		MessageBox(NULL, m_description.c_str(), m_caption.c_str(), MB_OK);
	}
}