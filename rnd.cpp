// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*

rnd
Copyright (C) 2024 Nikolay Raspopov <raspopov@cherubicsoft.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <random>

using namespace std::chrono;

const auto usage =
	"rnd - The utility for the creation and verification of files filled with a pseudo-random sequence of bytes.\n"
	"\n"
	"Copyright (C) 2024 Nikolay Raspopov <raspopov@cherubicsoft.com>\n"
	"https://github.com/raspopov/rnd\n"
	"\n"
	"Usage: rnd (\"create\"|\"verify\") file_name [ file_size (=0 - infinite) [K|M|G] [ pseudo-random_seed_number (=1) ] ]\n";

unsigned char data[ 4 * 1024 ];
unsigned char read[ sizeof( data ) ];

volatile bool stop = false;

void SigIntHandler(int)
{
	stop = true;
}

int main(int argc, char * argv[])
{
	// Ctrl + С handler
	std::signal( SIGINT, &SigIntHandler );

	FILE * f = nullptr;
	auto time_begin = steady_clock::now();
	size_t size_begin = 0;
	size_t pos = 0;

	if ( argc > 2 )
	{
		size_t size = 0;
		if ( argc > 3 )
		{
			size = atoll( argv[ 3 ] );
			switch ( argv[ 3 ][ strlen( argv[ 3 ] ) - 1 ] )
			{
			case 'k':
			case 'K':
				size <<= 10;
				break;

			case 'm':
			case 'M':
				size <<= 20;
				break;

			case 'g':
			case 'G':
				size <<= 30;
				break;
			}
		}

		unsigned int seed = 1;
		if ( argc > 4 )
		{
			seed = atoi( argv[ 4 ] );
		}

		std::mt19937 rnd( seed );
		std::uniform_int_distribution<> dist( 0, 255 );

		if ( _stricmp( argv[ 1 ], "create" ) == 0 )
		{
			if ( fopen_s( &f, argv[ 2 ], "wbS" ) == 0 )
			{
				while ( ! stop && ( ! size || pos < size ) )
				{
					const auto len = size ? std::min( size - pos, sizeof( data ) ) : sizeof( data );

					for ( auto & j : data )
					{
						j = static_cast< char >( dist( rnd ) );
					}

					const auto was_write = fwrite( &data, 1, len, f );

					pos += was_write;
					if ( was_write != len )
					{
						break;
					}

					if ( ( pos & 0x7fffff ) == 0 ) // every 8 MB
					{
						fflush( f );

						const auto time_end = steady_clock::now();
						const auto elapsed = duration_cast< milliseconds >( time_end - time_begin ).count();
						time_begin = time_end;
						printf( "\rWriting %zu MB (%zu MB/s)   ", pos >> 20, elapsed ? ( ( ( pos - size_begin ) * 1000u ) / elapsed ) >> 20 : 0u );
						size_begin = pos;
					}
				}

				printf( "\rWrite stop at file position %zu (0x%08zX)\n", pos, pos );

				fclose( f );

				return 0;
			}
			else
			{
				printf( "Failed to create file: %d\n", errno );

				return 2;
			}
		}
		else if ( _stricmp( argv[ 1 ], "verify" ) == 0 )
		{
			if ( fopen_s( &f, argv[ 2 ], "rbS" ) == 0 )
			{
				while ( ! stop && ( ! size || pos < size ) )
				{
					const auto len = size ? std::min( size - pos, sizeof( data ) ) : sizeof( data );

					for ( auto & j : data )
					{
						j = static_cast< char >( dist( rnd ) );
					}

					const auto was_read = fread( &read, 1, len, f );

					if ( was_read && memcmp( read, data, was_read ) != 0 )
					{
						size_t j = 0;
						for ( ; j < was_read && read[ j ] == data[ j ]; ++j );

						printf( "\rVerify error at file position %zu (0x%08zX): read %u (0x%02X), expected %u (0x%02X)\n",
							pos + j, pos + j, read[ j ], read[ j ], data[ j ], data[ j ] );

						fclose( f );

						return 3;
					}

					pos += was_read;
					if ( was_read != len )
					{
						break;
					}

					if ( ( pos & 0x7fffff ) == 0 ) // every 8 MB
					{
						const auto time_end = steady_clock::now();
						const auto elapsed = duration_cast< milliseconds >( time_end - time_begin ).count();
						time_begin = time_end;
						printf( "\rReading %zu MB (%zu MB/s)   ", pos >> 20, elapsed ? ( ( ( pos - size_begin ) * 1000u ) / elapsed ) >> 20 : 0u );
						size_begin = pos;
					}
				}

				printf( "\rVerify complete at file position %zu (0x%08zX)\n", pos, pos );

				fclose( f );

				return 0;
			}
			else
			{
				printf( "Failed to open file: %d\n", errno );

				return 2;
			}
		}
	}

	puts( usage );

	return 1;
}
