#ifndef SHADER_TYPE_H
#define SHADER_TYPE_H

namespace Magnet
{
namespace HALgfx
{
enum ShaderType
{
	VERTEX_SHADER = 0,
	GEOMETRY_SHADER,
	HULL_SHADER,
	DOMAIN_SHADER,
	PIXEL_SHADER,
	COMPUTE_SHADER,
	MAX_SHADER_NUM
};

} // namespace HALgfx
} // namespace Magnet

#endif