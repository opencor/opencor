/**
 * @file region.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_REGION_HPP__
#define CMZN_REGION_HPP__

#include "opencmiss/zinc/region.h"
#include "opencmiss/zinc/context.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Fieldmodule;
class Scene;
class StreaminformationRegion;

class Region
{
protected:
	cmzn_region_id id;

public:

	Region() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Region(cmzn_region_id in_region_id) : id(in_region_id)
	{  }

	Region(const Region& region) : id(cmzn_region_access(region.id))
	{ }

	Region& operator=(const Region& region)
	{
		cmzn_region_id temp_id = cmzn_region_access(region.id);
		if (0 != id)
		{
			cmzn_region_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Region()
	{
		if (0 != id)
		{
			cmzn_region_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_region_id getId() const
	{
		return id;
	}

	int beginChange()
	{
		return cmzn_region_begin_change(id);
	}

	int endChange()
	{
		return cmzn_region_end_change(id);
	}

	int beginHierarchicalChange()
	{
		return cmzn_region_begin_hierarchical_change(id);
	}

	int endHierarchicalChange()
	{
		return cmzn_region_end_hierarchical_change(id);
	}

	Region createChild(const char *name)
	{
		return Region(cmzn_region_create_child(id, name));
	}

	Region createSubregion(const char *path)
	{
		return Region(cmzn_region_create_subregion(id, path));
	}

	Region createRegion()
	{
		return Region(cmzn_region_create_region(id));
	}

	inline Fieldmodule getFieldmodule() const;

	int readFile(const char *fileName)
	{
		return cmzn_region_read_file(id, fileName);
	}

	char *getName()
	{
		return cmzn_region_get_name(id);
	}

	int setName(const char *name)
	{
		return cmzn_region_set_name(id, name);
	}

	Region getParent()
	{
		return Region(cmzn_region_get_parent(id));
	}

	Region getFirstChild()
	{
		return Region(cmzn_region_get_first_child(id));
	}

	Region getNextSibling()
	{
		return Region(cmzn_region_get_next_sibling(id));
	}

	Region getPreviousSibling()
	{
		return Region(cmzn_region_get_previous_sibling(id));
	}

	int appendChild(Region newChild)
	{
		return cmzn_region_append_child(id, newChild.id);
	}

	int insertChildBefore(const Region& newChild, const Region& refChild)
	{
		return cmzn_region_insert_child_before(id, newChild.id, refChild.id);
	}

	int removeChild(const Region& oldChild)
	{
		return cmzn_region_remove_child(id, oldChild.id);
	}

	Region findChildByName(const char *name)
	{
		return Region(cmzn_region_find_child_by_name(id, name));
	}

	Region findSubregionAtPath(const char *path)
	{
		return Region(cmzn_region_find_subregion_at_path(id, path));
	}

	bool containsSubregion(const Region& subregion)
	{
		return cmzn_region_contains_subregion(id, subregion.id);
	}

	int writeFile(const char *fileName)
	{
		return cmzn_region_write_file(id, fileName);
	}

	inline Scene getScene();

	inline StreaminformationRegion createStreaminformationRegion();

	inline int read(const StreaminformationRegion& streaminformationRegion);

	inline int write(const StreaminformationRegion& streaminformationRegion);

};

inline bool operator==(const Region& a, const Region& b)
{
	return a.getId() == b.getId();
}

inline Region Context::getDefaultRegion()
{
	return Region(cmzn_context_get_default_region(id));
}

inline int Context::setDefaultRegion(const Region& region)
{
	return cmzn_context_set_default_region(id, region.getId());
}

inline Region Context::createRegion()
{
	return Region(cmzn_context_create_region(id));
}

}  // namespace Zinc
}

#endif
