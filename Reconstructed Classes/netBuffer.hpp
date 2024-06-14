#pragma once
#include <cstdint>
#include <cstddef>
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	namespace rage
	{
		struct netBuffer
		{
			inline netBuffer(void* data, std::size_t size)
			{
				m_data = data;
				m_f8 = 0;
				m_maxBit = size * 8;
				m_unkBit = 0;
				m_curBit = 0;
				m_unk2Bit = 0;
				m_f1C = 0;
			}

			inline std::uint32_t GetPosition()
			{
				return m_unkBit;
			}

			inline bool Seek(std::int32_t bits)
			{
				if (bits >= 0)
				{
					uint32_t length = (m_f1C & 1) ? m_maxBit : m_curBit;

					if (bits <= length)
					{
						m_unkBit = bits;
					}
				}

				return false;
			}

			inline std::size_t GetDataLength()
			{
				char leftoverBit = (m_curBit % 8) ? 1 : 0;

				return (m_curBit / 8) + leftoverBit;
			}

			//bool ReadInteger(std::uint32_t* integer, int bits);
			//bool WriteInteger(std::uint32_t integer, int bits);
			//bool WriteBit(bool bit);

			void* m_data;
			std::uint32_t m_f8;
			std::uint32_t m_maxBit;
			std::uint32_t m_unkBit;
			std::uint32_t m_curBit;
			std::uint32_t m_unk2Bit;
			std::uint8_t m_f1C;
		};
	}
}

#pragma pack(pop)
