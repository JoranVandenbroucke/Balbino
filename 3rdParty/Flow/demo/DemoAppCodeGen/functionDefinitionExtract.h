// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2014-2021 NVIDIA Corporation. All rights reserved.

#pragma once

struct StrHeap
{
	char* data;
	unsigned int size;
	unsigned int allocIdx;
};

StrHeap allocateStrHeap(const char** definitions, unsigned int numFunctions)
{
	unsigned int count = 0u;
	for (unsigned int functionIdx = 0u; functionIdx < numFunctions; functionIdx++)
	{
		count += (unsigned int)strlen(definitions[functionIdx]);
	}

	// offset count to cover null terminators
	count += numFunctions * (2 * maxFunctionParams + 1 + 1);

	StrHeap heap = {};
	heap.data = (char*)malloc(count);
	heap.size = count;
	heap.allocIdx = 0u;
	return heap;
}

void freeStrHeap(StrHeap* heap)
{
	free(heap->data);
	heap->data = nullptr;
	heap->size = 0u;
	heap->allocIdx = 0u;
}

inline bool isSpace(char c)
{
	return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\f' || c == '\v';
}

inline bool isAlphaNum(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_');
}

void skipSpace(unsigned int* readIdx, const char* definition)
{
	while (definition[*readIdx] != '\0')
	{
		if (isSpace(definition[*readIdx]))
		{
			*readIdx = *readIdx + 1;
		}
		else
		{
			break;
		}
	}
}

void findChar(unsigned int* readIdx, const char* definition, char c)
{
	while (definition[*readIdx] != '\0')
	{
		if (definition[*readIdx] == c)
		{
			break;
		}
		else
		{
			*readIdx = *readIdx + 1;
		}
	}
}

unsigned int findParamDelimiter(unsigned int readIdx, const char* definition)
{
	while (definition[readIdx] != '\0')
	{
		if (definition[readIdx] == ',' || definition[readIdx] == ')')
		{
			return readIdx;
		}
		else
		{
			readIdx = readIdx + 1;
		}
	}
	return readIdx;
}

unsigned int reverseFindAlphaNum(unsigned int maxReadIdx, const char* definition)
{
	int readIdx = (int)maxReadIdx;
	readIdx--;
	bool hitAlphaNum = false;
	while (readIdx >= 0)
	{
		if (isAlphaNum(definition[readIdx]))
		{
			hitAlphaNum = true;
			readIdx--;
		}
		else if (definition[readIdx] == '(' || definition[readIdx] == ',')
		{
			if (hitAlphaNum)
			{
				return (unsigned int)(readIdx + 1);
			}
			else
			{
				return maxReadIdx;
			};
		}
		else if (isSpace(definition[readIdx]))
		{
			if (hitAlphaNum)
			{
				return (unsigned int)(readIdx + 1);
			}
			else
			{
				readIdx--;
			}
		}
		else
		{
			break;
		}
	}
	return maxReadIdx;
}

void extractAlphaNum(StrHeap* heap, const char** dstPtr, unsigned int* readIdx, const char* definition)
{
	skipSpace(readIdx, definition);

	*dstPtr = heap->data + heap->allocIdx;

	while (heap->allocIdx < heap->size)
	{
		if (isAlphaNum(definition[(*readIdx)]))
		{
			heap->data[heap->allocIdx++] = definition[(*readIdx)++];
		}
		else
		{
			break;
		}
	}
	heap->data[heap->allocIdx++] = '\0';
}

void extractRegion(StrHeap* heap, const char** dstPtr, unsigned char readIdx, unsigned char maxReadIdx, const char* definition)
{
	*dstPtr = heap->data + heap->allocIdx;

	while (heap->allocIdx < heap->size)
	{
		if (readIdx < maxReadIdx)
		{
			heap->data[heap->allocIdx++] = definition[readIdx++];
		}
		else
		{
			break;
		}
	}
	heap->data[heap->allocIdx++] = '\0';
}

Function genFunction(StrHeap* heap, const char* definition)
{
	Function function = {};

	unsigned int readIdx = 0u;
	unsigned int tempReadIdx = 0u;

	unsigned int paramStartReadIdx = readIdx;
	findChar(&paramStartReadIdx, definition, '(');

	unsigned int methodReadIdx = reverseFindAlphaNum(paramStartReadIdx, definition);

	extractRegion(heap, &function.retType, readIdx, methodReadIdx, definition);

	tempReadIdx = methodReadIdx;
	extractAlphaNum(heap, &function.method, &tempReadIdx, definition);

	readIdx = paramStartReadIdx;

	function.numParams = 0;
	while (function.numParams < maxFunctionParams)
	{
		unsigned int maxReadIdx = findParamDelimiter(readIdx, definition);

		unsigned int instNameReadIdx = reverseFindAlphaNum(maxReadIdx, definition);

		// break if no name found
		if (instNameReadIdx == maxReadIdx)
		{
			break;
		}

		FunctionParams funcParams = {};

		// extract type name, found on interval readIdx to instNameReadIdx
		extractRegion(heap, &funcParams.typeName, readIdx + 1, instNameReadIdx, definition);

		// extract inst name, found on interval instNameReadIdx to maxReadIdx
		tempReadIdx = instNameReadIdx;
		extractAlphaNum(heap, &funcParams.instName, &tempReadIdx, definition);

		// advance read index
		readIdx = maxReadIdx + 1;

		function.params[function.numParams++] = funcParams;
	}

	return function;
}