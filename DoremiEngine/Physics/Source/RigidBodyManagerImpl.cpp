#include <Internal/RigidBodyManagerImpl.hpp>
#include <Internal/PhysicsModuleImplementation.hpp>
namespace DoremiEngine
{
    namespace Physics
    {
        RigidBodyManagerImpl::RigidBodyManagerImpl(InternalPhysicsUtils& p_utils) : m_utils(p_utils)
        {
            m_nextBody = 0;
            m_nextStaticBody = 0;
        }
        RigidBodyManagerImpl::~RigidBodyManagerImpl() {}

        int RigidBodyManagerImpl::AddBoxBodyDynamic(XMFLOAT3 p_position, XMFLOAT4 p_orientation, XMFLOAT3 p_dims, int p_materialID)
        {
            // Create a world matrix (only translation) i think
            PxVec3 position = PxVec3(p_position.x, p_position.y, p_position.z);
            PxQuat orientation = PxQuat(p_orientation.x, p_orientation.y, p_orientation.z, p_orientation.w);
            PxVec3 dims = PxVec3(p_dims.x, p_dims.y, p_dims.z);
            // Creates the actual body.
            PxTransform transform = PxTransform(position, orientation);
            // This body is dynamic
            PxRigidDynamic* body = m_utils.m_physics->createRigidDynamic(transform);
            // Create a shape for the body
            body->createShape(PxBoxGeometry(dims), *m_utils.m_physicsMaterialManager->GetMaterial(p_materialID));
            // Give the body some mass (since it is dynamic. Static objects probably don't need mass)
            PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
            // Add the now fully created body to the scene
            m_utils.m_worldScene->addActor(*body);

            // Finally add the body to our list
            m_bodies[m_nextBody] = body;
            m_nextBody++;
            /*
            And now we have added a box to the world at the given position
            I'm not too sure how we update the box, or the scene, or perform
            any form of interaction however*/

            // Return ID of body we just created
            return m_nextBody - 1;
        }

        int RigidBodyManagerImpl::AddBoxBodyStatic(XMFLOAT3 p_position, XMFLOAT4 p_orientation, XMFLOAT3 p_dims, int p_materialID)
        {
            // Create a world matrix (only translation) i think
            PxVec3 position = PxVec3(p_position.x, p_position.y, p_position.z);
            PxQuat orientation = PxQuat(p_orientation.x, p_orientation.y, p_orientation.z, p_orientation.w);
            PxVec3 dims = PxVec3(p_dims.x, p_dims.y, p_dims.z);
            // Creates the physics object shape thingy, which collides with stuff. Shapes are just objects. I
            PxMaterial* material = m_utils.m_physicsMaterialManager->GetMaterial(p_materialID);
            PxShape* shape = m_utils.m_physics->createShape(PxBoxGeometry(dims), *m_utils.m_physicsMaterialManager->GetMaterial(p_materialID));
            // Creates the actual body.
            PxTransform transform = PxTransform(position, orientation);
            // This body is static
            PxTransform trans = PxTransform(PxVec3(0, 0, 0), PxQuat(0, 0, 0, 1));
            PxRigidStatic* body = m_utils.m_physics->createRigidStatic(transform);

            // Attach shape to the body
            body->attachShape(*shape);

            // Add the now fully created body to the scene
            m_utils.m_worldScene->addActor(*body);
            // We're done with the shape. Release it
            shape->release();

            // Finally add the body to our list
            m_bodies[m_nextBody] = body;
            m_nextBody++;

            /*
            And now we have added a box to the world at the given position
            I'm not too sure how we update the box, or the scene, or perform
            any form of interaction however*/

            // Return ID of body we just created
            return m_nextBody - 1;
        }

        void RigidBodyManagerImpl::SetCallback(int p_bodyID, int p_filterGroup, int p_filterMask)
        {
            PxFilterData filterData;
            filterData.word0 = p_filterGroup; // Own ID
            filterData.word1 = p_filterMask; // ID mask to filter pairs that trigger contact callback
            PxRigidActor* actor = m_bodies[p_bodyID];
            int numShapes = actor->getNbShapes();
            // Magic allocation of memory (i think)
            PxShape** shapes = (PxShape**)m_utils.m_allocator.allocate(sizeof(PxShape*) * numShapes, 0, __FILE__, __LINE__);
            actor->getShapes(shapes, numShapes);
            for(size_t i = 0; i < numShapes; i++)
            {
                PxShape* shape = shapes[i];
                shape->setSimulationFilterData(filterData);
            }
            if(shapes)
            {
                m_utils.m_allocator.deallocate(shapes);
                shapes = NULL;
            }
        }

        void RigidBodyManagerImpl::AddForceToBody(int p_bodyID, XMFLOAT3 p_force)
        {
            if(m_bodies[p_bodyID]->isRigidDynamic()) ((PxRigidDynamic*)m_bodies[p_bodyID])->addForce(PxVec3(p_force.x, p_force.y, p_force.z));
        }

        void RigidBodyManagerImpl::AddTorqueToBody(int p_bodyID, XMFLOAT3 p_torque)
        {
            if(m_bodies[p_bodyID]->isRigidDynamic()) ((PxRigidDynamic*)m_bodies[p_bodyID])->addTorque(PxVec3(p_torque.x, p_torque.y, p_torque.z));
        }

        void RigidBodyManagerImpl::SetBodyVelocity(int p_bodyID, XMFLOAT3 p_v)
        {
            /*((PxRigidDynamic*)m_bodies[p_bodyID])->setAngularVelocity(PxVec3(p_v.x, p_v.y, p_v.z));*/
            ((PxRigidDynamic*)m_bodies[p_bodyID])->setLinearVelocity(PxVec3(p_v.x, p_v.y, p_v.z));
        }

        void RigidBodyManagerImpl::SetBodyAngularVelocity(int p_bodyID, XMFLOAT3 p_v)
        {
            ((PxRigidDynamic*)m_bodies[p_bodyID])->setAngularVelocity(PxVec3(p_v.x, p_v.y, p_v.z));
        }

        void RigidBodyManagerImpl::SetBodyPosition(int p_bodyID, XMFLOAT3 p_v, XMFLOAT4 p_o)
        {
            PxVec3 position = PxVec3(p_v.x, p_v.y, p_v.z);
            PxQuat orientation = PxQuat(p_o.x, p_o.y, p_o.z, p_o.w);
            PxTransform trans;
            trans.p = position;
            trans.q = orientation;
            m_bodies[p_bodyID]->setGlobalPose(trans);
            PxVec3 pos = m_bodies[p_bodyID]->getGlobalPose().p;
        }

        void RigidBodyManagerImpl::SetLinearDampening(int p_bodyID, float p_dampening)
        {
            ((PxRigidDynamic*)m_bodies[p_bodyID])->setLinearDamping(p_dampening);
        }

        XMFLOAT3 RigidBodyManagerImpl::GetBodyPosition(int p_bodyID)
        {
            PxVec3 p = m_bodies[p_bodyID]->getGlobalPose().p;
            return XMFLOAT3(p.x, p.y, p.z);
        }

        XMFLOAT4 RigidBodyManagerImpl::GetBodyOrientation(int p_bodyID)
        {

            PxQuat q = m_bodies[p_bodyID]->getGlobalPose().q;
            return XMFLOAT4(q.x, q.y, q.z, q.w);
        }

        XMFLOAT3 RigidBodyManagerImpl::GetBodyVelocity(int p_bodyID)
        {
            PxVec3 v = ((PxRigidDynamic*)m_bodies[p_bodyID])->getLinearVelocity();
            return XMFLOAT3(v.x, v.y, v.z);
        }

        XMFLOAT3 RigidBodyManagerImpl::GetBodyAngularVelocity(int p_bodyID)
        {
            PxVec3 v = ((PxRigidDynamic*)m_bodies[p_bodyID])->getAngularVelocity();
            return XMFLOAT3(v.x, v.y, v.z);
        }

        float RigidBodyManagerImpl::GetLinearDampening(int p_bodyID) { return ((PxRigidDynamic*)m_bodies[p_bodyID])->getLinearDamping(); }

        bool RigidBodyManagerImpl::IsSleeping(int p_bodyID)
        {
            bool isSleeping = ((PxRigidDynamic*)m_bodies[p_bodyID])->isSleeping();
            return isSleeping;
        }

        void RigidBodyManagerImpl::RemoveBody(int p_bodyID)
        {
            m_bodies[p_bodyID]->release();
            m_bodies.erase(p_bodyID);
        }
    }
}