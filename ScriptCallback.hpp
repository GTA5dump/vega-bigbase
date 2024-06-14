#pragma once
#include "PrecompiledHeader.hpp"

namespace Vega
{

	class ScriptCallback
	{
	public:
		virtual ~ScriptCallback() = default;
		virtual bool hasTimedOut() = 0;
		virtual void onTimeout() = 0;
		virtual bool isDone() = 0;
		virtual void onSuccess() = 0;
		virtual void onFailure() = 0;
	};

	class ModelScriptCallback : public ScriptCallback
	{
	private:
		std::function<void()> m_SuccessCallback;
		std::uint32_t m_ModelHash = 0;
	public:
		ModelScriptCallback(std::uint32_t modelHash, std::function<void()> successCallback);
		~ModelScriptCallback() override = default;

		bool isDone() override;
		bool hasTimedOut() override;
		void onTimeout() override;
		void onSuccess() override;
		void onFailure() override;
	};

	class WeaponScriptCallback : public ScriptCallback
	{
	private:
		std::function<void()> m_SuccessCallback;
		std::uint32_t m_ModelHash = 0;
	public:
		WeaponScriptCallback(std::uint32_t modelHash, std::function<void()> successCallback);
		~WeaponScriptCallback() override = default;

		bool isDone() override;
		bool hasTimedOut() override;
		void onTimeout() override;
		void onSuccess() override;
		void onFailure() override;
	};

	class NetworkControlScriptCallback : public ScriptCallback
	{
	private:
		std::int32_t m_EntityHandle = -1;
		std::int32_t m_NetworkId = -1;

		std::uint32_t m_StartTime = 0;
		std::function<void()> m_SuccessCallback;
	public:
		NetworkControlScriptCallback(std::int32_t entityHandle, std::function<void()> successCallback);
		~NetworkControlScriptCallback() override = default;

		bool isDone() override;
		bool hasTimedOut() override;
		void onTimeout() override;
		void onSuccess() override;
		void onFailure() override;
	};

	class ParticleFXScriptCallback : public ScriptCallback
	{
	private:
		char m_AssetName[64] = {};
		std::function<void()> m_SuccessCallback;
	public:
		ParticleFXScriptCallback(const char* assetName, std::function<void()> successCallback);
		~ParticleFXScriptCallback() override = default;

		bool isDone() override;
		bool hasTimedOut() override;
		void onTimeout() override;
		void onSuccess() override;
		void onFailure() override;
	};

	class KeyboardScriptCallback : public ScriptCallback
	{
	private:
		char m_CustomLabel[64] = {};
		char m_CustomLabelText[64] = {};
		std::uint32_t m_MaxLength = 20;
		std::function<void(const char*)> m_SuccessCallback;
	public:
		KeyboardScriptCallback(const char* text, std::uint32_t maxLength, std::function<void(const char*)> successCallback);
		~KeyboardScriptCallback() override = default;

		bool isDone() override;
		void onTimeout() override;
		bool hasTimedOut() override;
		void onSuccess() override;
		void onFailure() override;
	};

	class ScriptCallbackManager
	{
	private:
		std::uint64_t m_DisplayFlags = 0;
		std::deque<std::unique_ptr<ScriptCallback>> m_Callbacks;
	public:
		explicit ScriptCallbackManager();
		~ScriptCallbackManager();

		void onTick();

		template <typename T>
		void addCallback(T&& callback)
		{
			m_Callbacks.push_back(std::make_unique<T>(callback));
		}

		void setDisplayFlag(std::uint64_t flag);
		bool isFlagSet(std::uint64_t flag);
	};

	namespace ScriptCallbackDisplayFlags
	{
		enum Enum : std::uint64_t
		{
			Loading = (1 << 0),
			RequestingControl = (1 << 1),
			Keyboard = (1 << 2)
		};
	}

	extern std::unique_ptr<ScriptCallbackManager> g_ScriptCallbackManager;
}
