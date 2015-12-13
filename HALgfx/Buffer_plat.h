#ifndef BUFFER_H
#define BUFFER_H

#include <d3d11.h>

#include "IBuffer.h"
#include "IResource.h"

namespace Magnet
{
namespace HALgfx
{

class Buffer : public IBuffer
{
public:
	Buffer(const BufferDesc& desc) : mDesc(desc)
	{
	}
	~Buffer();
	virtual void GetDesc(BufferDesc& bufferDesc) const;
	virtual ResourceType GetType() const ;
	//virtual unsigned int GetElementCount() const;
	//virtual unsigned int GetElementStride() const;
	void Create(const SubResourceData&, ID3D11Device* pDevice);
	ID3D11Buffer* GetD3DPtr();

private:
	BufferDesc mDesc;
	unsigned int m_uElemCount;
	ID3D11Buffer* m_pBuffer;
};

inline ID3D11Buffer* Buffer::GetD3DPtr()
{
	return m_pBuffer;
}

inline ResourceType Buffer::GetType() const
{
	return RESOURCE_BUFFER;
}

} // namespace HALgfx
} // namespace Magnet

#endif