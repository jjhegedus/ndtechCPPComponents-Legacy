#pragma once

#include "pch.h"
#include <ndtech/ShaderStructures.h>

namespace ndtech {
  namespace components {

    template <typename ThisVertexType>
    struct IndexedInstancedModelImpl {
      using VertexType = ThisVertexType;

      ModelConstantBuffer					                    m_modelConstantBufferData;
      winrt::com_ptr<ID3D11Buffer>			              m_modelConstantBuffer;
      winrt::com_ptr<ID3D11InputLayout>               m_inputLayout;
      winrt::com_ptr<ID3D11Buffer>			              m_vertexBuffer;
      winrt::com_ptr<ID3D11Buffer>                    m_indexBuffer;

    };

  }
}