#pragma once

#include "pch.h"

namespace ndtech {
  namespace components {

    template <typename ThisVertexType>
    struct IndexedInstancedModelImpl {
      using VertexType = ThisVertexType;

      GLuint m_vao;
      GLuint m_vbo;
      GLuint m_ibo;

    };

  }
}