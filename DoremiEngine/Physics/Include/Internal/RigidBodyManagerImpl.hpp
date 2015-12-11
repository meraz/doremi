#pragma once
#include <RigidBodyManager.hpp>
#include <unordered_map>
#include <DirectXMath.h>
#include <PhysX/PxPhysicsAPI.h>
using namespace physx;
using namespace std;
using namespace DirectX;
namespace DoremiEngine
{
    namespace Physics
    {
        struct InternalPhysicsUtils;
        class RigidBodyManagerImpl : public RigidBodyManager
        {
            public:
            RigidBodyManagerImpl(InternalPhysicsUtils& p_utils);
            virtual ~RigidBodyManagerImpl();

            int AddBoxBodyDynamic(XMFLOAT3 p_position, XMFLOAT4 p_orientation, XMFLOAT3 p_dims, int p_materialID) override;
            void AddForceToBody(int p_bodyID, XMFLOAT3 p_force) override;
            XMFLOAT3 GetBodyPosition(int p_bodyID) override;

            private:
            InternalPhysicsUtils& m_utils;
            unordered_map<int, PxRigidBody*> m_bodies;
            int m_nextBody;
        };
    }
}