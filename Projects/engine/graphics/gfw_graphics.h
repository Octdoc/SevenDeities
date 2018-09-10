#pragma once

#include "helpers/hcs_helpers.h"
#include "resources/gfw_resource.h"
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>

namespace gfw
{
	struct GraphicsSettings
	{
		int width;
		int height;
		bool fullscreen;
		bool vsync;
		bool antialiasing;
		bool cullBack;
		bool enableAlpha;
		bool enableZBuffer;
		std::wstring windowName;

	public:
		GraphicsSettings();
	};

	class Graphics
	{
		SHARED_ONLY(Graphics);

		bool m_vsync;
		bool m_fullscreen;
		int m_screenWidth;
		int m_screenHeight;
		int m_videoCardMemory;
		std::wstring m_videoCardDescription;

		AutoReleasePtr<ID3D11Device> m_device;
		AutoReleasePtr<ID3D11DeviceContext> m_deviceContext;
		AutoReleasePtr<IDXGISwapChain> m_swapChain;
		AutoReleasePtr<ID3D11RenderTargetView> m_renderTargetView;
		AutoReleasePtr<ID3D11Texture2D> m_depthStencilBuffer;
		AutoReleasePtr<ID3D11DepthStencilView> m_depthStencilView;
		AutoReleasePtr<ID3D11DepthStencilState> m_depthStencilState_ZEnabled;
		AutoReleasePtr<ID3D11DepthStencilState> m_depthStencilState_ZDisabled;
		AutoReleasePtr<ID3D11RasterizerState> m_rasterizerStateSolid;
		AutoReleasePtr<ID3D11RasterizerState> m_rasterizerStateWireframe;
		AutoReleasePtr<ID3D11BlendState> m_blendState_alphaOn;
		AutoReleasePtr<ID3D11BlendState> m_blendState_alphaOff;

		float m_clearColor[4];

	private:
		Graphics(HWND hwnd, GraphicsSettings& settings);
		~Graphics();
		void Initialize(HWND hwnd, GraphicsSettings& settings);

		DXGI_RATIONAL InitializeAdapter();
		IDXGIAdapter* GetAdapter();
		void GetAdapterDescription(IDXGIAdapter* adapter);
		void GetDisplayModeList(IDXGIAdapter* adapter, std::vector<DXGI_MODE_DESC>& displayModeList);
		DXGI_RATIONAL GetRefreshRate(std::vector<DXGI_MODE_DESC>& displayModeList);

		void CreateDeviceDeviceContextSwapChain(HWND hwnd, DXGI_RATIONAL& refreshrate);
		void FillSwapChainDesc(DXGI_SWAP_CHAIN_DESC& swapChainDesc, HWND hwnd, DXGI_RATIONAL& refreshrate);
		void CreateScreenRenderTargetView();

		void CreateDepthStencilStates();
		void FillDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC& depthStencilDesc);
		void CreateDepthStencilBuffer(int width, int height);
		void FillDepthBufferDesc(D3D11_TEXTURE2D_DESC& depthBufferDesc, int width, int height);
		void CreateDepthStencilView();
		void FillDepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC& depthStencilViewDesc);

		void CreateRasterizerStates(bool antialiasing, bool cullBack);
		void FillRasterizerDesc(D3D11_RASTERIZER_DESC& rasterizerDesc, bool antialiasing, bool cullBack);

		void CreateBlendStates();
		void FillBlendDesc(D3D11_BLEND_DESC& blendDesc);

		void InitializeDirect3D(HWND hwnd, GraphicsSettings& settings);

	public:
		static Graphics::P Create(HWND hwnd, GraphicsSettings& settings);
		
		void RenderToScreen();
		void RenderToScreenSetTarget();
		void RenderToSurface(ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView);
		void Present();

		void setClearColor(float r, float g, float b, float a);
		void SetViewPort();
		void SetViewPort(float width, float height);
		void SetViewPort(float x, float y, float width, float height);
		void EnableAlphaBlending(bool alpha);
		void EnableZBuffer(bool enable);
		void SetPrimitiveTopology_Points();
		void SetPrimitiveTopology_Lines();
		void SetPrimitiveTopology_Triangles();
		void SetFillMode_Solid();
		void SetFillMode_Wireframe();

		ID3D11Device* getDevice();
		ID3D11DeviceContext* getDeviceContext();
		float GetScreenAspectRatio();

		void getVideoCardInfo(std::wstring& cardName, int& memory);
	};
}