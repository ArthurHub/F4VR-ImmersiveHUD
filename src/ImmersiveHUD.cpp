#include "ImmersiveHUD.h"

#include <PCH.h>

#include "Debug.h"
#include "common/MatrixUtils.h"
#include "f4vr/PlayerNodes.h"
#include "f4vr/VRControllersManager.h"

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
        const auto armNode = f4vr::findAVObject(f4vr::getCommonNode(), "LArm_ForeArm3");
        const auto pipboyNode = f4vr::findNode(armNode, g_config.debugFlowText2.c_str());
        const auto hmdNifNode = f4vr::findNode(f4vr::getPlayerNodes()->UprightHmdLagNode, "world_HMD_info.nif");
        if (pipboyNode && hmdNifNode) {
            const auto sphere = f4vr::getClonedNiNodeForNifFile("IHUD_debug_sphere.nif");
            sphere->local.scale = 10;
            pipboyNode->AttachChild(sphere, true);

            const auto sphere2 = f4vr::getClonedNiNodeForNifFile("IHUD_debug_sphere.nif");
            hmdNifNode->AttachChild(sphere2, true);
        }
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

        if (f4vr::VRControllers.isLongPressed(f4vr::Hand::Primary, vr::k_EButton_A)) {
            f4vr::showNotification("Testing notification message 1.. 2.. 3.. hello world!");
        }

        const auto pn = f4vr::getPlayerNodes();

        // const auto pipboyNode = f4vr::findNode(pn->PipboyParentNode, g_config.debugFlowText1.c_str());

        const auto armNode = f4vr::findAVObject(f4vr::getCommonNode(), "LArm_ForeArm3");
        const auto pipboyNode = f4vr::findNode(armNode, g_config.debugFlowText1.c_str());
        // const auto pipboyNode2 = f4vr::findNode(armNode, "PipboyRoot");
        // const auto pipboyNode = f4vr::findAVObject(commonNode, "PipboyBone");

        if (!pipboyNode) {
            logger::sample("No pipboy node found");
            return;
        }

        const auto hmdNifNode = f4vr::findNode(pn->UprightHmdLagNode, "world_HMD_info.nif");

        relocateHUDElement("location_", hmdNifNode, pipboyNode);
        relocateHUDElement("message_", hmdNifNode, pipboyNode);
        relocateHUDElement("stealth_", hmdNifNode, pipboyNode);
        relocateHUDElement("questAnim_", hmdNifNode, pipboyNode);
        relocateHUDElement("condition_", hmdNifNode, pipboyNode);
        relocateHUDElement("quest_", hmdNifNode, pipboyNode);
        relocateHUDElement("lowBattery_", hmdNifNode, pipboyNode);
        relocateHUDElement("IHUD_debug_sphere", hmdNifNode, pipboyNode);

        // relocateHUDElement("IHUD_debug_sphere", armNode, pipboyNode);

        const auto node = f4vr::findAVObjectStartsWith(hmdNifNode, "IHUD_debug_sphere");
        node->local.scale = g_config.debugFlowFlag1;
    }

    void ImmersiveHUD::relocateHUDElement(const std::string& elementName, RE::NiAVObject* rootNode, const RE::NiAVObject* toNode)
    {
        const auto node = f4vr::findAVObjectStartsWith(rootNode, elementName.c_str());

        if (!node) {
            using namespace std::string_literals;
            logger::sample(10000, fmt::runtime("No node found: "s + std::string(elementName)));
            return;
        }

        // logger::sample(5000, fmt::runtime("Relocating HUD element: "s + std::string(node->name)));

        // revert to original transform
        node->local.rotate = getIdentityMatrix();
        node->local.translate = RE::NiPoint3(0, 0, 0);

        // update world transforms after reverting to original
        f4vr::updateTransforms(node);

        // calculate relocation transform and set to local
        node->local = calculateRelocation(node, toNode, { 0, g_config.debugFlowFlag2, g_config.debugFlowFlag3 });
    }
}
