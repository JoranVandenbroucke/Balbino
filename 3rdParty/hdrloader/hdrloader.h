/***********************************************************************************
	Created:	17:9:2002
	FileName: 	hdrloader.h
	Author:		Igor Kravtchenko
	
	Info:		Load HDR image and Convert to a set of float32 RGB triplet.
************************************************************************************/

#pragma once
class HDRLoaderResult final
{
public:
    int width, height;
    // each pixel takes 3 float32, each component can be of any mix_mode_value...
    float* cols;
};

class HDRLoader final
{
public:
    static bool Load( const char* fileName, HDRLoaderResult& res );
};
