#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "IResource.h"
#include "Format.h"

namespace Magnet
{
namespace HALgfx
{

struct Texture2dDesc
{
	Texture2dDesc() : width(0), height(0), mipLevels(9), arraySize(1)
	{
		usage = USAGE_DEFAULT;
		miscFlags = MISC_DEFAULT;
		cpuAccessFlags = 0;
	}
	int width;
	int height;
	int mipLevels;
	int arraySize;
	Format format;
	Usage usage;
	SampleDesc sampleDesc;
	unsigned int bindFlags;
	unsigned int cpuAccessFlags;
	unsigned int miscFlags;
};

class ITexture2d : public IResource
{
	virtual ResourceType GetType() const = 0;
};


} // namespace HALgfx
} // namespace Magnet

#endif