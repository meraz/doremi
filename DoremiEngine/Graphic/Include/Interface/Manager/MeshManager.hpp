#pragma once
#include <string>
namespace DirectX
{
    struct XMFLOAT4X4;
    struct XMFLOAT2;
}
namespace DoremiEngine
{
    namespace Graphic
    {
        class MeshInfo;
        class MaterialInfo;
        class MeshManager
        {
            public:
            // TODOKO add documents
            virtual MeshInfo* BuildMeshInfo(const std::string& p_fileName) = 0;
            virtual MeshInfo* BuildQuadMeshInfo(const std::string& p_fileName) = 0;
            // TODOKO change to acctually load material and not just textures
            virtual MaterialInfo* BuildMaterialInfo(const std::string& p_fileName) = 0;
            virtual void AddToRenderList(MeshInfo& p_mesh, MaterialInfo& p_material, const DirectX::XMFLOAT4X4& p_orientationMatrix) = 0;
            virtual void Draw() = 0;
        };
    }
}