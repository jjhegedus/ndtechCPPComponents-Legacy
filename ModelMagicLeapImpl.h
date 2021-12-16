#pragma once
#include "pch.h"

namespace ndtech {
  namespace components {

    template <typename ThisVertexType>
    struct ModelImpl {
      using VertexType = ThisVertexType;

      GLuint m_vao;
      GLuint m_vbo;
      GLuint m_ibo;
    };

  }
}