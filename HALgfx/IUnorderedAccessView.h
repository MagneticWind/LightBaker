#ifndef IUNORDERED_ACCESS_VIEW
#define IUNORDERED_ACCESS_VIEW

#include "Format.h"
#include "IView.h"

namespace Magnet
{
namespace HALgfx
{

#define MAX_NUMBER_UAVS 4

enum UAVDimension
{
	UAV_DIMENSION_UNKNOWN = 0,
	UAV_DIMENSION_BUFFER = 1,
	UAV_DIMENSION_TEXTURE1D = 2,
	UAV_DIMENSION_TEXTURE1DARRAY = 3,
	UAV_DIMENSION_TEXTURE2D = 4,
	UAV_DIMENSION_TEXTURE2DARRAY = 5,
	UAV_DIMENSION_TEXTURE3D = 8
};

struct BufferUAV
{
	unsigned int firstElement;
	unsigned int numElements;
	unsigned int flags;
};

struct Tex1DUAV
{
	unsigned int mipSlice;
};

struct Tex1DArrayUAV
{
	unsigned int mipSlice;
	unsigned int firstArraySlice;
	unsigned int arraySize;
};

struct Tex2DUAV
{
	unsigned int mipSlice;
};

struct Tex2DArrayUAV
{
	unsigned int mipSlice;
	unsigned int firstArraySlice;
	unsigned int arraySize;
};

struct Tex3DUAV
{
	unsigned int mipSlice;
	unsigned int firstWSlice;
	unsigned int wSize;
};

struct UnorderedAccessViewDesc
{
	Format format;
	UAVDimension viewDimension;
	union
	{
		BufferUAV bufferUAV;
		Tex1DUAV tex1DUAV;
		Tex1DArrayUAV tex1DArrayUAV;
		Tex2DUAV tex2DUAV;
		Tex2DArrayUAV tex2DArrayUAV;
		Tex3DUAV tex3DUAV;
	};
};

class IUnorderedAccessView : public IView
{

};

} // namespace HALgfx
} // namespace Magnet

#endif