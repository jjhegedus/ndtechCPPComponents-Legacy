#pragma once

#pragma once

#include "pch.h"

#include <ndtech/Action.h>
#include <ndtech/Dispatcher.h>

#include "Billboard.h"

namespace ndtech {
    namespace holo {
        namespace entities {

            struct BillboardActions {

                static Action CREATE_BILLBOARD;
                static Action CreateBillboard(Billboard billboard) {
                    return Action{ CREATE_BILLBOARD.actionTypeId, CREATE_BILLBOARD.typeName, billboard, ActionCategory::AnyThreadImmediate };
                }

            };


            Action BillboardActions::CREATE_BILLBOARD = Action{ Dispatcher::GetNextActionTypeId(), "CreateBillboard" };
        }
    }
}