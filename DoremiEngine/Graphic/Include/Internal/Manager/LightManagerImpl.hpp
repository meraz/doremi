#pragma once

#include <Interface/Manager/LightManager.hpp>
#include <Internal/Light/LightInfoImpl.hpp>
#include <d3d11_1.h>
#include <vector>

namespace DoremiEngine
{
    namespace Graphic
    {
        struct GraphicModuleContext;
        // Global light list
        struct LightBuffer
        {
            Light lightList[100];
        };

        class LightManagerImpl : public LightManager
        {
        public:
            LightManagerImpl(const GraphicModuleContext& p_graphicContext);
            virtual ~LightManagerImpl();
            Light* AddLight(int type, float intensity, DirectX::XMFLOAT3 color, float coneAngle, DirectX::XMFLOAT3 direction, float penumAngle,
                            DirectX::XMFLOAT3 position) override;
            void InitLightManager() override;
            void TestFunc() override;
            void UpdateLights() override;

        private:
            const GraphicModuleContext& m_graphicContext;
            ID3D11Device* m_device;
            ID3D11DeviceContext* m_deviceContext;
            ID3D11Buffer* m_lBuffer;
            LightBuffer m_lightBuffer;
            int m_lightcount;
        };
    }
}
