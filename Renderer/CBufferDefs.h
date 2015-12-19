#ifndef CBUFFER_DEFS_H
#define CBUFFER_DEFS_H

#include "Math\Matrix4f.h"
#include "Math\Vector4f.h"

namespace Magnet
{
namespace Renderer
{

struct CBufferTransform
{
	Math::Matrix4f mWorld;
	Math::Matrix4f mView;
	Math::Matrix4f mProjection;
};

struct CBufferMaterialNormal
{
	Math::Vector4f v4Ka;
	Math::Vector4f v4Kd;
	Math::Vector4f v4Ks; // w component is the exponent
	Math::Vector4f v4Ke;
};

struct CBufferLights
{
	Math::Vector4f v4LightPosition;
	Math::Vector4f v4LightColor;
};

struct CBufferToneMapping
{
	Math::Vector4f v4Param; // (width, height, avg_luminance)
};

} // namespace Renderer
} // namespace Magnet

#endif