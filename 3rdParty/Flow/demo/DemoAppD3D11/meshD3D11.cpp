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

//direct3d headers
#include <d3d11.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")

#include <math.h>

#include "meshD3D11.h"

#include "meshVS.hlsl.h"
#include "meshPS.hlsl.h"

namespace
{
	template <class T>
	void inline COMRelease(T& t)
	{
		if (t) t->Release();
		t = nullptr;
	}
}

struct MeshContextD3D11
{
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;

	ID3D11InputLayout* m_inputLayout = nullptr;
	ID3D11VertexShader* m_meshVS = nullptr;
	ID3D11PixelShader* m_meshPS = nullptr;
	ID3D11Buffer* m_constantBuffer = nullptr;
	ID3D11RasterizerState* m_rasterizerStateRH = nullptr;

	MeshContextD3D11() {}
	~MeshContextD3D11()
	{
		COMRelease(m_inputLayout);
		COMRelease(m_meshVS);
		COMRelease(m_meshPS);
		COMRelease(m_constantBuffer);
		COMRelease(m_rasterizerStateRH);
	}
};

inline MeshContextD3D11* cast_to_MeshContextD3D11(MeshContext* ctx)
{
	return (MeshContextD3D11*)(ctx);
}

inline MeshContext* cast_from_MeshContextD3D11(MeshContextD3D11* ctx)
{
	return (MeshContext*)(ctx);
}

struct MeshIndexBufferD3D11
{
	ID3D11Buffer* m_buffer = nullptr;
	MeshUint m_numElements = 0u;

	MeshIndexBufferD3D11() {}
	~MeshIndexBufferD3D11()
	{
		COMRelease(m_buffer);
	}
};

inline MeshIndexBufferD3D11* cast_to_MeshIndexBufferD3D11(MeshIndexBuffer* buf)
{
	return (MeshIndexBufferD3D11*)(buf);
}

inline MeshIndexBuffer* cast_from_MeshIndexBufferD3D11(MeshIndexBufferD3D11* buf)
{
	return (MeshIndexBuffer*)(buf);
}

struct MeshVertexBufferD3D11
{
	ID3D11Buffer* m_buffer = nullptr;
	MeshUint m_numElements = 0u;

	MeshVertexBufferD3D11() {}
	~MeshVertexBufferD3D11()
	{
		COMRelease(m_buffer);
	}
};

inline MeshVertexBufferD3D11* cast_to_MeshVertexBufferD3D11(MeshVertexBuffer* buf)
{
	return (MeshVertexBufferD3D11*)(buf);
}

inline MeshVertexBuffer* cast_from_MeshVertexBufferD3D11(MeshVertexBufferD3D11* buf)
{
	return (MeshVertexBuffer*)(buf);
}

MeshContext* MeshContextCreateD3D11(const MeshContextDesc* descIn)
{
	auto desc = cast_to_MeshContextDescD3D11(descIn);

	MeshContextD3D11* context = new MeshContextD3D11;

	context->m_device = desc->device;
	context->m_deviceContext = desc->context;

	// create the input layout
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		context->m_device->CreateInputLayout(inputElementDescs, 2, g_meshVS, sizeof(g_meshVS), &context->m_inputLayout);
	}

	// create the shaders
	context->m_device->CreateVertexShader(g_meshVS, sizeof(g_meshVS), nullptr, &context->m_meshVS);
	context->m_device->CreatePixelShader(g_meshPS, sizeof(g_meshPS), nullptr, &context->m_meshPS);

	// create a constant buffer
	{
		D3D11_BUFFER_DESC bufDesc;
		bufDesc.ByteWidth = 64 * sizeof(float);
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufDesc.MiscFlags = 0;

		context->m_device->CreateBuffer(&bufDesc, nullptr, &context->m_constantBuffer);
	}

	// create the rastersizer state
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = TRUE;	// This is non-default
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0.f;
		desc.SlopeScaledDepthBias = 0.f;
		desc.DepthClipEnable = TRUE;
		desc.ScissorEnable = FALSE;
		desc.MultisampleEnable = FALSE;
		desc.AntialiasedLineEnable = FALSE;

		context->m_device->CreateRasterizerState(&desc, &context->m_rasterizerStateRH);
	}

	return cast_from_MeshContextD3D11(context);
}

void MeshContextUpdateD3D11(MeshContext* contextIn, const MeshContextDesc* descIn)
{
	auto context = cast_to_MeshContextD3D11(contextIn);
	auto desc = cast_to_MeshContextDescD3D11(descIn);

	context->m_device = desc->device;
	context->m_deviceContext = desc->context;
}

void MeshContextReleaseD3D11(MeshContext* context)
{
	if (context == nullptr) return;

	delete cast_to_MeshContextD3D11(context);
}

MeshIndexBuffer* MeshIndexBufferCreateD3D11(MeshContext* contextIn, MeshUint* indices, MeshUint numIndices)
{
	auto context = cast_to_MeshContextD3D11(contextIn);

	MeshIndexBufferD3D11* buffer = new MeshIndexBufferD3D11;

	buffer->m_numElements = numIndices;
	// create an index buffer
	{
		UINT bufferSize = (UINT)(numIndices) * sizeof(UINT);

		D3D11_BUFFER_DESC bufDesc;
		bufDesc.ByteWidth = bufferSize;
		bufDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufDesc.CPUAccessFlags = DXGI_CPU_ACCESS_NONE;
		bufDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = indices;

		context->m_device->CreateBuffer(&bufDesc, &data, &buffer->m_buffer);
	}

	return cast_from_MeshIndexBufferD3D11(buffer);
}

void MeshIndexBufferReleaseD3D11(MeshIndexBuffer* buffer)
{
	if (buffer == nullptr) return;

	delete cast_to_MeshIndexBufferD3D11(buffer);
}

MeshVertexBuffer* MeshVertexBufferCreateD3D11(MeshContext* contextIn, MeshVertex* vertices, MeshUint numVertices)
{
	auto context = cast_to_MeshContextD3D11(contextIn);

	MeshVertexBufferD3D11* buffer = new MeshVertexBufferD3D11;

	buffer->m_numElements = numVertices;
	// create a vertex buffer
	{
		UINT bufferSize = (UINT)(numVertices * sizeof(MeshVertex));

		D3D11_BUFFER_DESC bufDesc;
		bufDesc.ByteWidth = bufferSize;
		bufDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufDesc.CPUAccessFlags = DXGI_CPU_ACCESS_NONE;
		bufDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = vertices;

		context->m_device->CreateBuffer(&bufDesc, &data, &buffer->m_buffer);
	}

	return cast_from_MeshVertexBufferD3D11(buffer);
}

void MeshVertexBufferReleaseD3D11(MeshVertexBuffer* buffer)
{
	if (buffer == nullptr) return;

	delete cast_to_MeshVertexBufferD3D11(buffer);
}

void MeshContextDrawD3D11(MeshContext* contextIn, const MeshContextDrawParams* params)
{
	auto context = cast_to_MeshContextD3D11(contextIn);

	using namespace DirectX;

	XMMATRIX matrix = XMMatrixTranspose(XMMatrixMultiply(XMMatrixMultiply(
		params->params->model, 
		params->params->view),
		params->params->projection)
		);

	ID3D11DeviceContext* deviceContext = context->m_deviceContext;

	// update constant buffer
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};
		if (S_OK == deviceContext->Map(context->m_constantBuffer, 0u, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))
		{
			XMStoreFloat4x4((XMFLOAT4X4*)(mappedResource.pData), matrix);

			deviceContext->Unmap(context->m_constantBuffer, 0u);
		}
	}

	deviceContext->VSSetShader(context->m_meshVS, nullptr, 0u);
	deviceContext->PSSetShader(context->m_meshPS, nullptr, 0u);

	deviceContext->IASetInputLayout(context->m_inputLayout);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetConstantBuffers(0, 1, &context->m_constantBuffer);

	UINT vertexStride = sizeof(MeshVertex);
	UINT offset = 0u;
	deviceContext->IASetVertexBuffers(0, 1, &cast_to_MeshVertexBufferD3D11(params->vertexBuffer)->m_buffer, &vertexStride, &offset);
	deviceContext->IASetIndexBuffer(cast_to_MeshIndexBufferD3D11(params->indexBuffer)->m_buffer, DXGI_FORMAT_R32_UINT, 0u);

	float depthSign = DirectX::XMVectorGetW(params->params->projection.r[2]);
	if (depthSign < 0.f)
	{
		deviceContext->RSSetState(context->m_rasterizerStateRH);
	}

	deviceContext->DrawIndexed((UINT)cast_to_MeshIndexBufferD3D11(params->indexBuffer)->m_numElements, 0, 0);

	if (depthSign < 0.f)
	{
		deviceContext->RSSetState(nullptr);
	}
}