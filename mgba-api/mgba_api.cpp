#include "mgba_api.hpp"

#include <format>
#include <iostream>
#include <ws2tcpip.h>

bool mgba_api::init_socket()
{
	WSADATA wsaData;
	if( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
	{
		printf( "[!] Could not initialize Winsock!\n" );
		return false;
	}

	this->m_server_fd = socket( AF_INET, SOCK_STREAM, 0 );
	if( this->m_server_fd == INVALID_SOCKET )
	{
		printf( "[!] Could not create the socket!\n" );
		WSACleanup();
		return false;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family      = AF_INET;
	serverAddr.sin_port        = htons( this->m_port );
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if( bind( this->m_server_fd, reinterpret_cast< sockaddr* >( &serverAddr ), sizeof( serverAddr ) ) == SOCKET_ERROR )
	{
		printf( "[!] Could not bind the socket!\n" );
		closesocket( this->m_server_fd );
		WSACleanup();
		return false;
	}

	return true;
}


bool mgba_api::wait_for_connection()
{
	if( listen( this->m_server_fd, SOMAXCONN ) == SOCKET_ERROR )
	{
		printf( "[!] Could not open the socket!\n" );
		closesocket( this->m_server_fd );
		WSACleanup();
		return false;
	}

	printf( "[#] Waiting for incoming connection..\n" );

	sockaddr_in clientAddr;
	int         clientAddrSize = sizeof( clientAddr );

	this->m_client_fd = accept( this->m_server_fd, reinterpret_cast< sockaddr* >( &clientAddr ), &clientAddrSize );
	if( this->m_client_fd == INVALID_SOCKET )
	{
		printf( "[!] Could not establish connection to client!\n" );
		closesocket( this->m_server_fd );
		WSACleanup();
		return false;
	}

	char clientIP[ INET_ADDRSTRLEN ];
	inet_ntop( AF_INET, &clientAddr.sin_addr, clientIP, sizeof( clientIP ) );

	printf( "[+] Established connection to client: %s\n", clientIP );

	return true;
}

std::string mgba_api::recv_message() const
{
	char       buffer[ 1024 ];
	const auto bytesReceived = recv( this->m_client_fd, buffer, sizeof( buffer ), 0 );

	if( bytesReceived > 0 )
		buffer[ bytesReceived ] = '\0';

	return { buffer };
}

void mgba_api::send_message( const std::string& msg ) const
{
	send( this->m_client_fd, msg.c_str(), msg.size(), 0 );
}


bool mgba_api::release_socket()
{
	closesocket( this->m_client_fd );
	closesocket( this->m_server_fd );
	WSACleanup();

	return true;
}

uint8_t mgba_api::read8( const std::uintptr_t address ) const
{
	this->send_message( std::vformat( "READ8 {:x}", std::make_format_args( address ) ) );

	const auto value = this->recv_message();

	return std::atoi( value.c_str() );
}

uint16_t mgba_api::read16( const std::uintptr_t address ) const
{
	this->send_message( std::vformat( "READ16 {:x}", std::make_format_args( address ) ) );

	const auto value = this->recv_message();

	return std::atoi( value.c_str() );
}

uint32_t mgba_api::read32( const std::uintptr_t address ) const
{
	this->send_message( std::vformat( "READ32 {:x}", std::make_format_args( address ) ) );

	const auto value = this->recv_message();

	return std::atoi( value.c_str() );
}

bool mgba_api::write8( const std::uintptr_t address, const uint8_t value ) const
{
	this->send_message( std::vformat( "WRITE8 {:x} {}", std::make_format_args( address, value ) ) );

	return this->recv_message() == "OK";
}

bool mgba_api::write16( const std::uintptr_t address, const uint16_t value ) const
{
	this->send_message( std::vformat( "WRITE16 {:x} {}", std::make_format_args( address, value ) ) );

	return this->recv_message() == "OK";
}

bool mgba_api::write32( const std::uintptr_t address, const uint32_t value ) const
{
	this->send_message( std::vformat( "WRITE32 {:x} {}", std::make_format_args( address, value ) ) );

	return this->recv_message() == "OK";
}

std::string mgba_api::get_game_title() const
{
	this->send_message( "GET_GAME_TITLE" );

	return this->recv_message();
}

std::string mgba_api::get_game_code() const
{
	this->send_message( "GET_GAME_CODE" );

	return this->recv_message();
}

bool mgba_api::send_key_press( int32_t keycode ) const
{
	this->send_message( std::vformat( "PRESS_KEY {}", std::make_format_args( keycode ) ) );

	return this->recv_message() == "OK";
}

bool mgba_api::send_key_release( int32_t keycode ) const
{
	this->send_message( std::vformat( "RELEASE_KEY {}", std::make_format_args( keycode ) ) );

	return this->recv_message() == "OK";
}

bool mgba_api::press_key_once( int32_t keycode, uint32_t delay ) const
{
	if( !this->send_key_press( keycode ) )
		return false;

	Sleep( delay );

	return this->send_key_release( keycode );
}
