#pragma once

#include "ndtech/pch.h"
#include "ndtech/App.h"

namespace ndtech {
  namespace components {
    template <typename ConcreteComponent>
    struct Component {

#if NDTECH_HOLO
      // Used to send per-vertex data to the vertex shader.
      struct VertexPositionColor
      {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT3 color;
      };

      // Constant buffer used to send hologram position transform to the shader pipeline.
      DirectX::XMFLOAT4X4 transformationMatrix = DirectX::XMMatrixIdentity;
#endif


#if NDTECH_ML
      glm::mat4 transformationMatrix = glm::make_mat4(Triangle::matrixData);
#endif

    };
  }
}