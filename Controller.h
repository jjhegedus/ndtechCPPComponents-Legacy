#pragma once

#if USE_GLFW
#include <ml_controller.h>
#endif

#if ML_DEVICE
#include <ml_controller.h>
#endif

namespace ndtech {
  class Controller
  {
  public:
    Controller();
    ~Controller();
  };
}

