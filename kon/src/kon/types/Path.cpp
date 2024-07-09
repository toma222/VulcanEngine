
#include "Path.hpp"

#include <sys/stat.h>

namespace kon
{
	Path::Path(const String &str)
		: m_pathString(str)
	{
		// load the file from the system
	
		struct stat buffer;
		int err = stat(str.c_str(), &buffer);
		if(err == -1)
		{
			return;
		}
		
		
		m_fileData |= (u8)PathDataBits::ExistsBit;

		if(buffer.st_mode == S_IFDIR)
		{ 
			m_fileData |= (u8)PathDataBits::DirectoryBit;
		}else{
			// get the size of the file
			m_fileSize = buffer.st_size;
		}

		if(str.c_str()[0] == '/') m_fileData |= (u8)PathDataBits::RelitaveBit;
		
	}
}

