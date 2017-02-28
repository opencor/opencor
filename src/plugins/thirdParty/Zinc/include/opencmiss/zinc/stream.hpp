/**
 * @file stream.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_STREAM_HPP__
#define CMZN_STREAM_HPP__

#include "opencmiss/zinc/stream.h"

namespace OpenCMISS
{
namespace Zinc
{

class StreamresourceFile;
class StreamresourceMemory;

class Streamresource
{
protected:

	cmzn_streamresource_id id;

public:

	Streamresource() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Streamresource(cmzn_streamresource_id in_streamresource_id) : id(in_streamresource_id)
	{  }

	Streamresource(const Streamresource& streamResource) : id(cmzn_streamresource_access(streamResource.id))
	{  }

	Streamresource& operator=(const Streamresource& streamResource)
	{
		cmzn_streamresource_id temp_id = cmzn_streamresource_access(streamResource.id);
		if (0 != id)
		{
			cmzn_streamresource_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Streamresource()
	{
		if (0 != id)
		{
			cmzn_streamresource_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_streamresource_id getId() const
	{
		return id;
	}

	inline StreamresourceFile castFile();
	inline StreamresourceMemory castMemory();

};

class StreamresourceFile : public Streamresource
{
public:

	StreamresourceFile() : Streamresource()
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit StreamresourceFile(cmzn_streamresource_file_id streamresource_file_id) :
		Streamresource(reinterpret_cast<cmzn_streamresource_id>(streamresource_file_id))
	{ }

	char *getName()
	{
		return cmzn_streamresource_file_get_name(
			reinterpret_cast<cmzn_streamresource_file_id>(id));
	}

};

class StreamresourceMemory : public Streamresource
{
public:

	StreamresourceMemory() : Streamresource()
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit StreamresourceMemory(cmzn_streamresource_memory_id streamresource_memory_id) :
		Streamresource(reinterpret_cast<cmzn_streamresource_id>(streamresource_memory_id))
	{ }

	int getBuffer(void **memory_buffer_references, unsigned int *memory_buffer_sizes)
	{
		return cmzn_streamresource_memory_get_buffer(
			reinterpret_cast<cmzn_streamresource_memory_id>(id), memory_buffer_references,
			memory_buffer_sizes);
	}

};

inline StreamresourceFile Streamresource::castFile()
{
	return StreamresourceFile(cmzn_streamresource_cast_file(id));
}

inline StreamresourceMemory Streamresource::castMemory()
{
	return StreamresourceMemory(cmzn_streamresource_cast_memory(id));
}

class StreaminformationImage;
class StreaminformationRegion;
class StreaminformationScene;

class Streaminformation
{
protected:

	cmzn_streaminformation_id id;

public:

	Streaminformation() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Streaminformation(cmzn_streaminformation_id in_streaminformation_id) : id(in_streaminformation_id)
	{  }

	Streaminformation(const Streaminformation& streamInformation) :
		id(cmzn_streaminformation_access(streamInformation.id))
	{  }

	enum DataCompressionType
	{
		DATA_COMPRESSION_TYPE_INVALID = 	CMZN_STREAMINFORMATION_DATA_COMPRESSION_TYPE_INVALID,
		DATA_COMPRESSION_TYPE_DEFAULT = CMZN_STREAMINFORMATION_DATA_COMPRESSION_TYPE_DEFAULT,
		DATA_COMPRESSION_TYPE_NONE = CMZN_STREAMINFORMATION_DATA_COMPRESSION_TYPE_NONE,
		DATA_COMPRESSION_TYPE_GZIP = CMZN_STREAMINFORMATION_DATA_COMPRESSION_TYPE_GZIP,
		DATA_COMPRESSION_TYPE_BZ2 = CMZN_STREAMINFORMATION_DATA_COMPRESSION_TYPE_BZIP2
	};

	Streaminformation& operator=(const Streaminformation& streamInformation)
	{
		cmzn_streaminformation_id temp_id = cmzn_streaminformation_access(streamInformation.id);
		if (0 != id)
		{
			cmzn_streaminformation_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Streaminformation()
	{
		if (0 != id)
		{
			cmzn_streaminformation_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_streaminformation_id getId() const
	{
		return id;
	}

	StreamresourceFile createStreamresourceFile(const char *file_name)
	{
		return StreamresourceFile(reinterpret_cast<cmzn_streamresource_file_id>(
			cmzn_streaminformation_create_streamresource_file(id, file_name)));
	}

	StreamresourceMemory createStreamresourceMemory()
	{
		return StreamresourceMemory(reinterpret_cast<cmzn_streamresource_memory_id>(
			cmzn_streaminformation_create_streamresource_memory(id)));
	}

	StreamresourceMemory createStreamresourceMemoryBuffer(const void *buffer,
		unsigned int buffer_length)
	{
		return StreamresourceMemory(reinterpret_cast<cmzn_streamresource_memory_id>(
			cmzn_streaminformation_create_streamresource_memory_buffer(id, buffer, buffer_length)));
	}

	StreamresourceMemory createStreamresourceMemoryBufferCopy(const void *buffer,
		unsigned int buffer_length)
	{
		return StreamresourceMemory(reinterpret_cast<cmzn_streamresource_memory_id>(
			cmzn_streaminformation_create_streamresource_memory_buffer_copy(id, buffer, buffer_length)));
	}

	enum DataCompressionType getResourceDataCompressionType(const Streamresource& resource)
	{
		return static_cast<DataCompressionType>(
			cmzn_streaminformation_get_resource_data_compression_type(
				reinterpret_cast<cmzn_streaminformation_id>(id), resource.getId()));
	}

	int setResourceDataCompressionType(const Streamresource& resource, DataCompressionType dataCompressionType)
	{
		return cmzn_streaminformation_set_resource_data_compression_type(
			reinterpret_cast<cmzn_streaminformation_id>(id), resource.getId(),
			static_cast<cmzn_streaminformation_data_compression_type>(dataCompressionType));
	}

	enum DataCompressionType getDataCompressionType()
	{
		return static_cast<DataCompressionType>(
			cmzn_streaminformation_get_data_compression_type(
				reinterpret_cast<cmzn_streaminformation_id>(id)));
	}

	int setDataCompressionType(DataCompressionType dataCompressionType)
	{
		return cmzn_streaminformation_set_data_compression_type(
			reinterpret_cast<cmzn_streaminformation_id>(id),
			static_cast<cmzn_streaminformation_data_compression_type>(dataCompressionType));
	}

	inline StreaminformationImage castImage();
	inline StreaminformationRegion castRegion();
	inline StreaminformationScene castScene();

};

}  // namespace Zinc
}


#endif /* CMZN_STREAM_HPP__ */
