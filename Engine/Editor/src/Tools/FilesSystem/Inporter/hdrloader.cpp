/***********************************************************************************
	Created:	17:9:2002
	FileName: 	hdrloader.cpp
	Author:		Igor Kravtchenko
	
	Info:		Load HDR image and Convert to a set of float32 RGB triplet.
************************************************************************************/

#include "hdrloader.h"

#include <cmath>
#include <memory.h>
#include <cstdio>

typedef unsigned char RGBE[4];
#define R            0
#define G            1
#define B            2
#define E            3

#define  MINELEN    8                // minimum scanline length for encoding
#define  MAXELEN    0x7fff            // maximum scanline length for encoding

static void workOnRGBE( RGBE* scan, int len, float* cols );
static bool decrunch( RGBE* scanline, int len, FILE* file );
static bool oldDecrunch( RGBE* scanline, int len, FILE* file );

bool HDRLoader::load( const char* fileName, HDRLoaderResult& res )
{
    int  i;
    char str[200];
    FILE* file;
    
    fopen_s( &file, fileName, "rb" );
    if ( !file )
    {
        return false;
    }
    
    fread( str, 10, 1, file );
    if ( memcmp( str, "#?RADIANCE", 10 ) != 0 )
    {
        fclose( file );
        return false;
    }
    
    fseek( file, 1, SEEK_CUR );
    
    char cmd[200];
    i = 0;
    char c = 0, oldc;
    while ( true )
    {
        oldc = c;
        c    = (char)fgetc( file );
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
        c = (char)fgetc( file );
        reso[i++] = c;
        if ( c == 0xa )
        {
            break;
        }
    }
    
    int w, h;
    if ( !sscanf_s( reso, "-Y %ld +X %ld", &h, &w ))
    {
        fclose( file );
        return false;
    }
    
    res.width  = w;
    res.height = h;
    
    float* cols = new float[w * h * 3];
    res.cols = cols;
    
    RGBE* scanline = new RGBE[w];
    
    // Convert image
    for ( int y = h - 1; y >= 0; y-- )
    {
        if ( !decrunch( scanline, w, file ) )
        {
            break;
        }
        workOnRGBE( scanline, w, cols );
        cols += w * 3;
    }
    
    delete[] scanline;
    fclose( file );
    
    return true;
}

float convertComponent( int expo, int val )
{
    float v = float (val) / 256.0f;
    float d = (float) pow( 2, expo );
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

bool decrunch( RGBE* scanline, int len, FILE* file )
{
    int i, j;
    
    if ( len < MINELEN || len > MAXELEN )
    {
        return oldDecrunch( scanline, len, file );
    }
    
    i = fgetc( file );
    if ( i != 2 )
    {
        fseek( file, -1, SEEK_CUR );
        return oldDecrunch( scanline, len, file );
    }
    
    scanline[0][G] = (unsigned char)fgetc( file );
    scanline[0][B] = (unsigned char)fgetc( file );
    i = fgetc( file );
    
    if ( scanline[0][G] != 2 || scanline[0][B] & 128 )
    {
        scanline[0][R] = 2;
        scanline[0][E] = (unsigned char)i;
        return oldDecrunch( scanline + 1, len - 1, file );
    }
    
    // read each component
    for ( i = 0; i < 4; i++ )
    {
        for ( j = 0; j < len; )
        {
            unsigned char code = (unsigned char)fgetc( file );
            if ( code > 128 )
            { // run
                code &= 127;
                unsigned char val = (unsigned char)fgetc( file );
                while ( code-- )
                {
                    scanline[j++][i] = val;
                }
            }
            else
            {    // non-run
                while ( code-- )
                {
                    scanline[j++][i] = (unsigned char)fgetc( file );
                }
            }
        }
    }
    
    return feof( file ) ? false : true;
}

bool oldDecrunch( RGBE* scanline, int len, FILE* file )
{
    int i;
    int rshift = 0;
    
    while ( len > 0 )
    {
        scanline[0][R] = (unsigned char)fgetc( file );
        scanline[0][G] = (unsigned char)fgetc( file );
        scanline[0][B] = (unsigned char)fgetc( file );
        scanline[0][E] = (unsigned char)fgetc( file );
        if ( feof( file ))
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