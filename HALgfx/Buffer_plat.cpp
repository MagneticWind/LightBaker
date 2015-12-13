#include "Buffer_plat.h"
#include "Device_plat.h"
#include "Resource_plat.h"
#include "Memory.h"

namespace Magnet
{
namespace HALgfx
{


Buffer::~Buffer()
{
	if (m_pBuffer)
	{
		m_pBuffer->Release();
		m_pBuffer = 0;
	}
}

void Buffer::GetDesc(BufferDesc& bufferDesc) const
{
	bufferDesc = mDesc;
}

void Buffer::Create(const SubResourceData& subResourceData, ID3D11Device* pDevice)
{

	// Setup D3D descriptor.
	D3D11_BUFFER_DESC descD3D;
	descD3D.ByteWidth           = mDesc.byteSize;
	descD3D.Usage               = GetD3DUsage(mDesc.usage);
	descD3D.BindFlags           = GetD3DBindFlags(mDesc.bindFlags);
	descD3D.CPUAccessFlags      = GetD3DAccessFlags(mDesc.cpuAccessFlags);
	descD3D.MiscFlags           = GetD3DMiscFlags(mDesc.miscFlags);
	descD3D.StructureByteStride = mDesc.byteStride;

	// Setup initialization data.
	D3D11_SUBRESOURCE_DATA initialData;
	if (subResourceData.pMem)
	{
		initialData.pSysMem = subResourceData.pMem;
		initialData.SysMemPitch = subResourceData.uMemPitch;
		initialData.SysMemSlicePitch = subResourceData.uMemSlicePitch;

		pDevice->CreateBuffer(&descD3D, &initialData, &m_pBuffer);
	}
	else
		pDevice->CreateBuffer(&descD3D, 0, &m_pBuffer);
}

} // namespace HALgfx
} // namespace Magnet