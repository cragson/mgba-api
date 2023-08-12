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

	[[nodiscard]] bool init_socket();

	[[nodiscard]] bool wait_for_connection();

	[[nodiscard]] std::string recv_message() const;

	void send_message( const std::string& msg ) const;

	[[nodiscard]] bool release_socket();

	[[nodiscard]] uint8_t  read8( const std::uintptr_t address ) const;
	[[nodiscard]] uint16_t read16( const std::uintptr_t address ) const;
	[[nodiscard]] uint32_t read32( const std::uintptr_t address ) const;

	[[nodiscard]] bool write8( const std::uintptr_t address, const uint8_t value ) const;
	[[nodiscard]] bool write16( const std::uintptr_t address, const uint16_t value ) const;
	[[nodiscard]] bool write32( const std::uintptr_t address, const uint32_t value ) const;

	[[nodiscard]] std::string get_game_title() const;
	[[nodiscard]] std::string get_game_code() const;

	[[nodiscard]] bool send_key_press( int32_t keycode ) const;
	[[nodiscard]] bool send_key_release( int32_t keycode ) const;
	[[nodiscard]] bool press_key_once( int32_t keycode, uint32_t delay = 100 ) const;

private:
	uint32_t m_port;

	SOCKET m_client_fd;
	SOCKET m_server_fd;
};
