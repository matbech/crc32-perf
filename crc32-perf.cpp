// crcperf.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "slice16.h"
#include "pcl.h"
#include "zlib.h"
#include "adler32.h"

#include <cstdint>
#include <cstdio>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <sys/time.h>
#endif

#define ZTIME_USEC_PER_SEC 1000000
#define ZTIME_MSEC_PER_SEC 1000
#define MEGABYTE (1024*1024)

/* ztime_t represents usec */
typedef uint64_t ztime_t;

static void ztime(ztime_t *ztimep)
{
#ifdef _WIN32
	*ztimep = GetTickCount64() * 1000;
#else
	struct timeval tv;

	gettimeofday(&tv, NULL);

	*ztimep = ((uint64_t) tv.tv_sec * ZTIME_USEC_PER_SEC) + tv.tv_usec;
#endif
}

template<typename Func>
void run(size_t bufferSize, Func&& func)
{
	auto buffer = new uint8_t[bufferSize];
	// fill buffer
	for (size_t i = 0; i < bufferSize; ++i)
	{
		buffer[i] = static_cast<uint8_t>(i);
	}

	ztime_t start;
	ztime(&start);

	uint32_t crcValue;

	// 10M iterations
	static const size_t iterations = 10000000;
	for (size_t i = 0; i < iterations; i++)
	{
		crcValue = func(buffer, bufferSize, 0);
	}

	ztime_t stop;
	ztime(&stop);
	const auto timeInMs = (uint32_t) ((stop - start) / ZTIME_MSEC_PER_SEC);

	fprintf(stdout, "buffer size %lu, checksum 0x%x, time %ums, rate %u MB/s\n",
		static_cast<unsigned long>(bufferSize),
		crcValue,
		timeInMs,
		(uint32_t) ((((bufferSize * iterations) / timeInMs) * 1000) / MEGABYTE)
	);

	delete [] buffer;
}

int main()
{
	static const size_t bufferSizes []{ 32, 64, 128, 256, 384, 512, 1024, 8 * 1024 };
	for (auto bufferSize : bufferSizes)
	{
		// just included for reference
		fprintf(stdout, "adler32_zlib\n");
		run(bufferSize, adler32_zlib);

		// zlib crc32 implementation is splice by 4. With the updated implementation of crc32_little, you should see the same performance 
		// as with crc32_4bytes.
		fprintf(stdout, "crc32_zlib\n");
		run(bufferSize, crc32_zlib);

		fprintf(stdout, "crc32_4bytes\n");
		run(bufferSize, crc32_4bytes);

		fprintf(stdout, "crc32_8bytes\n");
		run(bufferSize, crc32_8bytes);

		fprintf(stdout, "crc32_16bytes\n");
		run(bufferSize, crc32_16bytes);

		// crc32_pcl starts to get significantly faster for buffersize >= 512
		fprintf(stdout, "crc32_pcl\n");
		run(bufferSize, crc32_pcl);
	}
    return 0;
}

