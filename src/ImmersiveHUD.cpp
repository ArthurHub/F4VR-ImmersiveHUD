#include "ImmersiveHUD.h"

using namespace common;

// This is the entry point to the mod.
extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_skse, F4SE::PluginInfo* a_info)
{
    return f4cf::g_mod->onF4SEPluginQuery(a_skse, a_info);
}

// This is the entry point to the mod.
extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_f4se)
{
    return f4cf::g_mod->onF4SEPluginLoad(a_f4se);
}

namespace immersive_hud
{
    /**
     * Run F4SE plugin load and initialize the plugin given the init handle.
     */
    void ImmersiveHUD::onModLoaded(const F4SE::LoadInterface*)
    {
        //noop
    }

    /**
     * On game fully loaded initialize things that should be initialized only once.
     */
    void ImmersiveHUD::onGameLoaded()
    {
        //noop
    }

    /**
     * Game session can be initialized multiple times as it is fired on new game and save loaded events.
     */
    void ImmersiveHUD::onGameSessionLoaded()
    {
        //noop
    }

    /**
     * On every frame if player is initialized.
     */
    void ImmersiveHUD::onFrameUpdate()
    {
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->loadedData) {
            logger::sample(3000, "no player data - noop");
            return;
        }

        // TODO: add logic
    }
}
