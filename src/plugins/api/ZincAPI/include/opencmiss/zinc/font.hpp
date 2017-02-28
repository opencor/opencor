/**
 * @file font.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FONT_HPP__
#define CMZN_FONT_HPP__

#include "opencmiss/zinc/font.h"
#include "opencmiss/zinc/context.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Font
{
protected:
	cmzn_font_id id;

public:

	Font() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Font(cmzn_font_id font_id) : id(font_id)
	{  }

	Font(const Font& font) : id(cmzn_font_access(font.id))
	{  }

	Font& operator=(const Font& font)
	{
		cmzn_font_id temp_id = cmzn_font_access(font.id);
		cmzn_font_destroy(&id);
		id = temp_id;
		return *this;
	}

	~Font()
	{
		cmzn_font_destroy(&id);
	}

	enum RenderType
	{
		RENDER_TYPE_INVALID = CMZN_FONT_RENDER_TYPE_INVALID,
		RENDER_TYPE_BITMAP = CMZN_FONT_RENDER_TYPE_BITMAP,
		RENDER_TYPE_PIXMAP = CMZN_FONT_RENDER_TYPE_PIXMAP,
		RENDER_TYPE_POLYGON = CMZN_FONT_RENDER_TYPE_POLYGON,
		RENDER_TYPE_OUTLINE = CMZN_FONT_RENDER_TYPE_OUTLINE,
		RENDER_TYPE_EXTRUDE = CMZN_FONT_RENDER_TYPE_EXTRUDE
	};

	enum TypefaceType
	{
		TYPEFACE_TYPE_INVALID = CMZN_FONT_TYPEFACE_TYPE_INVALID,
		TYPEFACE_TYPE_OPENSANS = CMZN_FONT_TYPEFACE_TYPE_OPENSANS
	};

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_font_id getId() const
	{
		return id;
	}

	char *getName()
	{
		return cmzn_font_get_name(id);
	}

	int setName(const char *name)
	{
		return cmzn_font_set_name(id, name);
 	}

	bool isBold()
	{
		return cmzn_font_is_bold(id);
	}

	int setBold(bool bold)
	{
		return cmzn_font_set_bold(id, bold);
	}

	double getDepth()
	{
		return cmzn_font_get_depth(id);
	}

	int setDepth(double depth)
	{
		return cmzn_font_set_depth(id, depth);
	}

	bool isItalic()
	{
		return cmzn_font_is_italic(id);
	}

	int setItalic(bool italic)
	{
		return cmzn_font_set_italic(id, italic);
	}

	int getPointSize()
	{
		return cmzn_font_get_point_size(id);
	}

	int setPointSize(int size)
	{
		return cmzn_font_set_point_size(id, size);
	}

	enum RenderType getRenderType()
	{
		return static_cast<RenderType>(cmzn_font_get_render_type(id));
	}

	int setRenderType(RenderType renderType)
	{
		return cmzn_font_set_render_type(id,
			static_cast<cmzn_font_render_type>(renderType));
	}

	TypefaceType getTypefaceType()
	{
		return static_cast<TypefaceType>(cmzn_font_get_typeface_type(id));
	}

	int setTypefaceType(TypefaceType typefaceType)
	{
		return cmzn_font_set_typeface_type(id, static_cast<cmzn_font_typeface_type>(typefaceType));
	}
};

inline bool operator==(const Font& a, const Font& b)
{
	return a.getId() == b.getId();
}

class Fontmodule
{
protected:
	cmzn_fontmodule_id id;

public:

	Fontmodule() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Fontmodule(cmzn_fontmodule_id in_fontmodule_id) :
		id(in_fontmodule_id)
	{  }

	Fontmodule(const Fontmodule& fontModule) :
		id(cmzn_fontmodule_access(fontModule.id))
	{  }

	Fontmodule& operator=(const Fontmodule& fontModule)
	{
		cmzn_fontmodule_id temp_id = cmzn_fontmodule_access(
			fontModule.id);
		if (0 != id)
		{
			cmzn_fontmodule_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Fontmodule()
	{
		if (0 != id)
		{
			cmzn_fontmodule_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_fontmodule_id getId() const
	{
		return id;
	}

	Font createFont()
	{
		return Font(cmzn_fontmodule_create_font(id));
	}

	Font findFontByName(const char *name)
	{
		return Font(cmzn_fontmodule_find_font_by_name(id, name));
	}

	int beginChange()
	{
		return cmzn_fontmodule_begin_change(id);
	}

	int endChange()
	{
		return cmzn_fontmodule_end_change(id);
	}

	Font getDefaultFont()
	{
		return Font(cmzn_fontmodule_get_default_font(id));
	}

	int setDefaultFont(const Font& font)
	{
		return cmzn_fontmodule_set_default_font(id, font.getId());
	}
};

inline Fontmodule Context::getFontmodule()
{
	return Fontmodule(cmzn_context_get_fontmodule(id));
}

} // namespace Zinc
}

#endif
