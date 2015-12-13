#ifndef IINPUT_LAYOUT_H
#define IINPUT_LAYOUT_H

#include "Format.h"

namespace Magnet
{
namespace HALgfx
{

#define MAX_NUM_ELEMENTS 8


enum InputClassification
{
	INPUT_PER_VERTEX_DATA,
	INPUT_PER_INSTANCE_DATA
};

struct InputElementDesc
{
	char semantic[32];
	Format format;
	InputClassification inputClassification;
	unsigned int semanticIndex;
	unsigned int inputSlot;
	unsigned int alignedByteOffset;
	unsigned int instanceDataStepRate;
};

enum InputSemantic
{
	INPUT_SEMANTIC_POSITION,
	INPUT_SEMANTIC_NORMAL,
	INPUT_SEMANTIC_TEXCOORD,
	INPUT_SEMANTIC_COLOR,
	INPUT_SEMANTIC_TANGENT,
	INPUT_SEMANTIC_BINORMAL,
	INPUT_SEMANTIC_BLENDWEIGHT,
	INPUT_SEMANTIC_BLENDINDICES
};

class IInputLayout
{
public:
	virtual ~IInputLayout(){}
};
} // namespace HALgfx
} // namespace Magnet

#endif