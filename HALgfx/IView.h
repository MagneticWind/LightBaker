#ifndef VIEW_H
#define VIEW_H

#include "IResource.h"

namespace Magnet
{
namespace HALgfx
{
class IView
{
public:
	virtual ~IView() {}
	//virtual IResource* GetResource() = 0;
};

} // namespace HALgfx
} // namespace Magnet

#endif