#pragma once

#include "pch.h"

// Effects Files will always need to include Action and Store
#include <ndtech/Action.h>
#include <ndtech/Store.h>

// Effects Files will always need to include the file defining the actions they implement
// and the file defining the state of the store they are using
#include "BillboardActions.h"
#include <ndtechHolo/AppState.h>

// Effects Files that are effects for components will typically need to respond to some Actions from the App as well
#include <ndtech/Action.h>

// Effects will often need to include other headers with required definitions
#include "Billboard.h"
#include <ndtech/ShaderStructures.h>
#include <ndtechHolo/SpeechRecognition.h>
#include <ndtech/Utilities.h>
#include <ndtech/Store.h>

using namespace DirectX;


namespace ndtech {
    namespace holo {
        namespace entities {


            namespace BillboardEffects {

                // Utiliy Functions : Begin
                // to improve modularity and reuse
                // These Functions take the StoreState as a parameter
                // so they can only be called from within an effect,
                // they MUST be synchronous and
                // they typically return the store state
                // so changes to the store state are not lost

                // Utility Functions : End


                // Handler Functions : Begin
                // Handler functions in Entity Effects files take the list of entities as a parameter
                // they SHOULD be synchronous and
                // they MUST return the updated entities in order for changes to take effect
                // they can work with the Dispatcher

                // Handler Function for the CreateBillboard event
                template<typename EntityType>
                std::vector<std::pair<size_t, EntityType>> CreateBillboard(std::vector<std::pair<size_t, EntityType>> entities, Action action) {

                    //TraceLogWrite("BillboardEffects handler for BillboardActions::CREATE_POINT_Billboard");

                    Billboard billboard = std::any_cast<Billboard>(action.payload);

                    entities.push_back(std::pair<size_t, EntityType>{billboard.entityId, billboard});

                    return entities;
                }

                // Handler Functions : End

                // Effects files will have at least one function to register the effects they provide
                // which will take a reference to the store as a parameter.
                // In some cases, a single effects file may have multiple RegisterEffects functions
                // for different stores or different categories of functionality
                template<typename EntityTypeAndDependencies>
                void RegisterBillboardEffects(EntityStore<EntityTypeAndDependencies>* store) {

                    std::string test = TypeUtilities::GetTypesString(EntityStore<EntityTypeAndDependencies>::DataType{}, "EntityStore<EntityTypeAndDependencies>::DataType:");

                    //TraceLogWrite(
                    //    "ndtech::holo::entities::BillboardEffects::RegisterBillboardEffects()",
                    //    TraceLoggingString(test.c_str(), "EntityStore<EntityTypeAndDependencies>::DataType"));

                    store->RegisterEffect(
                        BillboardActions::CREATE_BILLBOARD.actionTypeId,
                        [](typename EntityStore<EntityTypeAndDependencies>::DataType entities, Action action) -> typename EntityStore<EntityTypeAndDependencies>::DataType {
                            return CreateBillboard(entities, action);
                        }
                    );

                }
            }
        }
    }
}