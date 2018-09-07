#include "gfw_graphics.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "windowscodecs.lib")
#pragma comment (lib, "DirectXTex.lib")

namespace gfw
{
	GraphicsSettings::GraphicsSettings()
		:width(1280),
		height(720),
		fullscreen(false),
		vsync(true),
		antialiasing(false),
		cullBack(true),
		enableAlpha(false),
		enableZBuffer(true),
		windowName(L"Octdoc") {}

#pragma region Adapter init

	IDXGIAdapter* Graphics::GetAdapter()
	{
		HRESULT result;
		AutoReleasePtr<IDXGIFactory> factory;
		IDXGIAdapter* adapter;

		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result))
			throw hcs::Exception(L"Failed to create DXGI factory.", result);

		factory->EnumAdapters(0, &adapter);
		return adapter;
	}
	void Graphics::GetAdapterDescription(IDXGIAdapter* adapter)
	{
		DXGI_ADAPTER_DESC adapterDesc;
		adapter->GetDesc(&adapterDesc);
		m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
		m_videoCardDescription = adapterDesc.Description;
	}
	void  Graphics::GetDisplayModeList(IDXGIAdapter* adapter, std::vector<DXGI_MODE_DESC>& displayModeList)
	{
		AutoReleasePtr<IDXGIOutput> adapterOutput;
		unsigned int numModes;

		adapter->EnumOutputs(0, &adapterOutput);
		adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		displayModeList.resize(numModes);
		adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList.data());
	}
	DXGI_RATIONAL Graphics::GetRefreshRate(std::vector<DXGI_MODE_DESC>& displayModeList)
	{
		DXGI_RATIONAL refreshrate = { 0, 1 };
		for (size_t i = 0; i < displayModeList.size(); i++)
			if (displayModeList[i].Width == (UINT)m_screenWidth && displayModeList[i].Height == (UINT)m_screenHeight)
				refreshrate = displayModeList[i].RefreshRate;
		return refreshrate;
	}
	DXGI_RATIONAL Graphics::InitializeAdapter()
	{
		AutoReleasePtr<IDXGIAdapter> adapter;
		std::vector<DXGI_MODE_DESC> displayModeList;

		adapter = GetAdapter();
		GetAdapterDescription(adapter);
		GetDisplayModeList(adapter, displayModeList);
		return GetRefreshRate(displayModeList);
	}

#pragma endregion

#pragma region Device, swapchain

	void Graphics::FillSwapChainDesc(DXGI_SWAP_CHAIN_DESC& swapChainDesc, HWND hwnd, DXGI_RATIONAL& refreshrate)
	{
		ClearStruct(swapChainDesc);
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = m_screenWidth;
		swapChainDesc.BufferDesc.Height = m_screenHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		if (m_vsync)
		{
			swapChainDesc.BufferDesc.RefreshRate = refreshrate;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = !m_fullscreen;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;
	}
	void Graphics::CreateDeviceDeviceContextSwapChain(HWND hwnd, DXGI_RATIONAL& refreshrate)
	{
		HRESULT result;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		D3D_FEATURE_LEVEL featureLevel[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0
		};
		FillSwapChainDesc(swapChainDesc, hwnd, refreshrate);
		result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
			featureLevel, sizeof(featureLevel) / sizeof(featureLevel[0]), D3D11_SDK_VERSION,
			&swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
		if (FAILED(result))
			throw hcs::Exception(L"Failed to create directX device and swap chain.", result);
	}
	void Graphics::CreateScreenRenderTargetView()
	{
		HRESULT result;
		AutoReleasePtr<ID3D11Texture2D> backBufferPtr;
		result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferPtr);
		if (FAILED(result))
			throw hcs::Exception(L"Failed to get swap chain buffer.", result);
		result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
		if (FAILED(result))
			throw hcs::Exception(L"Failed to create render target view.", result);
	}

#pragma endregion

#pragma region Depth buffer

	void Graphics::FillDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC& depthStencilDesc)
	{
		ClearStruct(depthStencilDesc);
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	}
	void Graphics::FillDepthBufferDesc(D3D11_TEXTURE2D_DESC& depthBufferDesc, int width, int height)
	{
		ClearStruct(depthBufferDesc);
		depthBufferDesc.Width = width;
		depthBufferDesc.Height = height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
	}
	void Graphics::FillDepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC& depthStencilViewDesc)
	{
		ClearStruct(depthStencilViewDesc);
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
	}
	void Graphics::CreateDepthStencilStates()
	{
		HRESULT result;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		FillDepthStencilDesc(depthStencilDesc);

		depthStencilDesc.DepthEnable = true;
		result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState_ZEnabled);
		if (FAILED(result))
			throw hcs::Exception(L"Failed to create depth stencil state.", result);

		depthStencilDesc.DepthEnable = false;
		result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState_ZDisabled);
		if (FAILED(result))
			throw hcs::Exception(L"Failed to create depth stencil state.", result);
	}
	void Graphics::CreateDepthStencilBuffer(int width, int height)
	{
		HRESULT result;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		FillDepthBufferDesc(depthBufferDesc, width, height);
		result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
		if (FAILED(result))
			throw hcs::Exception(L"Failed to create depth buffer.", result);
	}
	void Graphics::CreateDepthStencilView()
	{
		HRESULT result;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		FillDepthStencilViewDesc(depthStencilViewDesc);
		result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
		if (FAILED(result))
			throw hcs::Exception(L"Failed to create depth stencil view.", result);
	}

#pragma endregion

#pragma region Rasterizer

	void Graphics::CreateRasterizerStates(bool antialiasing, bool cullBack)
	{
		HRESULT result;
		D3D11_RASTERIZER_DESC rasterizerDesc;
		FillRasterizerDesc(rasterizerDesc, antialiasing, cullBack);
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		result = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStateSolid);
		if (FAILED(result))
			throw hcs::Exception(L"Failed to create rasterizer state.", result);
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		result = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerStateWireframe);
		if (FAILED(result))
			throw hcs::Exception(L"Failed to create rasterizer state.", result);
	}
	void Graphics::FillRasterizerDesc(D3D11_RASTERIZER_DESC& rasterizerDesc, bool antialiasing, bool cullBack)
	{
		ClearStruct(rasterizerDesc);
		rasterizerDesc.AntialiasedLineEnable = antialiasing;
		rasterizerDesc.CullMode = cullBack ? D3D11_CULL_BACK : D3D11_CULL_NONE;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = true;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.MultisampleEnable = false;
		rasterizerDesc.ScissorEnable = false;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	}

#pragma endregion

#pragma region Blend state

	void Graphics::FillBlendDesc(D3D11_BLEND_DESC& blendDesc)
	{
		ClearStruct(blendDesc);
		blendDesc.RenderTarget[0].BlendEnable = false;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	}
	void Graphics::CreateBlendStates()
	{
		HRESULT result;
		D3D11_BLEND_DESC blendDesc;
		FillBlendDesc(blendDesc);

		blendDesc.RenderTarget[0].BlendEnable = true;
		result = m_device->CreateBlendState(&blendDesc, &m_blendState_alphaOn);
		if (FAILED(result))
			throw hcs::Exception(L"Failed to create blend state.", result);

		blendDesc.RenderTarget[0].BlendEnable = false;
		result = m_device->CreateBlendState(&blendDesc, &m_blendState_alphaOff);
		if (FAILED(result))
			throw hcs::Exception(L"Failed to create blend state.", result);
	}

#pragma endregion

#pragma region Direct3D

	void Graphics::InitializeDirect3D(HWND hwnd, GraphicsSettings& settings)
	{
		DXGI_RATIONAL refreshrate = InitializeAdapter();
		CreateDeviceDeviceContextSwapChain(hwnd, refreshrate);
		CreateScreenRenderTargetView();
		CreateDepthStencilStates();
		CreateDepthStencilBuffer(m_screenWidth, m_screenHeight);
		CreateDepthStencilView();
		CreateRasterizerStates(settings.antialiasing, settings.cullBack);
		CreateBlendStates();

		m_deviceContext->RSSetState(m_rasterizerStateSolid);
		SetPrimitiveTopology_Triangles();
		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
		SetViewPort(m_screenWidth, m_screenHeight);
		EnableAlphaBlending(settings.enableAlpha);
		EnableZBuffer(settings.enableZBuffer);
	}
	void Graphics::ShutdownDirect3D()
	{
		if (m_fullscreen && m_swapChain != nullptr)
			m_swapChain->SetFullscreenState(false, nullptr);
		m_device.Release();
		m_deviceContext.Release();
		m_swapChain.Release();
		m_renderTargetView.Release();
		m_depthStencilBuffer.Release();
		m_depthStencilView.Release();
		m_depthStencilState_ZEnabled.Release();
		m_depthStencilState_ZDisabled.Release();
		m_rasterizerStateSolid.Release();
		m_rasterizerStateWireframe.Release();
		m_blendState_alphaOn.Release();
		m_blendState_alphaOff.Release();
	}

#pragma endregion

#pragma region Create, shutdown
	
	Graphics::Graphics() :m_vsync(false), m_fullscreen(false), m_screenWidth(0), m_screenHeight(0), m_videoCardMemory(0)
	{
		m_clearColor[0] = 0.0f;
		m_clearColor[1] = 0.0f;
		m_clearColor[2] = 0.0f;
		m_clearColor[3] = 0.0f;
	}

	Graphics::~Graphics()
	{
		Shutdown();
	}

	void Graphics::Initialize(HWND hwnd, GraphicsSettings& settings)
	{
		m_vsync = settings.vsync;
		m_fullscreen = settings.fullscreen;
		m_screenWidth = settings.width;
		m_screenHeight = settings.height;
		setClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		InitializeDirect3D(hwnd, settings);
	}
	void Graphics::Shutdown()
	{
		ShutdownDirect3D();
	}

#pragma endregion

#pragma region Rendering


	void Graphics::RenderToScreen()
	{
		m_deviceContext->ClearRenderTargetView(m_renderTargetView, m_clearColor);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	void Graphics::RenderToScreenSetTarget()
	{
		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
		m_deviceContext->ClearRenderTargetView(m_renderTargetView, m_clearColor);
		m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	void Graphics::RenderToSurface(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView)
	{
		m_deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
		m_deviceContext->ClearRenderTargetView(renderTargetView, m_clearColor);
		m_deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	void Graphics::Present()
	{
		HRESULT result = m_swapChain->Present(m_vsync ? 1 : 0, 0);
		if (FAILED(result))
		{
			result = m_device->GetDeviceRemovedReason();
			throw hcs::Exception(L"Failed to present image", result);
		}
	}

#pragma endregion

#pragma region setters

	void Graphics::setClearColor(float r, float g, float b, float a)
	{
		m_clearColor[0] = r;
		m_clearColor[1] = g;
		m_clearColor[2] = b;
		m_clearColor[3] = a;
	}
	void Graphics::SetViewPort()
	{
		SetViewPort(m_screenWidth, m_screenHeight);
	}
	void Graphics::SetViewPort(float width, float height)
	{
		D3D11_VIEWPORT viewport;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		m_deviceContext->RSSetViewports(1, &viewport);
	}
	void Graphics::EnableAlphaBlending(bool alpha)
	{
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_deviceContext->OMSetBlendState(alpha ? m_blendState_alphaOn : m_blendState_alphaOff, blendFactor, 0xffffffff);
	}
	void Graphics::EnableZBuffer(bool enable)
	{
		m_deviceContext->OMSetDepthStencilState(enable ? m_depthStencilState_ZEnabled : m_depthStencilState_ZDisabled, 0);
	}
	void Graphics::SetPrimitiveTopology_Points()
	{
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	}
	void Graphics::SetPrimitiveTopology_Lines()
	{
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	}
	void Graphics::SetPrimitiveTopology_Triangles()
	{
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	void Graphics::SetFillMode_Solid()
	{
		m_deviceContext->RSSetState(m_rasterizerStateSolid);
	}
	void Graphics::SetFillMode_Wireframe()
	{
		m_deviceContext->RSSetState(m_rasterizerStateWireframe);
	}

#pragma endregion

#pragma region getters

	ID3D11Device* Graphics::getDevice()
	{
		return m_device;
	}

	ID3D11DeviceContext* Graphics::getDeviceContext()
	{
		return m_deviceContext;
	}

	float Graphics::GetScreenAspectRatio()
	{
		return (float)m_screenWidth / (float)m_screenHeight;
	}

	void Graphics::getVideoCardInfo(std::wstring& cardName, int& memory)
	{
		cardName = m_videoCardDescription;
		memory = m_videoCardMemory;
	}

#pragma endregion

}