/***********************************************************************************
	Created:	17:9:2002
	FileName: 	hdrloader.cpp
	Author:		Igor Kravtchenko
	
	Info:		Load HDR image and convert to a set of float32 RGB triplet.
************************************************************************************/

#include "hdrloader.h"

#include <cmath>
#include <memory.h>
#include <cstdio>
#include <fstream>

typedef unsigned char RGBE[4];
constexpr int         R{ 0 };
constexpr int         G{ 1 };
constexpr int         B{ 2 };
constexpr int         E{ 3 };

constexpr int MINELEN{ 8 };                // minimum scanline length for encoding
constexpr int MAXELEN{ 0x7fff };            // maximum scanline length for encoding

static void workOnRGBE( RGBE* scan, int len, float* cols );
static bool decrunch( RGBE* scanline, int len, std::ifstream& file );
static bool oldDecrunch( RGBE* scanline, int len, std::ifstream& file );

bool HDRLoader::load( const char* fileName, HDRLoaderResult& res )
{
    int           i;
    char          str[200];
    std::ifstream file;
    
    file.open( fileName, std::ios::in | std::ios::binary );
    if ( !file )
    {
        return false;
    }
    
    file.read( str, 10 );
    if ( memcmp( str, "#?RADIANCE", 10 ) != 0 )
    {
        file.close();
        return false;
    }
    file.seekg( 1, std::ios::cur );
    
    char cmd[200];
    i = 0;
    char c = 0, oldc;
    while ( true )
    {
        oldc = c;
        file.get( c );
        if ( c == 0xa && oldc == 0xa )
        {
            break;
        }
        cmd[i++] = c;
    }
    
    char reso[200];
    i = 0;
    while ( true )
    {
        file.get( c );
        reso[i++] = c;
        if ( c == 0xa )
        {
            break;
        }
    }
    
    int w, h;
    if ( !sscanf_s( reso, "-Y %ld +X %ld", &h, &w ))
    {
        file.close();
        return false;
    }
    
    res.width  = w;
    res.height = h;
    
    if ( !w || !h )
    {
        file.close();
        return false;
    }
    
    auto* cols = new float[w * h * 3];
    res.cols = cols;
    
    RGBE* scanline = new RGBE[w];
    
    // convert image
    for ( int y = h - 1; y >= 0; y-- )
    {
        if ( !decrunch( scanline, w, file ))
        {
            break;
        }
        workOnRGBE( scanline, w, cols );
        cols += w * 3;
    }
    
    delete[] scanline;
    file.close();
    return true;
}

float convertComponent( int expo, int val )
{
    const float v = (float) val / 256.0f;
    const float d = powf( 2.0f, (float) expo );
    return v * d;
}

void workOnRGBE( RGBE* scan, int len, float* cols )
{
    while ( len-- > 0 )
    {
        int expo = scan[0][E] - 128;
        cols[0] = convertComponent( expo, scan[0][R] );
        cols[1] = convertComponent( expo, scan[0][G] );
        cols[2] = convertComponent( expo, scan[0][B] );
        cols += 3;
        scan++;
    }
}

bool decrunch( RGBE* scanline, int len, std::ifstream& file )
{
    int i, j;
    
    if ( len < MINELEN || len > MAXELEN )
    {
        return oldDecrunch( scanline, len, file );
    }
    
    file.get( reinterpret_cast<char&>(i));
    if ( i != 2 )
    {
        file.seekg( -1, std::ios::cur );
        return oldDecrunch( scanline, len, file );
    }
    
    file.get( reinterpret_cast<char&>(scanline[0][G]));
    file.get( reinterpret_cast<char&>(scanline[0][B]));
    file.get( reinterpret_cast<char&>(i));
    
    if ( scanline[0][G] != 2 || scanline[0][B] & 128 )
    {
        scanline[0][R] = 2;
        scanline[0][E] = (unsigned char) i;
        return oldDecrunch( scanline + 1, len - 1, file );
    }
    
    // read each component
    for ( i = 0; i < 4; i++ )
    {
        for ( j = 0; j < len; )
        {
            unsigned char code;
            file.get( reinterpret_cast<char&>(code));
            if ( code > 128 )
            { // run
                code &= 127;
                unsigned char val;
                file.get( reinterpret_cast<char&>(val));
                while ( code-- )
                {
                    scanline[j++][i] = val;
                }
            }
            else
            {    // non-run
                while ( code-- )
                {
                    file.get( reinterpret_cast<char&>(scanline[j++][i]));
                }
            }
        }
    }
    
    return !file.eof();
}

bool oldDecrunch( RGBE* scanline, int len, std::ifstream& file )
{
    int i;
    int rshift = 0;
    
    while ( len > 0 )
    {
        file.get( reinterpret_cast<char&>(scanline[0][R]));
        file.get( reinterpret_cast<char&>(scanline[0][G]));
        file.get( reinterpret_cast<char&>(scanline[0][B]));
        file.get( reinterpret_cast<char&>(scanline[0][E]));
        if ( file.eof())
        {
            return false;
        }
        
        if ( scanline[0][R] == 1 && scanline[0][G] == 1 && scanline[0][B] == 1 )
        {
            for ( i = scanline[0][E] << rshift; i > 0; i-- )
            {
                memcpy( &scanline[0][0], &scanline[-1][0], 4 );
                scanline++;
                len--;
            }
            rshift += 8;
        }
        else
        {
            scanline++;
            len--;
            rshift = 0;
        }
    }
    return true;
}