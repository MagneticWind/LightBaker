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

#define NUM_SAMPLER_PAIRS 5
struct CBufferSSAO
{
	CBufferSSAO()
	{
		fWidth = 800.f;
		fHeight = 600.f;
		fFarPlaneZ = 30.f;
		fCenterWeight = 0.5f;
		fInnerRadius = 0.3f;
		fOuterRadius = 0.5f;
		fMaxRadius = 1.0;
		fPlaceHolder = 0.f;
	}

	float fWidth;           // depth map width
	float fHeight;          // depth map height
	float fFarPlaneZ;       // z far
	float fCenterWeight;    // center sample weight (percentage in unit sphere volume)

	float fInnerRadius;     // small radius 
	float fOuterRadius;     // large radius
	float fMaxRadius;
	float fPlaceHolder;

	Math::Vector4f f4SamplePoints[NUM_SAMPLER_PAIRS];
};

} // namespace Renderer
} // namespace Magnet

#endif