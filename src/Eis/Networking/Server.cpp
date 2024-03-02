#include <Eispch.h>
#include "Server.h"

#include <chrono>

namespace Eis
{
	static Server* s_ServerInstance = nullptr;


	Server::Server(int port) : m_Port(port), m_Running(false) {}
	Server::~Server()
	{
		if (m_NetworkThread.joinable())
			m_NetworkThread.join();
	}


	void Server::Start()
	{
		if (m_Running)
			return;

		m_NetworkThread = std::thread([this]() { NetworkThreadFunc(); });
	}
	void Server::Stop()
	{
		m_Running = false;
	}


	void Server::NetworkThreadFunc()
	{
		EIS_CORE_ASSERT(!s_ServerInstance, "Server already exists!");
		s_ServerInstance = this;

		// Startup

		SteamDatagramErrMsg errMsg;
		if (!GameNetworkingSockets_Init(nullptr, errMsg))
		{
			OnFatalError(fmt::format("GameNetworkingSockets_Init failed: {}", errMsg));
			return;
		}

		m_Interface = SteamNetworkingSockets();

		SteamNetworkingIPAddr serverLocalAddr;
		serverLocalAddr.Clear();
		serverLocalAddr.m_port = m_Port;

		SteamNetworkingConfigValue_t options;
		options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)Server::ConnectionStatusChangedCallback);

		m_ListenSocket = m_Interface->CreateListenSocketIP(serverLocalAddr, 1, &options);
		if (m_ListenSocket == k_HSteamListenSocket_Invalid)
		{
			OnFatalError(fmt::format("Fatal error: Failed to listen on port {}", m_Port));
			return;
		}

		m_PollGroup = m_Interface->CreatePollGroup();
		if (m_PollGroup == k_HSteamNetPollGroup_Invalid)
		{
			OnFatalError(fmt::format("Fatal error: Failed to listen on port {}", m_Port));
			return;
		}

		EIS_CORE_INFO("Server listening on port {0}", m_Port);

		m_Running = true;

		// Run loop
		while (m_Running)
		{
			PollIncomingMessages();
			PollConnectionStateChanges();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		// Cleanup
		EIS_CORE_INFO("Closing connections...");
		for (const auto& [clientId, clientInfo] : m_ConnectedClients)
		{
			m_Interface->CloseConnection(clientId, 0, "Server Shutdown", true);
		}

		m_ConnectedClients.clear();

		m_Interface->CloseListenSocket(m_ListenSocket);
		m_ListenSocket = k_HSteamListenSocket_Invalid;

		m_Interface->DestroyPollGroup(m_PollGroup);
		m_PollGroup = k_HSteamNetPollGroup_Invalid;

		GameNetworkingSockets_Kill();
		m_Interface = nullptr;
	}

	void Server::ConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* info) { s_ServerInstance->OnConnectionStatusChanged(info); }
	void Server::OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info)
	{
		switch (info->m_info.m_eState)
		{
		case k_ESteamNetworkingConnectionState_None:
			// We will get callbacks here when we destroy connections.  You can ignore these.
			break;

		// close existing connection
		case k_ESteamNetworkingConnectionState_ClosedByPeer:
		case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
		{
			auto clientIt = m_ConnectedClients.find(info->m_hConn);
			// EIS_ASSERT(cielntIt != m_ConnectedClients.end(), "...")
			m_ClientDisconnectedCallback(clientIt->second); // TODO: should communicate ClosedByPeer or ProblemDetectedLocally

			if (info->m_eOldState == k_ESteamNetworkingConnectionState_Connected)
				m_ConnectedClients.erase(clientIt);
			//else;// TODO: check if info->m_eOldState == k_ESteamNetworkingConnectionState_Connecting

			m_Interface->CloseConnection(info->m_hConn, 0, nullptr, false);
			break;
		}

		// New connection
		case k_ESteamNetworkingConnectionState_Connecting:
		{
			// Try to accept
			if (m_Interface->AcceptConnection(info->m_hConn) != k_EResultOK)
			{
				m_Interface->CloseConnection(info->m_hConn, 0, nullptr, false);
				EIS_CORE_WARN("Could not accept connection");
				break;
			}

			// Assign to poll group
			if (!m_Interface->SetConnectionPollGroup(info->m_hConn, m_PollGroup))
			{
				m_Interface->CloseConnection(info->m_hConn, 0, nullptr, false);
				EIS_CORE_WARN("Could not assign connection to poll group");
				break;
			}

			SteamNetConnectionInfo_t connectionInfo;
			m_Interface->GetConnectionInfo(info->m_hConn, &connectionInfo);

			ClientInfo& client = m_ConnectedClients[info->m_hConn];
			client.Id = (ClientID)info->m_hConn;
			client.ConnectionDescription = connectionInfo.m_szConnectionDescription;

			m_ClientConnectedCallback(client);
			break;
		}

		case k_ESteamNetworkingConnectionState_Connected:
			// We will get a callback immediately after accepting the connection.
			// Since we are the server, we can ignore this, it's not news to us.
			break;

		default:
			break;
		}
	}

	void Server::PollIncomingMessages()
	{
		while (m_Running)
		{
			ISteamNetworkingMessage* incomingMessage = nullptr;
			int messageCount = m_Interface->ReceiveMessagesOnPollGroup(m_PollGroup, &incomingMessage, 1);
			if (messageCount == 0)
				return;
			if (messageCount < 0)
			{
				m_Running = false;
				EIS_CORE_CRITICAL("Failed to get incoming messages");
				return;
			}

			auto clientIt = m_ConnectedClients.find(incomingMessage->m_conn);
			if (clientIt == m_ConnectedClients.end())
			{
				EIS_CORE_ERROR("Recieved data from unregistered client");
				continue;
			}

			if (incomingMessage->m_cbSize)
				m_DataRecievedCallback(clientIt->second, Buffer(incomingMessage->m_pData, incomingMessage->m_cbSize));

			incomingMessage->Release();
		}
	}
	void Server::PollConnectionStateChanges()
	{
		m_Interface->RunCallbacks();
	}


	void Server::SendBufferToClient(ClientID clientId, const Buffer& buffer, bool reliable)
	{
		m_Interface->SendMessageToConnection(clientId, buffer.Data(), (uint32_t)buffer.GetSize(),
			reliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable, nullptr);
	}
	void Server::SendBufferToAllClients(const Buffer& buffer, ClientID excludeClient, bool reliable)
	{
		for (const auto& [clientId, clientInfo] : m_ConnectedClients)
		{
			if (clientId != excludeClient)
				SendBufferToClient(clientId, buffer, reliable);
		}
	}

	void Server::SendStringToClient(ClientID clientId, const std::string& string, bool reliable)
	{
		SendBufferToClient(clientId, Buffer(string.data(), string.size()), reliable);
	}
	void Server::SendStringToAllClients(const std::string& string, ClientID excludeClient, bool reliable)
	{
		SendBufferToAllClients(Buffer(string.data(), string.size()), excludeClient, reliable);
	}


	void Server::SetDataReceivedCallback(const DataReceivedCallback& f)
	{
		m_DataRecievedCallback = f;
	}
	void Server::SetClientConnectedCallback(const ClientConnectedCallback& f)
	{
		m_ClientConnectedCallback = f;
	}
	void Server::SetClientDisconnectedCallback(const ClientDisconnectedCallback& f)
	{
		m_ClientDisconnectedCallback = f;
	}


	void Server::SetClientNick(HSteamNetConnection conn, const char* nick)
	{
		m_Interface->SetConnectionName(conn, nick);
	}

	void Server::KickClient(ClientID clientId)
	{
		m_Interface->CloseConnection(clientId, 0, "Kicked by server", false);
	}

	void Server::OnFatalError(const std::string& message)
	{
		EIS_CORE_CRITICAL(message);
		m_Running = false;
	}
}