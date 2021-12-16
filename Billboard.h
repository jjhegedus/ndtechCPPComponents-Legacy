#pragma once

#include "pch.h"
#include "BillboardInstance.h"

namespace ndtech {
  namespace components {

    template <typename VertexType>
    struct Billboard {
      using                                           VertexType = VertexType;
      using                                           InstanceType = BillboardInstance;

      std::vector<VertexType>                         m_vertices;
      std::vector<unsigned short>                     m_indices;
      std::vector<InstanceType>                       m_instances;

      winrt::com_ptr<ID3D11InputLayout>               inputLayout;
      winrt::com_ptr<ID3D11Buffer>                    vertexBuffer;
      size_t                                          vertexCount;
      winrt::com_ptr<ID3D11Buffer>                    modelConstantBuffer;
      ModelConstantBuffer                             modelConstantBufferData;

      // Support for collisions and picking
      //DirectX::BoundingSphere                         originalBoundingSphere{};
      //DirectX::BoundingSphere                         transformedBoundingSphere{};
    };

  }
}