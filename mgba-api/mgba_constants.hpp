#pragma once
#include <cstdint>

enum CHECKSUM : int32_t
{
	CRC32 = 0
};

enum GBA_KEY : int32_t
{
	GBA_A = 0,
	GBA_B,
	GBA_SELECT,
	GBA_START,
	GBA_RIGHT,
	GBA_LEFT,
	GBA_UP,
	GBA_DOWN,
	GBA_R,
	GBA_L
};

enum GB_KEY : int32_t
{
	GB_A = 0,
	GB_B,
	GB_SELECT,
	GB_START,
	GB_RIGHT,
	GB_LEFT,
	GB_UP,
	GB_DOWN
};

enum PLATFORM : int32_t
{
	NONE = -1,
	GBA,
	GB
};

enum SAVESTATE : int32_t
{
	SCREENSHOT = 1,
	SAVEDATA = 2,
	CHEATS = 4,
	RTC = 8,
	METADATA = 16,
	ALL = 31
};
