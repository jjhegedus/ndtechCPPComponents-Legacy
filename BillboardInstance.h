#pragma once

#include "pch.h"

namespace ndtech {
  namespace components {

    static std::atomic<size_t> nextBillboardlInstanceId = 1;

    struct BillboardInstance {

      size_t                                      m_id = nextBillboardlInstanceId.fetch_add(1);
      glm::vec3                                   m_scale = { 0.0f, 0.0f, 0.0f };
      glm::vec3                                   m_position = { 0.0f, 0.0f, -2.0f };
      glm::vec3                                   m_targetPosition = { 0.f, 0.f, -2.f };
      glm::mat4                                   m_transformationMatrix = glm::mat4();
      glm::vec3                                   m_velocity = { 0.0f, 0.0f, 0.0f };
      glm::quat                                   m_angularVelocity = glm::quat();
      glm::quat                                   m_orientation = glm::quat();
      glm::vec3                                   m_angularVelocityAxis = glm::vec3();
      float                                       m_angularVelocityRadiansPerSecond;
      glm::quat                                   m_angularVelocityQuaternion = glm::quat();
      bool                                        m_fadingIn = false;
      float                                       LERP_RATE = 4.0f;                                   // The rate at which the hologram position is interpolated (LERPed) to the targetPosition
      float                                       MAX_FADE_TIME = 1.f;                                // Number of seconds it takes to fade the hologram in, or out.
      float                                       fadeTime = 0.f;
      bool                                        visible = true;
      float                                       blendFactors[4] = { 0.f, 0.f, 0.f, 0.f };
      bool                                        worldLocked = true;
      int                                         zindex = 0;

    };

  }
}