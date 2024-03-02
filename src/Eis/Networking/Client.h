#pragma once

#include <steam/steamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#ifndef STEAMNETWORKINGSOCKETS_OPENSOURCE
#include <steam/steam_api.h>
#endif

#include <string>
#include <thread>
#include <functional>

#include "Eis/Core/Buffer.h"


namespace Eis
{
	class Client
	{
	public:
		enum ConnectionStatus
		{
			Disconnected = 0, Connecting, Connected, ConnectionFailed
		};

		using DataReceivedCallback = std::function<void(Buffer&)>;
		using ServerConnectedCallback = std::function<void()>;
		using ServerDisconnectedCallback = std::function<void()>;

	public:
		Client();
		~Client();

		void ConnectToServer(const std::string& address);
		void Disconnect();

		// Set callbacks for server events
		// Will be called from the network thread
		void SetDataReceivedCallback(const DataReceivedCallback& f);
		void SetServerConnectedCallback(const ServerConnectedCallback& f);
		void SetServerDisconnectedCallback(const ServerDisconnectedCallback& f);

		// ----------- Data transfer --------------------------------------------------------------------
		void SendBuffer(const Buffer& buffer, bool reliable = true);
		void SendString(const std::string& string, bool reliable = true);

		template<typename T>
		void SendData(const T& data, bool reliable = true)
		{
			SendBuffer(Buffer(&data, sizeof(data)), reliable);
		}
		// ----------------------------------------------------------------------------------------------


		bool IsRunning() const { return m_Running; }
		ConnectionStatus GetConnectionStatus() const { return m_ConnectionStatus; }
		const std::string& GetConnectionDebugMessage() const { return m_ConnectionDebugMessage; }

	private:
		void NetworkThreadFunc();
		void Shutdown();

		static void ConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* info);
		void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);

		void PollIncomingMessages();
		void PollConnectionStateChanges();

		void OnFatalError(const std::string& message);

	private:
		std::thread m_NetworkThread;
		DataReceivedCallback m_DataReceivedCallback;
		ServerConnectedCallback m_ServerConnectedCallback;
		ServerDisconnectedCallback m_ServerDisconnectedCallback;

		ConnectionStatus m_ConnectionStatus = Disconnected;
		std::string m_ConnectionDebugMessage;

		std::string m_ServerAddr;
		bool m_Running;

		ISteamNetworkingSockets* m_Interface = nullptr;
		HSteamNetConnection m_Connection = k_HSteamNetConnection_Invalid;
	};
}