#include <Internal/DoremiEngineImplementation.hpp>
#include <Subsystem/EngineModuleEnum.hpp>

#include <DoremiEngine/Audio/Include/AudioModule.hpp>
#include <DoremiEngine/Physics/Include/PhysicsModule.hpp>
#include <DoremiEngine/Graphic/Include/GraphicModule.hpp>
#include <DoremiEngine/Network/Include/NetworkModule.hpp>
#include <DoremiEngine/Input/Include/InputModule.hpp>
#include <DoremiEngine/AI/Include/AIModule.hpp>
#include <Utility/DynamicLoader/Include/DynamicLoader.hpp>

#include <Internal/SharedContextImplementation.hpp>
#include <Windows.h>

// Logging
#include <Utility/DebugLog/Include/ConsoleManager.hpp>

namespace DoremiEngine
{
    namespace Core
    {
        DoremiEngineImplementation::DoremiEngineImplementation()
            : m_audioLibrary(nullptr),
              m_graphicLibrary(nullptr),
              m_networkLibrary(nullptr),
              m_physicsLibrary(nullptr),
              m_inputLibrary(nullptr),
              m_audioModule(nullptr),
              m_graphicModule(nullptr),
              m_networkModule(nullptr),
              m_physicsModule(nullptr),
              m_inputModule(nullptr),
              m_aiModule(nullptr),
              m_logger(nullptr)
        {
            Utility::DebugLog::ConsoleManager::Startup();
            m_logger = &Utility::DebugLog::ConsoleManager::GetInstance().CreateNewConsole("engine", false);
        }

        DoremiEngineImplementation::~DoremiEngineImplementation()
        {
            if(m_audioModule != nullptr)
            {
                delete m_audioModule;
            }

            if(m_graphicModule != nullptr)
            {
                delete m_graphicModule;
            }

            if(m_networkModule != nullptr)
            {
                delete m_networkModule;
            }

            if(m_physicsModule != nullptr)
            {
                delete m_physicsModule;
            }

            if(m_audioLibrary != nullptr)
            {
                DynamicLoader::FreeSharedLibrary(m_audioLibrary);
            }
            if(m_graphicLibrary != nullptr)
            {
                DynamicLoader::FreeSharedLibrary(m_graphicLibrary);
            }
            if(m_networkLibrary != nullptr)
            {
                DynamicLoader::FreeSharedLibrary(m_networkLibrary);
            }
            if(m_physicsLibrary != nullptr)
            {
                DynamicLoader::FreeSharedLibrary(m_physicsLibrary);
            }
            if(m_aiLibrary != nullptr)
            {
                DynamicLoader::FreeSharedLibrary(m_aiLibrary);
            }
        }

        SharedContext& DoremiEngineImplementation::Start(const size_t& p_flags)
        {
            m_sharedContext = new SharedContextImplementation();
            m_sharedContext->SetCoreModule(this);
            BuildWorkingDirectory(*m_sharedContext);

            if((p_flags & EngineModuleEnum::AUDIO) == EngineModuleEnum::AUDIO)
            {
                LoadAudioModule(*m_sharedContext);
            }

            if((p_flags & EngineModuleEnum::GRAPHIC) == EngineModuleEnum::GRAPHIC)
            {
                LoadGraphicModule(*m_sharedContext);
            }

            if((p_flags & EngineModuleEnum::NETWORK) == EngineModuleEnum::NETWORK)
            {
                LoadNetworkModule(*m_sharedContext);
            }

            if((p_flags & EngineModuleEnum::PHYSICS) == EngineModuleEnum::PHYSICS)
            {
                LoadPhysicsModule(*m_sharedContext);
            }

            if((p_flags & EngineModuleEnum::INPUT) == EngineModuleEnum::INPUT)
            {
                LoadInputModule(*m_sharedContext);
            }

            if((p_flags & EngineModuleEnum::AI) == EngineModuleEnum::AI)
            {
                LoadAIModule(*m_sharedContext);
            }
            return *m_sharedContext;
        }

        void DoremiEngineImplementation::Stop()
        {
            if(m_audioModule != nullptr)
            {
                m_audioModule->Shutdown();
            }

            if(m_graphicModule != nullptr)
            {
                m_graphicModule->Shutdown();
            }

            if(m_networkModule != nullptr)
            {
                m_networkModule->Shutdown();
            }

            if(m_physicsModule != nullptr)
            {
                m_physicsModule->Shutdown();
            }

            if(m_aiModule != nullptr)
            {
                m_aiModule->Shutdown();
            }
        }

        void DoremiEngineImplementation::BuildWorkingDirectory(SharedContextImplementation& o_sharedContext)
        {
            char path[254];
            GetModuleFileNameA(NULL, path, 254);
            std::string* directoryPath = new std::string(path);
            // TODOXX This only works because Client.exe and Server.exe is 10 characters long.
            o_sharedContext.SetWorkingDirectory(directoryPath->substr(0, directoryPath->length() - 10));
        }

        void DoremiEngineImplementation::LoadAudioModule(SharedContextImplementation& o_sharedContext)
        {
            using namespace Utility::DebugLog;
            m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Audio.dll");
            m_audioLibrary = DynamicLoader::LoadSharedLibrary("Audio.dll");

            if(m_audioLibrary != nullptr)
            {
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Audio.dll - Success");
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Audio.dll");

                CREATE_AUDIO_MODULE functionCreateAudioModule = (CREATE_AUDIO_MODULE)DynamicLoader::LoadProcess(m_audioLibrary, "CreateAudioModule");
                if(functionCreateAudioModule != nullptr)
                {
                    m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Audio.dll - Success");

                    m_audioModule = static_cast<Audio::AudioModule*>(functionCreateAudioModule(o_sharedContext));
                    m_audioModule->Startup();
                    o_sharedContext.SetAudioModule(m_audioModule);
                }
                else
                {
                    m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Audio.dll - Failed");
                }
            }
            else
            {
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Audio.dll - Failed");
            }
        }

        void DoremiEngineImplementation::LoadGraphicModule(SharedContextImplementation& o_sharedContext)
        {
            using namespace Utility::DebugLog;
            m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Graphic.dll");
            m_graphicLibrary = DynamicLoader::LoadSharedLibrary("Graphic.dll");

            if(m_graphicLibrary != nullptr)
            {
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Graphic.dll - Success");
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Graphic.dll");
                CREATE_GRAPHIC_MODULE functionCreateGraphicModule =
                    (CREATE_GRAPHIC_MODULE)DynamicLoader::LoadProcess(m_graphicLibrary, "CreateGraphicModule");
                if(functionCreateGraphicModule != nullptr)
                {
                    m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Graphic.dll - Success");
                    m_graphicModule = static_cast<Graphic::GraphicModule*>(functionCreateGraphicModule(o_sharedContext));
                    m_graphicModule->Startup();
                    o_sharedContext.SetGraphicModule(m_graphicModule);
                }
                else
                {
                    m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Graphic.dll - Failed");
                }
            }
            else
            {
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Graphic.dll - Failed");
            }
        }

        void DoremiEngineImplementation::LoadNetworkModule(SharedContextImplementation& o_sharedContext)
        {
            using namespace Utility::DebugLog;
            m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Network.dll");
            m_networkLibrary = DynamicLoader::LoadSharedLibrary("Network.dll");

            if(m_networkLibrary != nullptr)
            {
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Network.dll - Success");
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Network.dll");
                CREATE_NETWORK_MODULE functionCreateNetworkModule =
                    (CREATE_NETWORK_MODULE)DynamicLoader::LoadProcess(m_networkLibrary, "CreateNetworkModule");
                if(functionCreateNetworkModule != nullptr)
                {
                    m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Network.dll - Success");
                    m_networkModule = static_cast<Network::NetworkModule*>(functionCreateNetworkModule(o_sharedContext));
                    m_networkModule->Startup();
                    o_sharedContext.SetNetworkModule(m_networkModule);
                }
                else
                {
                    m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Network.dll - Failed");
                }
            }
            else
            {
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Network.dll - Failed");
            }
        }

        void DoremiEngineImplementation::LoadPhysicsModule(SharedContextImplementation& o_sharedContext)
        {
            using namespace Utility::DebugLog;
            m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Physics.dll");
            m_physicsLibrary = DynamicLoader::LoadSharedLibrary("Physics.dll");

            if(m_physicsLibrary != nullptr)
            {
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Physics.dll - Success");
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Physics.dll");
                CREATE_PHYSICS_MODULE functionCreatePhysicsModule =
                    (CREATE_PHYSICS_MODULE)DynamicLoader::LoadProcess(m_physicsLibrary, "CreatePhysicsModule");
                if(functionCreatePhysicsModule != nullptr)
                {
                    m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Physics.dll - Success");
                    m_physicsModule = static_cast<Physics::PhysicsModule*>(functionCreatePhysicsModule(o_sharedContext));
                    m_physicsModule->Startup();
                    o_sharedContext.SetPhysicsModule(m_physicsModule);
                }
                else
                {
                    m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Physics.dll - Failed");
                }
            }
            else
            {
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Physics.dll - Failed");
            }
        }

        void DoremiEngineImplementation::LoadInputModule(SharedContextImplementation& o_sharedContext)
        {
            using namespace Utility::DebugLog;
            m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Input.dll");
            m_inputLibrary = DynamicLoader::LoadSharedLibrary("Input.dll");

            if(m_inputLibrary != nullptr)
            {
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Input.dll - Success");
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Input.dll");
                CREATE_INPUT_MODULE functionCreateInputModule = (CREATE_INPUT_MODULE)DynamicLoader::LoadProcess(m_inputLibrary, "CreateInputModule");
                if(functionCreateInputModule != nullptr)
                {
                    //              m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Input.dll - Success");
                    m_inputModule = static_cast<Input::InputModule*>(functionCreateInputModule(o_sharedContext));
                    m_inputModule->Startup();
                    o_sharedContext.SetInputModule(m_inputModule);
                }
                else
                {
                    m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from Input.dll - Failed");
                }
            }
            else
            {
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading Input.dll - Failed");
            }
        }

        void DoremiEngineImplementation::LoadAIModule(SharedContextImplementation& o_sharedContext)
        {
            using namespace Utility::DebugLog;
            m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading AI.dll");
            m_aiLibrary = DynamicLoader::LoadSharedLibrary("AI.dll");

            if(m_aiLibrary != nullptr)
            {
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading AI.dll - Success");
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from AI.dll");
                CREATE_AI_MODULE functionCreateAIModule = (CREATE_AI_MODULE)DynamicLoader::LoadProcess(m_aiLibrary, "CreateAIModule");
                if(functionCreateAIModule != nullptr)
                {
                    m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from AI.dll - Success");

                    m_aiModule = static_cast<AI::AIModule*>(functionCreateAIModule(o_sharedContext));
                    m_aiModule->Startup();
                    o_sharedContext.SetAIModule(m_aiModule);
                }
                else
                {
                    m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading process from AI.dll - Failed");
                }
            }
            else
            {
                m_logger->LogText(LogTag::ENGINE_CORE, LogLevel::INFO, "Loading AI.dll - Failed");
            }
        }
    }
}

namespace
{
    DoremiEngine::Core::DoremiEngineImplementation* engine = nullptr;
}

const DoremiEngine::Core::SharedContext& StartEngine(const size_t& p_flags)
{
    if(engine == nullptr)
    {
        engine = new DoremiEngine::Core::DoremiEngineImplementation();
        engine->Start(p_flags);
    }
    return engine->GetSharedContext();
}

void StopEngine()
{
    engine->Stop();
    delete engine;
    engine = nullptr;
}