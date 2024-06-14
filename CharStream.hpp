#pragma once
#include "PrecompiledHeader.hpp"

struct CharStreamBuf : public std::streambuf
{
	CharStreamBuf(char* array, std::size_t size)
	{
		std::streambuf::setp(array, array + size - 1);
		std::memset(array, 0, size);
	}

	template <std::size_t size>
	CharStreamBuf(char(&array)[size]):
		CharStreamBuf(&array[0], size)
	{
	}
};
struct CharMemStream : virtual CharStreamBuf, public std::ostream
{

	CharMemStream(char* array, std::size_t size) :
		CharStreamBuf(array, size),
		std::ostream(this)
	{
	}

	template <std::size_t size>
	CharMemStream(char(&array)[size]) :
		CharStreamBuf(array),
		std::ostream(this)
	{
	}
};
