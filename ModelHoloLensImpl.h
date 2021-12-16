#pragma once

#include "pch.h"
#include <ndtech/ShaderStructures.h>

namespace ndtech {
  namespace components {

    template <typename ThisVertexType>
    struct ModelImpl {
      using VertexType = ThisVertexType;

      ModelConstantBuffer					                    m_modelConstantBufferData;
      winrt::com_ptr<ID3D11Buffer>			              m_modelConstantBuffer;
      winrt::com_ptr<ID3D11InputLayout>               m_inputLayout;
      winrt::com_ptr<ID3D11Buffer>			              m_vertexBuffer;
      winrt::com_ptr<ID3D11Buffer>                    m_indexBuffer;

      DirectX::XMMATRIX                             initialOrientationMatrix;
      DirectX::XMMATRIX                             orientationMatrix;


      float vertices[9] = {
                 -1.0f, -1.0f, 0.0f,
                 1.0f, -1.0f, 0.0f,
                 0.0f,  1.0f, 0.0f,
      };

      UINT                                        m_stride = sizeof(VertexType);
      UINT                                        m_offset = 0;

    };

  }
}