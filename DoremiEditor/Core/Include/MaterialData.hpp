#pragma once
#include <DoremiEditor/Core/Include/SharedVariables.hpp>

namespace DoremiEditor
{
    namespace Core
    {
        enum class ActiveTextures : int
        {
            COLORMAP = 0x01,
            GLOWMAP = 0x02,
            SPECMAP = 0x04,
            BUMPMAP = 0x08
        };
        struct MaterialData
        {
            MaterialData()
            {
                mapMasks = 0;
                diffuse = 0;
                color[0] = color[1] = color[2] = 0.5f;
                ambColor[0] = ambColor[1] = ambColor[2] = 0.0f;
                specColor[0] = specColor[1] = specColor[2] = 0.0f;
                specCosine = specEccentricity = specRollOff = 0;
            }
            int mapMasks;
            float diffuse;
            float color[3];
            float ambColor[3];
            float specColor[3];
            float specCosine;
            float specEccentricity;
            float specRollOff;
            char* diffuseTextureName;
            char* glowTextureName;
        };
    }
}