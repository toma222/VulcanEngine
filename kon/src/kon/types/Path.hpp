
#pragma once

#include "kon/core/Core.hpp"
#include <kon/types/String.hpp>

namespace kon
{
	// Defines a path on users computer
	// also the whole thing is const lmao
	class Path
	{
	public:
		enum class PathDataBits : u8
		{
			ExistsBit    = KN_BIT(0),
			DirectoryBit = KN_BIT(1),
			RelitaveBit  = KN_BIT(2),
		};

	public:
		Path(const String &str);
		~Path() = default;

		bool Valid() const { return (m_fileData & (u8)PathDataBits::ExistsBit); }
		u32 GetFileSize() const { return m_fileSize; }
		u8 GetFileDataBits() const { return m_fileData; }
		
		// goes back to the previous folder/file
		Path Back();

		Path operator /=(const String &string)
		{
			String s = m_pathString.SubString(m_pathString.IndexOfString(string));
			return Path(s);
		}

		Path operator +=(const String &string)
		{
			String s = m_pathString.AppendString(string).c_str();
			return Path(s);
		}
		
	public:
		operator String() const { return m_pathString; }

	private:
		const String m_pathString;
		u8 m_fileData {0};
		u32 m_fileSize {0};
	};
}

