#pragma once
#include <string>
#include <DirectXMath.h>
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct D3D11_DEPTH_STENCIL_DESC;
struct D3D11_RASTERIZER_DESC;
struct ID3D11SamplerState;
struct D3D11_SAMPLER_DESC;
struct ID3D11SamplerState;
struct ID3D11RasterizerState;
struct ID3D11DepthStencilState;
namespace DoremiEngine
{
    namespace Graphic
    {
        class DepthStencilState;
        class RasterizerState;
        class DirectXManager
        {
        public:
            // TODOKO add documents
            virtual ID3D11Device* GetDevice() = 0;
            virtual ID3D11DeviceContext* GetDeviceContext() = 0;
            virtual void SetScreenResolution(DirectX::XMFLOAT2) = 0;
            virtual DirectX::XMFLOAT2 GetScreenResolution() = 0;
            // TODOKO should end draw be here?
            virtual DepthStencilState* CreateDepthStencilState(D3D11_DEPTH_STENCIL_DESC p_depthStencilDesc) = 0; // Forts�tt h�r imorgon
            virtual ID3D11SamplerState* CreateSamplerState(D3D11_SAMPLER_DESC p_samplerDesc) = 0;
            virtual void SwapDepthStencilState(DepthStencilState* p_depthStencilState) = 0;
            virtual RasterizerState* CreateRasterizerState(D3D11_RASTERIZER_DESC p_rasterizerDesc) = 0;
            virtual void SwapRasterizerState(RasterizerState* p_rasterizerState) = 0;
            virtual void EndDraw() = 0;
            virtual ID3D11SamplerState* GetDefaultSamplerState() = 0;
            virtual void DrawCurrentRenderList(ID3D11RasterizerState* p_rasterizerState, ID3D11DepthStencilState* p_depthStencilState) = 0;
        };
    }
}