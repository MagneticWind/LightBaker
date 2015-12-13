#ifndef IGEOMETRY_H
#define IGEOMETRY_H

namespace Magnet
{
namespace Scene
{

enum GeometryType
{
	GEOMETRY_MESH
};

enum PrimitiveType
{
	PRIMITIVE_TRIANGLE_LIST,
	PRIMITIVE_TRIANGLE_STRIP
};

class IGeometry
{

public:
	virtual ~IGeometry(){}
	virtual PrimitiveType GetPrimitiveType() const = 0;
	virtual GeometryType GetGeometryType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetNumPrimitives() const = 0;

	virtual bool IsLoaded() const = 0;
	virtual void SetLoaded(bool bLoaded) = 0;

protected:
	PrimitiveType m_ePrimitiveType;
};

} // namespace Scene
} // namespace Magnet

#endif