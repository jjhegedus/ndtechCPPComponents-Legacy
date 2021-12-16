#pragma once

#include "pch.h"

#include <ndtech/ShaderStructures.h>
#include <ndtech/DirectXVertexShader.h>
#include <ndtech/DirectXPixelShader.h>
#include <ndtech/DirectXGeometryShader.h>
#include <ndtech/Entity.h>

namespace ndtech {

    namespace holo {
        namespace entities {

            struct Billboard {

                // Direct3D resources for cube geometry.
                DirectXVertexShader                             vertexShader;
                DirectXPixelShader                              pixelShader;
                DirectXGeometryShader                           geometryShader;

                bool                                            initialized = false;
                bool                                            active = false;

                // Variables used with the rendering loop.
                float                                           radius = .1f;
                float                                           relativePositioning = false;
                winrt::Windows::Foundation::Numerics::float3    relativePosition = { 0.f, 0.f, 2.f };

                DirectX::XMFLOAT3                               color = DirectX::XMFLOAT3(0.f, 0.f, 1.0f);

                winrt::com_ptr<ID3D11InputLayout>               inputLayout;
                winrt::com_ptr<ID3D11Buffer>                    vertexBuffer;
                size_t                                          vertexCount;
                winrt::com_ptr<ID3D11Buffer>                    modelConstantBuffer;

                // System resources for buble geometry.
                ModelConstantBuffer                             modelConstantBufferData;

                // Support for collisions and picking
                DirectX::BoundingSphere                         originalBoundingSphere{};
                DirectX::BoundingSphere                         transformedBoundingSphere{};

                size_t                                          entityId = nextEntityId.fetch_add(1);


                // Variables used with the rendering loop.
                float                                           degreesPerSecond = 45.f;
                winrt::Windows::Foundation::Numerics::float3    lastPosition = { 0.f, 0.f, -2.f };
                winrt::Windows::Foundation::Numerics::float3    velocity = { 0.f, 0.f,  0.f };

                // Timer used to fade the hologram in, or out.
                float                                           fadeTime = 0.f;

                // Whether or not the hologram is fading in, or out.
                bool                                            fadingIn = false;

                // This is the rate at which the hologram position is interpolated (LERPed) to the current location.
                float                                           LERP_RATE = 4.0f;

                // Number of seconds it takes to fade the hologram in, or out.
                float                                           MAX_FADE_TIME = 1.f;
                winrt::Windows::Foundation::Numerics::float3    position = { 0.f, 0.f, -2.f };

                bool                                            visible = true;

                float                                           blendFactors[4] = { 0.f, 0.f, 0.f, 0.f };

                bool                                            worldLocked = true;
                int                                             zindex = 0;

                bool operator==(const Billboard& other) const {
                    return entityId == other.entityId;
                }
            };
        }
    }


}