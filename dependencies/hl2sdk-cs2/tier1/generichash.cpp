//======= Copyright © 2005, , Valve Corporation, All rights reserved. =========
//
// Purpose: Variant Pearson Hash general purpose hashing algorithm described
//			by Cargill in C++ Report 1994.  Generates a 16-bit result.
//
//=============================================================================

#include <stdlib.h>
#include "tier0/basetypes.h"
#include "tier0/dbg.h"
#include "tier0/platform.h"
#include "tier0/strtools.h"
#include "generichash.h"

// NOTE: This has to be the last file included!
#include "tier0/memdbgon.h"

#define TOLOWERU( c ) ( ( uint32 ) ( ( ( c >= 'A' ) && ( c <= 'Z' ) )? c + 32 : c ) )

//-----------------------------------------------------------------------------
//
// Table of randomly shuffled values from 0-255 generated by:
//
//-----------------------------------------------------------------------------
/*
void MakeRandomValues()
{
	int i, j, r;
	unsigned  t;
	srand( 0xdeadbeef );

	for ( i = 0; i < 256; i++ )
	{
		g_nRandomValues[i] = (unsigned )i;
	}

	for (j = 0; j < 8; j++)
	{
		for (i = 0; i < 256; i++)
		{
			r = rand() & 0xff;
			t = g_nRandomValues[i];
			g_nRandomValues[i] = g_nRandomValues[r];
			g_nRandomValues[r] = t;
		}
	}

	printf("static unsigned g_nRandomValues[256] =\n{\n");

	for (i = 0; i < 256; i += 16)
	{
		printf("\t");
		for (j = 0; j < 16; j++)
			printf(" %3d,", g_nRandomValues[i+j]);
		printf("\n");
	}
	printf("};\n");
}
*/

static unsigned g_nRandomValues[256] =
{
	238,	164,	191,	168,	115,	 16,	142,	 11,	213,	214,	 57,	151,	248,	252,	 26,	198,
	 13,	105,	102,	 25,	 43,	 42,	227,	107,	210,	251,	 86,	 66,	 83,	193,	126,	108,
	131,	  3,	 64,	186,	192,	 81,	 37,	158,	 39,	244,	 14,	254,	 75,	 30,	  2,	 88,
	172,	176,	255,	 69,	  0,	 45,	116,	139,	 23,	 65,	183,	148,	 33,	 46,	203,	 20,
	143,	205,	 60,	197,	118,	  9,	171,	 51,	233,	135,	220,	 49,	 71,	184,	 82,	109,
	 36,	161,	169,	150,	 63,	 96,	173,	125,	113,	 67,	224,	 78,	232,	215,	 35,	219,
	 79,	181,	 41,	229,	149,	153,	111,	217,	 21,	 72,	120,	163,	133,	 40,	122,	140,
	208,	231,	211,	200,	160,	182,	104,	110,	178,	237,	 15,	101,	 27,	 50,	 24,	189,
	177,	130,	187,	 92,	253,	136,	100,	212,	 19,	174,	 70,	 22,	170,	206,	162,	 74,
	247,	  5,	 47,	 32,	179,	117,	132,	195,	124,	123,	245,	128,	236,	223,	 12,	 84,
	 54,	218,	146,	228,	157,	 94,	106,	 31,	 17,	 29,	194,	 34,	 56,	134,	239,	246,
	241,	216,	127,	 98,	  7,	204,	154,	152,	209,	188,	 48,	 61,	 87,	 97,	225,	 85,
	 90,	167,	155,	112,	145,	114,	141,	 93,	250,	  4,	201,	156,	 38,	 89,	226,	196,
	  1,	235,	 44,	180,	159,	121,	119,	166,	190,	144,	 10,	 91,	 76,	230,	221,	 80,
	207,	 55,	 58,	 53,	175,	  8,	  6,	 52,	 68,	242,	 18,	222,	103,	249,	147,	129,
	138,	243,	 28,	185,	 62,	 59,	240,	202,	234,	 99,	 77,	 73,	199,	137,	 95,	165,
};

//-----------------------------------------------------------------------------
// String 
//-----------------------------------------------------------------------------
unsigned FASTCALL HashString( const char *pszKey )
{
	return MurmurHash2( pszKey, (uint32)V_strlen( pszKey ), 0x3501A674 );
}


//-----------------------------------------------------------------------------
// Case-insensitive string 
//-----------------------------------------------------------------------------
unsigned FASTCALL HashStringCaseless( const char *pszKey )
{
	return MurmurHash2LowerCase( pszKey, 0x3501A674 );
}

//-----------------------------------------------------------------------------
// 32 bit conventional case-insensitive string 
//-----------------------------------------------------------------------------
unsigned FASTCALL HashStringFastCaselessConventional( const char *pszKey )
{
	uint32 hash = 0xAAAAAAAA; // Alternating 1's and 0's to maximize the effect of the later multiply and add
	hash += ( 2 * (uint32)V_strlen( pszKey ) ); // Add the string length * 2 to the hash to give it more variety

	for( ; *pszKey ; pszKey++ )
	{
		hash = ( ( hash << 5 ) + hash ) + (uint8)TOLOWERU(*pszKey);
	}

	return hash;
}

//-----------------------------------------------------------------------------
// 32 bit conventional case-sensitive string 
//-----------------------------------------------------------------------------
unsigned FASTCALL HashStringConventional( const char *pszKey )
{
	unsigned hash = 0xAAAAAAAA; // Alternating 1's and 0's to maximize the effect of the later multiply and add

	for(; *pszKey; pszKey++)
	{
		hash = ((hash << 5) + hash) + (uint8)*pszKey;
	}

	return hash;
}

//-----------------------------------------------------------------------------
// int hash
//-----------------------------------------------------------------------------
unsigned FASTCALL HashInt( const int n )
{
	unsigned		even, odd;
	odd   = g_nRandomValues[(((unsigned)n >> 8) & 0xff)];
	even  = g_nRandomValues[odd ^ ((unsigned)n >> 24)];
	odd   = g_nRandomValues[even ^ ((unsigned)n >> 16) & 0xff];
	even  = g_nRandomValues[odd ^ ((unsigned)n >> 8) & 0xff];
	odd   = g_nRandomValues[even  ^ ((unsigned)n & 0xff)];

	return (even << 8) | odd;
}

//-----------------------------------------------------------------------------
// 4-byte hash
//-----------------------------------------------------------------------------
unsigned FASTCALL Hash4( const void *pKey )
{
	const uint32 *	p = (const uint32 *) pKey;
	unsigned		even,
							odd,
							n;
	n     = *p;
	odd   = g_nRandomValues[((n >> 8) & 0xff)];
	even  = g_nRandomValues[odd ^ (n >> 24)];
	odd   = g_nRandomValues[even ^ (n >> 16) & 0xff];
	even  = g_nRandomValues[odd ^ (n >> 8) & 0xff];
	odd   = g_nRandomValues[even  ^ (n & 0xff)];

	return (even << 8) | odd;
}


//-----------------------------------------------------------------------------
// 8-byte hash
//-----------------------------------------------------------------------------
unsigned FASTCALL Hash8( const void *pKey )
{
	const uint32 *	p = (const uint32 *) pKey;
	unsigned		even,
							odd,
							n;
	n     = *p;
	odd   = g_nRandomValues[((n >> 8) & 0xff)];
	even  = g_nRandomValues[odd ^ (n >> 24)];
	odd   = g_nRandomValues[even ^ (n >> 16) & 0xff];
	even  = g_nRandomValues[odd ^ (n >> 8) &  0xff];
	odd   = g_nRandomValues[even  ^ (n & 0xff)];

	n     = *(p+1);
	even  = g_nRandomValues[odd ^ (n >> 24)];
	odd   = g_nRandomValues[even  ^ (n >> 16) & 0xff];
	even  = g_nRandomValues[odd ^ (n >> 8) & 0xff];
	odd   = g_nRandomValues[even  ^ (n & 0xff)];

	return (even << 8) | odd;
}


//-----------------------------------------------------------------------------
// 12-byte hash
//-----------------------------------------------------------------------------
unsigned FASTCALL Hash12( const void *pKey )
{
	const uint32 *	p = (const uint32 *) pKey;
	unsigned		even,
							odd,
							n;
	n     = *p;
	odd   = g_nRandomValues[((n >> 8) & 0xff)];

	even  = g_nRandomValues[odd ^ (n >> 24)];
	odd   = g_nRandomValues[even ^ (n >> 16) & 0xff];
	even  = g_nRandomValues[odd ^ (n >> 8) &  0xff];
	odd   = g_nRandomValues[even  ^ (n & 0xff)];

	n     = *(p+1);
	even  = g_nRandomValues[odd ^ (n >> 24)];
	odd   = g_nRandomValues[even  ^ (n >> 16) & 0xff];
	even  = g_nRandomValues[odd ^ (n >> 8) &  0xff];
	odd   = g_nRandomValues[even  ^ (n & 0xff)];

	n     = *(p+2);
	even  = g_nRandomValues[odd ^ (n >> 24)];
	odd   = g_nRandomValues[even  ^ (n >> 16) & 0xff];
	even  = g_nRandomValues[odd ^ (n >> 8) &  0xff];
	odd   = g_nRandomValues[even  ^ (n & 0xff)];

	return (even << 8) | odd;
}


//-----------------------------------------------------------------------------
// 16-byte hash
//-----------------------------------------------------------------------------
unsigned FASTCALL Hash16( const void *pKey )
{
	const uint32 *	p = (const uint32 *) pKey;
	unsigned		even,
							odd,
							n;
	n     = *p;
	odd   = g_nRandomValues[((n >> 8) & 0xff)];

	even  = g_nRandomValues[odd ^ (n >> 24)];
	odd   = g_nRandomValues[even ^ (n >> 16) & 0xff];
	even  = g_nRandomValues[odd ^ (n >> 8) &  0xff];
	odd   = g_nRandomValues[even  ^ (n & 0xff)];

	n     = *(p+1);
	even  = g_nRandomValues[odd ^ (n >> 24)];
	odd   = g_nRandomValues[even  ^ (n >> 16) & 0xff];
	even  = g_nRandomValues[odd ^ (n >> 8) & 0xff];
	odd   = g_nRandomValues[even  ^ (n & 0xff)];

	n     = *(p+2);
	even  = g_nRandomValues[odd ^ (n >> 24)];
	odd   = g_nRandomValues[even  ^ (n >> 16) & 0xff];
	even  = g_nRandomValues[odd ^ (n >> 8) &  0xff];
	odd   = g_nRandomValues[even  ^ (n & 0xff)];

	n     = *(p+3);
	even  = g_nRandomValues[odd ^ (n >> 24)];
	odd   = g_nRandomValues[even  ^ (n >> 16) & 0xff];
	even  = g_nRandomValues[odd ^ (n >> 8) &  0xff];
	odd   = g_nRandomValues[even  ^ (n & 0xff)];

	return (even << 8) | odd;
}


//-----------------------------------------------------------------------------
// Arbitrary fixed length hash
//-----------------------------------------------------------------------------
unsigned FASTCALL HashBlock( const void *pKey, unsigned size )
{
	const uint8 *	k    = (const uint8 *) pKey;
	unsigned 		even = 0,
					odd  = 0,
					n;

	while (size)
	{
		--size;
		n    = *k++;
		even = g_nRandomValues[odd ^ n];
		if (size)
		{
			--size;
			n   = *k++;
			odd = g_nRandomValues[even ^ n];
		}
		else
			break;
	}

	return (even << 8) | odd;
}


//-----------------------------------------------------------------------------
// Murmur hash
//-----------------------------------------------------------------------------
uint32 MurmurHash2( const void * key, int len, uint32 seed )
{
	// They're not really 'magic', they just happen to work well.

	const uint32 m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	uint32 h = seed ^ len;

	// Mix 4 bytes at a time into the hash

	const unsigned char * data = (const unsigned char *)key;

	while(len >= 4)
	{
		uint32 k = LittleDWord( *(uint32 *)data );

		k *= m; 
		k ^= k >> r; 
		k *= m; 

		h *= m; 
		h ^= k;

		data += 4;
		len -= 4;
	}

	// Handle the last few bytes of the input array

	switch(len)
	{
	case 3: h ^= data[2] << 16;
	case 2: h ^= data[1] << 8;
	case 1: h ^= data[0];
		h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

uint32 MurmurHash2LowerCase( char const *pString, int len, uint32 nSeed )
{
	char *p = ( char * ) stackalloc( len + 1 );
	for( int i = 0; i < len ; i++ )
	{
		p[i] = TOLOWERU( pString[i] );
	}
	return MurmurHash2(p, len, nSeed ); 
}

uint32 MurmurHash2LowerCase( char const *pString, uint32 nSeed )
{
	return MurmurHash2LowerCase( pString, (uint32)V_strlen(pString), nSeed );
}

//-----------------------------------------------------------------------------
// Murmur hash, 64 bit- endian neutral
//-----------------------------------------------------------------------------
uint64 MurmurHash64( const void * key, int len, uint32 seed )
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const uint32 m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	uint32 h1 = seed ^ len;
	uint32 h2 = 0;

	// Mix 4 bytes at a time into the hash

	const uint32 * data = (const uint32 *)key;
	while ( len >= 8 )
	{
		uint32 k1 = LittleDWord( *data++ );
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;

		uint32 k2 = LittleDWord( *data++ );
		k2 *= m; k2 ^= k2 >> r; k2 *= m;
		h2 *= m; h2 ^= k2;
		len -= 4;
	}

	if(len >= 4)
	{
		uint32 k1 = LittleDWord( *data++ );
		k1 *= m; k1 ^= k1 >> r; k1 *= m;
		h1 *= m; h1 ^= k1;
		len -= 4;
	}

	// Handle the last few bytes of the input array
	switch(len)
	{
	case 3: h2 ^= ((uint8*)data)[2] << 16;
	case 2: h2 ^= ((uint8*)data)[1] << 8;
	case 1: h2 ^= ((uint8*)data)[0];
			h2 *= m;
	};

	h1 ^= h2 >> 18; h1 *= m;
	h2 ^= h1 >> 22; h2 *= m;
	h1 ^= h2 >> 17; h1 *= m;
	h2 ^= h1 >> 19; h2 *= m;

	uint64 h = h1;

	h = (h << 32) | h2;

	return h;
}
