#pragma once

#include "pch.h"

#include <vector>

#include "ModelImpl.h"

namespace ndtech {
  namespace components {
    
    struct PhysicsComponent {

      glm::vec3                                   m_velocity = { 0.0f, 0.0f, 0.0f };
      glm::quat                                   m_angularVelocity = glm::quat();
      glm::vec3                                   m_angularVelocityAxis = glm::vec3();
      float                                       m_angularVelocityRadiansPerSecond;
      glm::quat                                   m_angularVelocityQuaternion = glm::quat();

    };

  }
}