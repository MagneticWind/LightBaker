#ifndef ISAMPLER_STATE_H
#define ISAMPLER_STATE_H

#include "ComparisonFunc.h"

namespace Magnet
{
namespace HALgfx
{

enum Filter
{
	FILTER_MIN_MAG_MIP_POINT,
	FILTER_MIN_MAG_MIP_LINEAR,
	FILTER_MIN_MAG_NOMIP_LINEAR,
	FILTER_ANISOTROPIC
};

enum AddressMode
{
	ADDRESSMODE_WRAP,
	ADDRESSMODE_CLAMP,
	ADDRESSMODE_BORDER
};

struct SamplerStateDesc
{
	int maxAnisotropy;
	float mipLodBias;
	float minLod;
	float maxLod;
	Filter filter;
	AddressMode addressU;
	AddressMode addressV;
	AddressMode addressW;
	ComparisonFunc comparisonFunc;
};

class ISamplerState
{
public:
	virtual ~ISamplerState(){}
	//virtual void GetDesc() = 0;
};

} // namespace HALgfx
} // namespace Magnet

#endif