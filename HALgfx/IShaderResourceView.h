#ifndef ISHADER_RESOURCE_VIEW_H
#define ISHADER_RESOURCE_VIEW_H

#include "IView.h"
#include "Format.h"

namespace Magnet
{
namespace HALgfx
{

#define MAX_NUM_SRVS 8

enum SRVDimension
{
	SRV_DIMENSION_UNKNOWN           = 0,
	SRV_DIMENSION_BUFFER            = 1,
	SRV_DIMENSION_TEXTURE1D         = 2,
	SRV_DIMENSION_TEXTURE1DARRAY    = 3,
	SRV_DIMENSION_TEXTURE2D         = 4,
	SRV_DIMENSION_TEXTURE2DARRAY    = 5,
	SRV_DIMENSION_TEXTURE2DMS       = 6,
	SRV_DIMENSION_TEXTURE2DMSARRAY  = 7,
	SRV_DIMENSION_TEXTURE3D         = 8,
	SRV_DIMENSION_TEXTURECUBE       = 9,
	SRV_DIMENSION_TEXTURECUBEARRAY  = 10,
	SRV_DIMENSION_BUFFEREX          = 11
};

struct BufferSRV
{
	union
	{
		unsigned int firstElement;
		unsigned int elementOffset;
	};

	union
	{
		unsigned int numElements;
		unsigned int elementWidth;
	};
};

struct TexSRV
{
	unsigned int mostDetailedMip;
	unsigned int mipLevels;
};

struct TexArraySRV
{
	unsigned int mostDetailedMip;
	unsigned int mipLevels;
	unsigned int firstArraySlice;
	unsigned int arraySize;
};

struct TexCubeArraySRV
{
	unsigned int mostDetailedMip;
	unsigned int mipLevels;
	unsigned int first2DArrayFace;
	unsigned int numCubes;

};


struct ShaderResourceViewDesc
{
	Format format;
	SRVDimension viewDimension;
	union
	{
		BufferSRV bufferSRV;
		TexSRV    texSRV;
		TexArraySRV texArraySRV;
		TexCubeArraySRV texCubeArraySRV;
	};

};

class IShaderResourceView : public IView
{
public:
	~IShaderResourceView(){}
};
} // namespace HALgfx
} // namespace Magnet

#endif