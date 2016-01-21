#pragma once

#include <DoremiEngine.hpp>
#include <Internal/SharedContextImplementation.hpp>


namespace Utility
{
    namespace DebugLog
    {
        class VirtualConsole;
    }
}


namespace DoremiEngine
{
    namespace Core
    {
        class DoremiEngineImplementation : public DoremiEngine
        {
        public:
            /**
                Constructor
            */
            DoremiEngineImplementation();

            /**
                Destructor
            */
            virtual ~DoremiEngineImplementation();

            /**
                Start the engine and all modules of interest
            */
            SharedContext& Start(const size_t&);

            /**
                Stops the engine and all modules
            */
            void Stop();


            /**
                TODO docs
            */
            const SharedContext& GetSharedContext() const override { return *static_cast<SharedContext*>(m_sharedContext); }

        private:
            void BuildWorkingDirectory(SharedContextImplementation& o_sharedContext);

            // Loading .dll
            void LoadAudioModule(SharedContextImplementation& o_sharedContext);
            void LoadGraphicModule(SharedContextImplementation& o_sharedContext);
            void LoadNetworkModule(SharedContextImplementation& o_sharedContext);
            void LoadPhysicsModule(SharedContextImplementation& o_sharedContext);
            void LoadInputModule(SharedContextImplementation& o_sharedContext);
            void LoadAIModule(SharedContextImplementation& o_sharedContext);

            // Shared context
            SharedContextImplementation* m_sharedContext;

            // Pointers to .dll processes
            void* m_audioLibrary;
            void* m_graphicLibrary;
            void* m_networkLibrary;
            void* m_physicsLibrary;
            void* m_inputLibrary;
            void* m_aiLibrary;

            // Pointers to the interfaces
            Audio::AudioModule* m_audioModule;
            Graphic::GraphicModule* m_graphicModule;
            Network::NetworkModule* m_networkModule;
            Physics::PhysicsModule* m_physicsModule;
            Input::InputModule* m_inputModule;
            AI::AIModule* m_aiModule;

            // Logging
            Utility::DebugLog::VirtualConsole* m_logger;
        };
    }
}
