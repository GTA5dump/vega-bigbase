#include "PrecompiledHeader.hpp"

namespace rage
{
#	pragma pack(push, 1)
	template <typename T>
	class scrHandle
	{	
	public:
		using containedType = T;
	private:
		std::uint8_t index[3];
		std::uint8_t state = -1;
	public:
		std::int32_t getRaw()
		{
			std::printf("%i", sizeof(*this));
			return *(std::int32_t*)(this);
		}

		operator std::int32_t()
		{
			return getRaw();
		}
	};
#	pragma pack(pop)
}
