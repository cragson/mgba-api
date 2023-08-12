#include <iostream>

#include "mgba_api.hpp"

int main()
{
	const auto api = std::make_unique< mgba_api >();

	if (!api->init_socket())
	{
		printf("[!] Could not initialize the socket!\n");

		return 1;
	}

	while (!api->wait_for_connection())
		Sleep(420);

	printf("[+] Client connected!\n");

	// ...

	if(!api->release_socket())
	{
		printf("[!] Could not release the socket!\n");

		return 1;
	}

	return 0;
}
