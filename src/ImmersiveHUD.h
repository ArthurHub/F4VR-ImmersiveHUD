#pragma once

#include "Config.h"
#include "ModBase.h"
#include "RE/NetImmerse/NiAVObject.h"

namespace immersive_hud
{
    class ImmersiveHUD : public f4cf::ModBase
    {
    public:
        ImmersiveHUD() :
            ModBase(Settings(
                Version::PROJECT,
                Version::NAME,
                &g_config,
                32,
                true)) {}

    protected:
        virtual void onModLoaded(const F4SE::LoadInterface* f4SE) override;
        virtual void onGameLoaded() override;
        virtual void onGameSessionLoaded() override;
        virtual void onFrameUpdate() override;
        static void relocateHUDElement(const std::string& elementName, RE::NiAVObject* rootNode, const RE::NiAVObject* toNode);

    private:
    };

    // The ONE global to rule them ALL
    inline ImmersiveHUD g_ihud;
}
