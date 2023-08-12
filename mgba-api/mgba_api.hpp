#pragma once
#include <cstdint>
#include <string>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

class mgba_api
{
public:
	mgba_api()
	{
		this->m_port = 61337;

		this->m_client_fd = {};

		this->m_server_fd = {};
	}

	~mgba_api()
	{
		closesocket( this->m_client_fd );

		closesocket( this->m_server_fd );

		WSACleanup();
	}

	bool init_socket();

	bool wait_for_connection();

	std::string recv_message();

	void send_message( const std::string& msg );

	bool release_socket();

	uint8_t  read8( const std::uintptr_t address );
	uint16_t read16( const std::uintptr_t address );
	uint32_t read32( const std::uintptr_t address );

	bool write8( const std::uintptr_t address, const uint8_t value );
	bool write16( const std::uintptr_t address, const uint16_t value );
	bool write32( const std::uintptr_t address, const uint32_t value );

private:
	uint32_t m_port;

	SOCKET m_client_fd;
	SOCKET m_server_fd;
};
