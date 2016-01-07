#include <Interface/PotentialField/PotentialField.hpp>
#include <Internal/PotentialField/PotentialFieldActorImpl.hpp>
namespace DoremiEngine
{
    namespace AI
    {
        class PotentialFieldImpl : public PotentialField
        {
            public:
            PotentialFieldImpl();
            virtual ~PotentialFieldImpl();
            void SetGrid(const std::vector<std::vector<PotentialFieldGridPoint>>& p_grid) override;
            void SetHeight(const float& p_height) override { m_height = p_height; };
            void SetWidth(const float& p_width) override { m_width = p_width; };
            void SetCenter(const DirectX::XMFLOAT2& p_center) override { m_center = p_center; };
            void Update() override;
            void AddActor(PotentialFieldActor* p_newActor) override;
            DirectX::XMFLOAT2 GetAttractionPosition(const DirectX::XMFLOAT3& p_unitPosition) override;

            private:
            std::vector<std::vector<PotentialFieldGridPoint>> m_grid; // [width][height]
            std::vector<PotentialFieldActor*> m_actors;
            float m_width;
            float m_height;
            DirectX::XMFLOAT2 m_center;
        };
    }
}