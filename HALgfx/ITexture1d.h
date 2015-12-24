#ifndef TEXTURE1D_H
#define TEXTURE1D_H

#include "IResource.h"
#include "Format.h"

namespace Magnet
{
namespace HALgfx
{

struct Texture1dDesc
{
	Texture1dDesc() : width(0), mipLevels(9), arraySize(1)
	{
		usage = USAGE_DEFAULT;
		miscFlags = MISC_DEFAULT;
		cpuAccessFlags = 0;
	}
	int width;
	int mipLevels;
	int arraySize;
	Format format;
	Usage usage;

	unsigned int bindFlags;
	unsigned int cpuAccessFlags;
	unsigned int miscFlags;
};

class ITexture1d : public IResource
{
	virtual ResourceType GetType() const = 0;
};


} // namespace HALgfx
} // namespace Magnet

#endif