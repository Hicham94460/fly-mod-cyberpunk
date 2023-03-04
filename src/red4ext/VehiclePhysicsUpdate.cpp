#include "FlightModule.hpp"
#include <RED4ext/Scripting/Natives/vehiclePhysics.hpp>
#include <RED4ext/Scripting/Natives/Generated/vehicle/BaseObject.hpp>
#include <RED4ext/Scripting/Natives/Generated/vehicle/CarBaseObject.hpp>
#include <RED4ext/RED4ext.hpp>
#include <spdlog/spdlog.h>
#include "VehiclePhysicsUpdate.hpp"
#include "FlightComponent.hpp"
#include "Addresses.hpp"
#include "VehicleSpeedUnlimiter.hpp"

REGISTER_FLIGHT_HOOK(void __fastcall, ProcessAirResistance, 
    RED4ext::vehicle::WheeledPhysics *a1, float deltaTime) {
  auto physicsData = a1->parent->physicsData;
  auto velocity = physicsData->velocity;
  auto X = velocity.X;
  auto Y = velocity.Y;
  auto Z = velocity.Z;
  auto speedSquared = (float)((float)(X * X) + (float)(Y * Y)) + (float)(Z * Z);
  if (_fdclass(speedSquared) != 1 && speedSquared >= 10000.0) {
    auto unk568 = a1->parent->unk568;
    if (speedSquared > 0.0099999998) {
      auto speed = sqrt(speedSquared);
      if (speed != 0.0) {
        X = X / speed;
        Y = Y / speed;
        Z = Z / speed;
      }
      RED4ext::Vector3 airResistanceForce;
      auto yankX = (float)((float)(X * -1.2) * a1->airResistanceFactor) * speedSquared;
      auto yankY = (float)((float)(Y * -1.2) * a1->airResistanceFactor) * speedSquared;
      auto yankZ = (float)((float)(Z * -1.2) * a1->airResistanceFactor) * speedSquared;
      airResistanceForce.X = yankX * deltaTime;
      airResistanceForce.Y = yankY * deltaTime;
      airResistanceForce.Z = yankZ * deltaTime;
      physicsData->force += airResistanceForce;
      unk568->unk108 = sqrt((float)((float)(yankX * yankX) + (float)(yankY * yankY)) + (float)(yankZ * yankZ));
    }
  }
  ProcessAirResistance_Original(a1, deltaTime);
}

REGISTER_FLIGHT_HOOK(void __fastcall, vehiclePhysicsData_ApplyTorqueAtPosition,
                     RED4ext::vehicle::PhysicsData *physicsData, RED4ext::Vector3 *offset, RED4ext::Vector3 *torque) {
  auto fc = FlightComponent::Get(physicsData->vehicle);
  if (fc && fc->active) {
    return;
  } else {
    vehiclePhysicsData_ApplyTorqueAtPosition_Original(physicsData, offset, torque);
  }
}

REGISTER_FLIGHT_HOOK(void __fastcall, vehiclePhysicsData_ApplyForceAtPosition,
                     RED4ext::vehicle::PhysicsData *physicsData, RED4ext::Vector3 *offset, RED4ext::Vector3 *force) {
  auto fc = FlightComponent::Get(physicsData->vehicle);
  if (fc && fc->active) {
    return;
  } else {
    vehiclePhysicsData_ApplyForceAtPosition_Original(physicsData, offset, force);
  }
}

REGISTER_FLIGHT_HOOK(uintptr_t __fastcall, VehicleHelperUpdate, RED4ext::vehicle::WheeledPhysics *p, float deltaTime) {
  auto fc = FlightComponent::Get(p->parent);
  if (fc) {
    auto size = p->driveHelpers.size;
    if (fc->active) {
      p->driveHelpers.size = 0;
    }
    auto result = VehicleHelperUpdate_Original(p, deltaTime);
    p->driveHelpers.size = size;
    return result;
  } else {
    return VehicleHelperUpdate_Original(p, deltaTime);
  }
}

REGISTER_FLIGHT_HOOK(void __fastcall, vehiclePhysicsData_AddTorque, 
    RED4ext::vehicle::PhysicsData *a1, RED4ext::Vector3 *torque) {
  auto fc = FlightComponent::Get(a1->vehicle);
  if (fc && fc->active) {
    return;
  } else {
    vehiclePhysicsData_AddTorque_Original(a1, torque);
  }
}

REGISTER_FLIGHT_HOOK(void __fastcall, VehicleUpdateOrientationWithPID, 
    RED4ext::vehicle::CarBaseObject *a1, RED4ext::Transform *a2, float a3, float a4) {
  auto fc = FlightComponent::Get(a1);
  if (fc && fc->active) {
    return;
  }
  VehicleUpdateOrientationWithPID_Original(a1, a2, a3, a4);
}

REGISTER_FLIGHT_HOOK(uintptr_t __fastcall, vehicleCarPhysics_AnimationUpdate, RED4ext::vehicle::CarPhysics *a1,
                     float timeDelta) {
  auto fc = FlightComponent::Get(a1->parent);
  if (fc) {
    auto rtti = RED4ext::CRTTISystem::Get();
    auto fcc = rtti->GetClass("FlightComponent");
    auto rollProp = fcc->GetProperty("roll");
    if (fc->active) {
      a1->parent->turnInput = rollProp->GetValue<float>(fc);
    }
  }
  return vehicleCarPhysics_AnimationUpdate_Original(a1, timeDelta);
}

REGISTER_FLIGHT_HOOK(uintptr_t __fastcall, vehicleBikePhysics_AnimationUpdate, RED4ext::vehicle::BikePhysics *a1) {
  auto fc = FlightComponent::Get(a1->parent);
  if (fc && fc->active) {
    a1->parent->turnInput = 0.0;
    a1->turnRate = 0.0;
    a1->tiltControlEnabled = 0;
  } else {
    a1->tiltControlEnabled = 1;
  }
  auto og = vehicleBikePhysics_AnimationUpdate_Original(a1);
  // UpdateAnimValueForCName(a1->parent3, "throttle", 0.0);
  return og;
}

// prevents wheels from adding torque/etc
//REGISTER_FLIGHT_HOOK(void __fastcall, vehicleWheeledPhysics_SomethingWheelRayTrace,
//    RED4ext::vehicle::WheeledPhysics *physics, unsigned __int8 index) {
//  auto fc = FlightComponent::Get(physics->parent);
//  if (fc && fc->active) {
//    for (int i = 0; i < physics->unkD10->numWheels; i++) {
//      auto wheel = physics->unkD10->wheel[i];
//      wheel.unk8C = RED4ext::Vector3(INFINITY, INFINITY, INFINITY);
//      wheel.unkA0.position.X = INFINITY;
//      wheel.unkA0.position.Y = INFINITY;
//      wheel.unkA0.position.Z = INFINITY;
//      wheel.unkA0.position.W = 0.0;
//      wheel.unkA0.orientation = RED4ext::Quaternion(0.0, 0.0, 0.0, 1.0);
//      wheel.physMaterial.hash = 0;
//      wheel.unkC0[0] = 0x00000000;
//      wheel.unkC0[1] = 0x00000000;
//      wheel.unkC0[2] = 0xffffffff;
//      wheel.unkC0[3] = 0x00000000;
//      wheel.unkC0[4] = 0xffffffff;
//      wheel.unkD4 = 0;
//      wheel.unkD8 = 0x00030000;
//      wheel.unkCF0 = false;
//      wheel.unkCF8 = INFINITY;
//      wheel.relatedToAir = INFINITY;
//    }
//    for (int i = 0; i < physics->insert2Count; i++) {
//      physics->insert2[i].worldPosition.X = 0.0;
//      physics->insert2[i].worldPosition.Y = 0.0;
//      physics->insert2[i].worldPosition.Z = 0.0;
//      physics->insert2[i].unk0C.X = 0.0;
//      physics->insert2[i].unk0C.Y = 0.0;
//      physics->insert2[i].unk18 = 0.0;
//      physics->insert2[i].unk110 = 0.0;
//      physics->insert2[i].unk118 = 0.0;
//      physics->insert2[i].unk11C = 0.0;
//      physics->insert2[i].unk120 = 0.0;
//      physics->insert2[i].unk150 = INFINITY;
//      physics->insert2[i].unk154 = INFINITY;
//      physics->insert2[i].unk158 = INFINITY;
//      physics->insert2[i].unk15C = 0.0;
//      physics->insert2[i].unk160 = 0.0;
//    }
//
//    physics->unkAC = 0xFFFFFFFF;
//    physics->wheelsNeedUpdate = 0;
//    physics->unkC40 = 0.0;
//  } else {
//    vehicleWheeledPhysics_SomethingWheelRayTrace_Original(physics, index);
//  }
//}


REGISTER_FLIGHT_HOOK(void __fastcall, vehicleWheeledPhysics_Update, RED4ext::vehicle::WheeledPhysics *physics) {
  auto fc = FlightComponent::Get(physics->parent);
  if (fc && fc->active) {
    //physics->isMoving = true;
  }
  vehicleWheeledPhysics_Update_Original(physics);
  //if (fc && fc->active) {
  //  physics->isMoving = 1;
  //  physics->unkAC = -1;
  //  physics->unkB0 = 0.0;
  //  physics->unkA4 = -INFINITY;
  //}
}

 //disables tire torque & friction
REGISTER_FLIGHT_HOOK(void __fastcall, RollFactorTorqueThing, 
    uint64_t *a1, float a2) {
  auto physics = (RED4ext::vehicle::WheeledPhysics *)a1[2];
  auto fc = FlightComponent::Get(physics->parent);
  if (fc && fc->active) {
    return;
  } else {
    RollFactorTorqueThing_Original(a1, a2);
  }
}

REGISTER_FLIGHT_HOOK(void __fastcall, FourWheelTorque,
    RED4ext::vehicle::WheeledPhysics *physics, unsigned __int8 rearWheelIndex, unsigned __int8 frontWheelIndex, float a4, RED4ext::Transform *transform) {
  auto fc = FlightComponent::Get(physics->parent);
  if (fc && fc->active) {
    vehicle::SpeedUnlimiter::PhysicsStructUpdate(physics->parent->physicsData);
  } else {
    FourWheelTorque_Original(physics, rearWheelIndex, frontWheelIndex, a4, transform);
  }
}

//REGISTER_FLIGHT_HOOK(void __fastcall, vehicleUnk588_UpdateTransform, RED4ext::vehicle::Unk588 * unk588, RED4ext::Transform * transform) {
//  ;
//  vehicleUnk588_UpdateTransform_Original(unk588, transform);
//  ;
//}