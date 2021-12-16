#pragma once

#include "pch.h"

#include <vector>

namespace ndtech {
  namespace components {

    struct InstanceRenderingData {
      glm::mat4 world;
      float col;
      float row;
    };

    static std::atomic<size_t> nextIdexedInstancedModelInstanceId;

    struct IndexedInstancedModelInstance {
      size_t                                      m_id = nextIdexedInstancedModelInstanceId.fetch_add(1);

      glm::vec3                                   m_scale = { 0.0f, 0.0f, 0.0f };
      glm::vec3                                   m_position = { 0.0f, 0.0f, 0.0f };
      glm::quat                                   m_orientation = glm::quat();

      glm::vec3                                   m_velocity = { 0.0f, 0.0f, 0.0f };
      glm::quat                                   m_angularVelocity = glm::quat();

      glm::mat4                                   m_transformationMatrix = glm::mat4();

    };

  }
}