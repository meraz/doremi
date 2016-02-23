#include <Doremi/Core/Include/Network/NetworkConnectionsServer.hpp>

// Engine
#include <DoremiEngine/Core/Include/SharedContext.hpp>

// Network
#include <DoremiEngine/Network/Include/NetworkModule.hpp>
#include <DoremiEngine/Network/Include/Adress.hpp>

namespace Doremi
{
    namespace Core
    {
        NetworkConnectionsServer* NetworkConnectionsServer::m_singleton = nullptr;

        NetworkConnectionsServer* NetworkConnectionsServer::GetInstance()
        {
            if(m_singleton == nullptr)
            {
                std::runtime_error("GetInstance was called before StartupConnectionsServer.");
            }
            return m_singleton;
        }

        void NetworkConnectionsServer::StartupConnectionsServer(const DoremiEngine::Core::SharedContext& p_sharedContext)
        {
            if(m_singleton != nullptr)
            {
                std::runtime_error("StartupConnectionsServer was called multiple times.");
            }
            m_singleton = new NetworkConnectionsServer(p_sharedContext);
        }

        NetworkConnectionsServer::NetworkConnectionsServer(const DoremiEngine::Core::SharedContext& p_sharedContext) {}

        NetworkConnectionsServer::~NetworkConnectionsServer() {}

        void NetworkConnectionsServer::CreateNewConnecting(const DoremiEngine::Network::Adress& p_adress)
        {
            // Create new connection and initialize values( could do this in constructor but nice to see)
            ClientConnectionFromServer* t_newConnection = new ClientConnectionFromServer();

            // First thing we do is version checking
            t_newConnection->ConnectionState = ClientConnectionStateFromServer::VERSION_CHECK;
            t_newConnection->LastResponse = 0;
            t_newConnection->NewConnection = true; // TODOCM evalueate if still needed

            // Copy adress
            DoremiEngine::Network::Adress* t_newAdress = m_sharedContext.GetNetworkModule().CreateAdress(p_adress);

            // Add connection to map
            m_connectingClientConnections[t_newAdress] = t_newConnection;
        }

        void NetworkConnectionsServer::RemoveConnection(DoremiEngine::Network::Adress& p_adress)
        {
            // Try to remove from both maps
            m_connectedClientConnections.erase(&p_adress);
            m_connectingClientConnections.erase(&p_adress);
        }

        bool NetworkConnectionsServer::AdressWithPortExist(const DoremiEngine::Network::Adress& p_adress, ClientConnectionFromServer*& o_connection)
        {
            // Check all connectd connections
            for(auto& t_connection : m_connectedClientConnections)
            {
                // Custom check cause of adress interface ( check AdressImplementation )
                // Check if we have same IP and Port
                if(*(t_connection.first) == p_adress)
                {
                    o_connection = t_connection.second;
                    return true;
                }
            }

            // TODOCM evaluate if we need to check for connected connections
            // Check all connecting connections
            for(auto& t_connection : m_connectingClientConnections)
            {
                // Custom check cause of adress interface ( check AdressImplementation )
                // Check if we have same IP and Port
                if(*(t_connection.first) == p_adress)
                {
                    o_connection = t_connection.second;
                    return true;
                }
            }

            return false;
        }

        bool NetworkConnectionsServer::AdressExistInConnecting(const DoremiEngine::Network::Adress& p_adress, ClientConnectionFromServer*& o_connection)
        {
            // TODOCM evaluate if we need to check for connected connections
            // Check all connecting connections
            for(auto& t_connection : m_connectingClientConnections)
            {
                // Custom check cause of adress interface ( check AdressImplementation )
                // Check if we have same IP
                if(*(t_connection.first) *= p_adress)
                {
                    o_connection = t_connection.second;
                    return true;
                }
            }

            return false;
        }
    }
}