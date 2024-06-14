#pragma once
#include "Types.hpp"

namespace Vega::rage {
	struct scrNativeCallContext {
	protected:
		void* m_Return;
		std::uint32_t m_ArgCount;
		char padding[4];
		void* m_Args;
		std::uint32_t m_DataCount;
		char padding1[4];
		alignas(std::uintptr_t) std::uint8_t m_VectorSpace[192];
	};
	struct scrNativeRegistration {
		std::uint64_t m_NextRegistration1;
		std::uint64_t m_NextRegistration2;
		void* m_Handlers[7];
		std::uint32_t m_NumEntries1;
		std::uint32_t m_NumEntries2;
		std::uint64_t m_Hashes;
		inline scrNativeRegistration* GetNextRegistration() {
			std::uintptr_t result;
			auto v5 = std::uintptr_t(&m_NextRegistration1); auto v12 = 2;
			auto v13 = v5 ^ m_NextRegistration2; auto v14 = (char*)&result - v5;
			do {
				*(DWORD*)&v14[v5] = static_cast<DWORD>(v13) ^ *(DWORD*)v5;
				v5 += 4; --v12;
			} while (v12);
			return (scrNativeRegistration*)result;
		}
		inline std::uint32_t GetNumEntries() {
			return static_cast<std::uint32_t>(((std::uintptr_t)&m_NumEntries1) ^ m_NumEntries1 ^ m_NumEntries2);
		}
		inline std::uint64_t GetHash(std::uint32_t index) {
			auto naddr = 16 * index + std::uintptr_t(&m_NextRegistration1) + 0x54; auto v8 = 2;
			std::uint64_t nResult; auto v11 = (char*)&nResult - naddr;
			auto v10 = naddr ^ *(DWORD*)(naddr + 8);
			do {
				*(DWORD*)&v11[naddr] = static_cast<DWORD>(v10 ^ *(DWORD*)(naddr));
				naddr += 4; --v8;
			} while (v8);
			return nResult;
		}
	};
	struct scrNativeRegistrationTable {
		scrNativeRegistration* m_entries[0xFF];
		std::uint32_t m_unk;
		bool m_initialized;
	};
	using scrNativeHash = std::uint64_t;
	using scrNativeMapping = std::pair<scrNativeHash, scrNativeHash>;
	using scrNativeHandler = void(*)(scrNativeCallContext*);
}
namespace Vega {
	struct NativeContext : public rage::scrNativeCallContext {
		NativeContext() {
			Reset();
		}
		void Reset() {
			m_Return = &m_ReturnData[0];
			m_ArgCount = 0;
			m_Args = &m_ArgumentData[0];
			m_DataCount = 0;
			std::fill(std::begin(m_VectorSpace), std::end(m_VectorSpace), '\0');
			std::fill(std::begin(m_ReturnData), std::end(m_ReturnData), 0);
			std::fill(std::begin(m_ArgumentData), std::end(m_ArgumentData), 0);
		}
		template <typename T>
		void Push(T value) {
			static_assert(sizeof(T) <= 8);
			std::uintptr_t data{};
			*(T*)(&data) = value;
			m_ArgumentData[m_ArgCount++] = data;
		}
		template <>
		void Push<Vector3>(Vector3 value) {
			Push(value.x);
			Push(value.y);
			Push(value.z);
		}
		template <typename T>
		T GetReturnValue() { return *static_cast<T*>(m_Return); }
		template <>
		void GetReturnValue<void>() {}
	private:
		std::uintptr_t m_ReturnData[10] = {};
		std::uintptr_t m_ArgumentData[100] = {};
	};
	class Invoker {
	public:
		Invoker() = default;
		~Invoker() = default;
		bool MapNatives();
		void BeginCall();
		template <typename T> void Push(T&& value) { m_Context.Push(std::forward<T>(value)); }
		template <typename T> T GetReturn() { return m_Context.GetReturnValue<T>(); }
		void EndCall(std::uint64_t hash);
	public:
		std::unordered_map<rage::scrNativeHash, rage::scrNativeHandler> m_Cache;
		NativeContext m_Context;
	};
	inline std::unique_ptr<Invoker> g_Invoker;
}