#pragma once

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
#include <steam/steam_api.h>
#endif

#include <map>
#include <string>
#include <thread>
#include <functional>

#include "Eis/Core/Buffer.h"


namespace Eis
{
	using ClientID = HSteamNetConnection;

	struct ClientInfo
	{
		ClientID Id;
		std::string ConnectionDescription;
	};

	class Server
	{
	public:
		using DataReceivedCallback = std::function<void(const ClientInfo&, Buffer&)>;
		using ClientConnectedCallback = std::function<void(const ClientInfo&)>;
		using ClientDisconnectedCallback = std::function<void(const ClientInfo&)>;

	public:
		Server(int port);
		~Server();

		void Start();
		void Stop();

		// Set callbacks for server events
		// Will be called from the network thread
		void SetDataReceivedCallback(const DataReceivedCallback& f);
		void SetClientConnectedCallback(const ClientConnectedCallback& f);
		void SetClientDisconnectedCallback(const ClientDisconnectedCallback& f);

		// ----------- Data transfer --------------------------------------------------------------------
		void SendBufferToClient(ClientID clientId, const Buffer& buffer, bool reliable = true);
		void SendBufferToAllClients(const Buffer& buffer, ClientID excludeClient = k_HSteamNetConnection_Invalid, bool reliable = true);
		
		// Strings sent are NOT null-terminated
		void SendStringToClient(ClientID clientId, const std::string& string, bool reliable = true);
		void SendStringToAllClients(const std::string& string, ClientID excludeClient = k_HSteamNetConnection_Invalid, bool reliable = true);

		template<typename T>
		void SendDataToClient(ClientID clientId, const T& data, bool reliable = true)
		{
			SendBufferToClient(clientId, Buffer(&data, sizeof(data)), reliable);
		}

		template<typename T>
		void SendDataToAllClients(const T& data, ClientID excludeClient = k_HSteamNetConnection_Invalid, bool reliable = true)
		{
			SendBufferToAllClients(Buffer(&data, sizeof(data)), excludeClient, reliable);
		}
		// ----------------------------------------------------------------------------------------------


		void KickClient(ClientID clientId);

		bool IsRunning() const { return m_Running; }

		void SetPort(int port) { if (!m_Running) m_Port = port; }

	private:
		void NetworkThreadFunc();

		static void ConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* info);
		void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);

		void PollIncomingMessages();
		void PollConnectionStateChanges();

		void SetClientNick(HSteamNetConnection conn, const char* nick);

		void OnFatalError(const std::string& message);

	private:
		std::thread m_NetworkThread;
		DataReceivedCallback m_DataRecievedCallback;
		ClientConnectedCallback m_ClientConnectedCallback;
		ClientDisconnectedCallback m_ClientDisconnectedCallback;

		int m_Port;
		bool m_Running;
		std::map<HSteamNetConnection, ClientInfo> m_ConnectedClients;

		ISteamNetworkingSockets* m_Interface = nullptr;
		HSteamListenSocket m_ListenSocket = k_HSteamListenSocket_Invalid;
		HSteamNetPollGroup m_PollGroup = k_HSteamNetPollGroup_Invalid;
	};
}