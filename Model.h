#pragma once

#include "pch.h"

#include <vector>

#include "ModelImpl.h"

namespace ndtech {
  namespace components {

    template <typename ThisVertexType>
    struct Model: public ModelImpl<ThisVertexType>{
      using VertexType = ThisVertexType;

      std::vector<VertexType>                     m_vertices;
      std::vector<unsigned short>                 m_indices;
      glm::vec3                                   m_scale = { 0.0f, 0.0f, 0.0f };
      glm::vec3                                   m_position = { 0.0f, 0.0f, 0.0f };
      glm::mat4                                   m_transformationMatrix = glm::mat4();
      glm::vec3                                   m_velocity = { 0.0f, 0.0f, 0.0f };
      glm::quat                                   m_angularVelocity = glm::quat();
      glm::quat                                   m_orientation = glm::quat();
      glm::vec3                                   m_angularVelocityAxis = glm::vec3();
      float                                       m_angularVelocityRadiansPerSecond;
      glm::quat                                   m_angularVelocityQuaternion = glm::quat();

    };

  }
}