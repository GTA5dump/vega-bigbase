#include "PrecompiledHeader.hpp"
#include "ScriptCallback.hpp"

#include "Reconstructed Classes/CObject.hpp"
#include "Reconstructed Classes/netObject.hpp"
#include "Reconstructed Classes/CPlayerInfo.hpp"

#include "User Interface/DrawHelper.hpp"
#include "Natives.hpp"
#include "Pointers.hpp"
#include "CustomText.hpp"
#include "Util.hpp"

namespace Vega
{
	std::unique_ptr<ScriptCallbackManager> g_ScriptCallbackManager;

	ModelScriptCallback::ModelScriptCallback(std::uint32_t modelHash, std::function<void()> successCallback) :
		m_ModelHash(modelHash),
		m_SuccessCallback(std::move(successCallback))
	{
	}

	bool ModelScriptCallback::isDone()
	{
		return Natives::STREAMING::HAS_MODEL_LOADED(m_ModelHash);
	}

	bool ModelScriptCallback::hasTimedOut()
	{
		return false;
	}

	void ModelScriptCallback::onTimeout()
	{
	}

	void ModelScriptCallback::onSuccess()
	{
		if (m_SuccessCallback)
		{
			std::invoke(m_SuccessCallback);
		}
	}

	void ModelScriptCallback::onFailure()
	{
		g_ScriptCallbackManager->setDisplayFlag(ScriptCallbackDisplayFlags::Loading);
		Natives::STREAMING::REQUEST_MODEL(m_ModelHash);
	}

	ScriptCallbackManager::ScriptCallbackManager()
	{
	}

	ScriptCallbackManager::~ScriptCallbackManager()
	{
	}

	void ScriptCallbackManager::onTick()
	{
		std::size_t infoCount = 0;
		if (m_DisplayFlags & ScriptCallbackDisplayFlags::Loading)
		{
			g_DrawHelper.text("Loading...", { 0.01f, (++infoCount * 0.04f) }, { 0.5f, 0.5f }, { 255, 255, 255, 255 }, DrawHelper::TextAlign::Left, DrawHelper::TextFont::ChaletComprimeCologne, { 0.f, 1.f }, true, true);
		}

		if (m_DisplayFlags & ScriptCallbackDisplayFlags::RequestingControl)
		{
			g_DrawHelper.text("Loading...", { 0.01f, (++infoCount * 0.04f) }, { 0.5f, 0.5f }, { 255, 255, 255, 255 }, DrawHelper::TextAlign::Left, DrawHelper::TextFont::ChaletComprimeCologne, { 0.f, 1.f }, true, true);
		}

		m_DisplayFlags = 0;

		for (auto it = m_Callbacks.begin(), end = m_Callbacks.end(); it != end; ++it)
		{
			auto const& callback = *it;

			if (callback->hasTimedOut())
			{
				callback->onTimeout();
				m_Callbacks.erase(it);
			}
			else if (callback->isDone())
			{
				callback->onSuccess();
				m_Callbacks.erase(it);
			}
			else
			{
				callback->onFailure();
			}
		}
	}

	void ScriptCallbackManager::setDisplayFlag(std::uint64_t flag)
	{
		m_DisplayFlags |= flag;
	}

	bool ScriptCallbackManager::isFlagSet(std::uint64_t flag)
	{
		return m_DisplayFlags & flag;
	}

	NetworkControlScriptCallback::NetworkControlScriptCallback(std::int32_t entityHandle, std::function<void()> successCallback) :
		m_EntityHandle(entityHandle),
		m_SuccessCallback(std::move(successCallback)),
		m_StartTime(GetTickCount()),
		m_NetworkId(Natives::NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(m_EntityHandle))
	{
	}

	bool NetworkControlScriptCallback::isDone()
	{
		if (!*g_Pointers->m_IsSessionStarted)
		{
			return true;
		}

		return Natives::NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(m_EntityHandle) || Natives::NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(m_NetworkId);
	}

	bool NetworkControlScriptCallback::hasTimedOut()
	{
		return (GetTickCount() - m_StartTime) >= 5000;
	}

	void NetworkControlScriptCallback::onTimeout()
	{
		if (auto ptr = Util::handleToPtr<ReconstructedClasses::CObject*>(m_EntityHandle))
		{
			if (auto netObj = ptr->netObject)
			{
				auto ownerId = netObj->syncData.ownerId;
				if (ownerId != Util::getLocalPlayerId())
				{
					Util::simpleNotify(6, "Couldn't take control of object from %s.", Util::getPlayerName(ownerId));
				}
				else
				{
					std::invoke(m_SuccessCallback);
				}
			}
		}
	}

	void NetworkControlScriptCallback::onSuccess()
	{
		if (m_SuccessCallback)
		{
			std::invoke(m_SuccessCallback);
		}
	}

	void NetworkControlScriptCallback::onFailure()
	{
		g_ScriptCallbackManager->setDisplayFlag(ScriptCallbackDisplayFlags::RequestingControl);
		Natives::NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(m_EntityHandle);
		Natives::NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(m_EntityHandle);
	}

	WeaponScriptCallback::WeaponScriptCallback(std::uint32_t modelHash, std::function<void()> successCallback) :
		m_ModelHash(modelHash),
		m_SuccessCallback(std::move(successCallback))
	{
	}

	bool WeaponScriptCallback::isDone()
	{
		return Natives::WEAPON::HAS_WEAPON_ASSET_LOADED(m_ModelHash);
	}

	bool WeaponScriptCallback::hasTimedOut()
	{
		return false;
	}

	void WeaponScriptCallback::onTimeout()
	{
	}

	void WeaponScriptCallback::onSuccess()
	{
		if (m_SuccessCallback)
		{
			std::invoke(m_SuccessCallback);
		}
	}

	void WeaponScriptCallback::onFailure()
	{
		g_ScriptCallbackManager->setDisplayFlag(ScriptCallbackDisplayFlags::Loading);
		Natives::WEAPON::REQUEST_WEAPON_ASSET(m_ModelHash, 31, false);
	}

	ParticleFXScriptCallback::ParticleFXScriptCallback(const char* assetName, std::function<void()> successCallback) : m_SuccessCallback(std::move(successCallback))
	{
		std::strncpy(m_AssetName, assetName, sizeof(m_AssetName) - 1);
	}

	bool ParticleFXScriptCallback::isDone()
	{
		return Natives::STREAMING::HAS_NAMED_PTFX_ASSET_LOADED(m_AssetName);
	}

	bool ParticleFXScriptCallback::hasTimedOut()
	{
		return false;
	}

	void ParticleFXScriptCallback::onTimeout()
	{
	}

	void ParticleFXScriptCallback::onSuccess()
	{
		Natives::GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL(m_AssetName);
		if (m_SuccessCallback)
		{
			std::invoke(m_SuccessCallback);
		}
	}

	void ParticleFXScriptCallback::onFailure()
	{
		g_ScriptCallbackManager->setDisplayFlag(ScriptCallbackDisplayFlags::Loading);
		Natives::STREAMING::REQUEST_NAMED_PTFX_ASSET(m_AssetName);
	}

	KeyboardScriptCallback::KeyboardScriptCallback(const char* text, std::uint32_t maxLength, std::function<void(const char*)> successCallback) :
		m_MaxLength(maxLength),
		m_SuccessCallback(std::move(successCallback))
	{
		std::snprintf(m_CustomLabel, sizeof(m_CustomLabel) - 1, "KEYBOARD_%i_%i", std::rand(), std::rand());
		std::strncpy(m_CustomLabelText, text, sizeof(m_CustomLabelText) - 1);
		g_CustomText.reg(m_CustomLabel, m_CustomLabelText);
		Natives::MISC::DISPLAY_ONSCREEN_KEYBOARD(NULL, m_CustomLabel, "", "", "", "", "", m_MaxLength);
	}

	bool KeyboardScriptCallback::isDone()
	{
		return Natives::MISC::UPDATE_ONSCREEN_KEYBOARD();
	}

	void KeyboardScriptCallback::onTimeout()
	{
	}

	bool KeyboardScriptCallback::hasTimedOut()
	{
		return false;
	}

	void KeyboardScriptCallback::onSuccess()
	{
		if (m_SuccessCallback)
		{
			if (auto result = Natives::MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			{
				std::invoke(m_SuccessCallback, result);
			}
		}

		g_CustomText.unreg(m_CustomLabel);
	}

	void KeyboardScriptCallback::onFailure()
	{
		g_ScriptCallbackManager->setDisplayFlag(ScriptCallbackDisplayFlags::Keyboard);
		Natives::MISC::UPDATE_ONSCREEN_KEYBOARD();
	}
}
