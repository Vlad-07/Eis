#include <Eispch.h>
#include "Client.h"

#include <chrono>


namespace Eis
{
	static Client* s_ClientInstance = nullptr;


	Client::Client() : m_Running(false)
	{
		EIS_CORE_ASSERT(!s_ClientInstance, "Client already exists!");
		s_ClientInstance = this;
	}
	Client::~Client()
	{
		m_Running = false;
		if (m_NetworkThread.joinable())
			m_NetworkThread.join();
		s_ClientInstance = nullptr;
	}


	void Client::ConnectToServer(const std::string& address)
	{
		if (m_Running)
			return;

		if (m_NetworkThread.joinable())
			m_NetworkThread.join();

		m_ServerAddr = address;
		m_NetworkThread = std::thread([this]() { NetworkThreadFunc(); });
	}
	void Client::Disconnect()
	{
		Shutdown();

		if (m_NetworkThread.joinable())
			m_NetworkThread.join();
	}


	void Client::NetworkThreadFunc()
	{
		m_ConnectionStatus = Connecting;

		// Startup

		SteamDatagramErrMsg errMsg;
		if (!GameNetworkingSockets_Init(nullptr, errMsg))
		{
			OnFatalError(fmt::format("GameNetworkingSockets_Init failed: {}", errMsg));
			m_ConnectionStatus = ConnectionFailed;
			return;
		}

		m_Interface = SteamNetworkingSockets();

		SteamNetworkingIPAddr addr;
		if (!addr.ParseString(m_ServerAddr.c_str()))
		{
			OnFatalError(fmt::format("Invalid IP address - could not parse {}", m_ServerAddr));
			m_ConnectionDebugMessage = "Invalid IP address";
			m_ConnectionStatus = ConnectionFailed;
			return;
		}

		SteamNetworkingConfigValue_t options;
		options.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, (void*)ConnectionStatusChangedCallback);
		m_Connection = m_Interface->ConnectByIPAddress(addr, 1, &options);
		if (m_Connection == k_HSteamNetConnection_Invalid)
		{
			m_ConnectionDebugMessage = "Failed to connect";
			m_ConnectionStatus = ConnectionFailed;
			return;
		}

		m_Running = true;

		while (m_Running)
		{
			PollIncomingMessages();
			PollConnectionStateChanges();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		m_Interface->CloseConnection(m_Connection, 0, nullptr, false);
		m_ConnectionStatus = Disconnected;

		GameNetworkingSockets_Kill();
		m_Interface = nullptr;
	}
	void Client::Shutdown()
	{
		m_Running = false;
	}

	void Client::ConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* info) { s_ClientInstance->OnConnectionStatusChanged(info); }
	void Client::OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info)
	{
		switch (info->m_info.m_eState)
		{
		case k_ESteamNetworkingConnectionState_None:
			// NOTE: We will get callbacks here when we destroy connections. You can ignore these.
			break;

		case k_ESteamNetworkingConnectionState_ClosedByPeer:
		case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
			m_Running = false;
			m_ConnectionStatus = Disconnected;
			m_ConnectionDebugMessage = info->m_info.m_szEndDebug;

			if (info->m_eOldState == k_ESteamNetworkingConnectionState_Connecting)
			{
				// should distinguish between a timeout, a rejected connection, or some other transport problem
				m_ConnectionStatus = ConnectionFailed;
				EIS_CORE_WARN("Could not connect to remote host. {0}", m_ConnectionDebugMessage);
			}
			else if (info->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally)
			{
				EIS_CORE_WARN("Lost connection with remote host. {0}", m_ConnectionDebugMessage);
			}
			else
			{
				// should check reason code for a normal disconnection
				EIS_CORE_INFO("Disconnected from host. {0}", m_ConnectionDebugMessage);
			}

			m_Interface->CloseConnection(m_Connection, 0, nullptr, false);
			m_Connection = k_HSteamNetConnection_Invalid;
			
			break;

		case k_ESteamNetworkingConnectionState_Connecting:
			// We will get this callback when we start connecting. We can ignore this.
			break;

		case k_ESteamNetworkingConnectionState_Connected:
			m_ConnectionStatus = Connected;
			m_ServerConnectedCallback();
			break;

		default:
			break;
		}
	}

	void Client::PollIncomingMessages()
	{
		while (m_Running)
		{
			ISteamNetworkingMessage* incomingMessage = nullptr;
			int messageCount = m_Interface->ReceiveMessagesOnConnection(m_Connection, &incomingMessage, 1);
			if (messageCount == 0)
				return;
			if (messageCount < 0)
			{
				m_Running = false;
				EIS_CORE_CRITICAL("Failed to receive messages");
			}

			m_DataReceivedCallback(Buffer(incomingMessage->m_pData, incomingMessage->m_cbSize));

			incomingMessage->Release();
		}
	}
	void Client::PollConnectionStateChanges()
	{
		m_Interface->RunCallbacks();
	}


	void Client::SendBuffer(const Buffer& buffer, bool reliable)
	{
		m_Interface->SendMessageToConnection(m_Connection, buffer.Data(), (uint32_t)buffer.GetSize(),
			reliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable, nullptr);
	}
	void Client::SendString(const std::string& string, bool reliable)
	{
		SendBuffer(Buffer(string.data(), string.size()), reliable);
	}


	void Client::SetDataReceivedCallback(const DataReceivedCallback& function)
	{
		m_DataReceivedCallback = function;
	}
	void Client::SetServerConnectedCallback(const ServerConnectedCallback& function)
	{
		m_ServerConnectedCallback = function;
	}
	void Client::SetServerDisconnectedCallback(const ServerDisconnectedCallback& function)
	{
		m_ServerDisconnectedCallback = function;
	}

	void Client::OnFatalError(const std::string& message)
	{
		EIS_CORE_CRITICAL(message);
		m_Running = false;
	}
}