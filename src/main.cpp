#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>

using namespace geode::prelude;

namespace {
    bool g_perfApplied = false;
    bool g_prevPerfMode = false;

    // Cached so per-frame hooks do not do a settings lookup every call.
    bool g_noParticles = true;
    bool g_noShaders = true;

    bool setting(char const* key) {
        return Mod::get()->getSettingValue<bool>(key);
    }

    void purgeMemory() {
        // Frees textures nothing is using anymore.
        CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    }

    void restorePerformanceMode() {
        if (g_perfApplied) {
            GameManager::get()->m_performanceMode = g_prevPerfMode;
            g_perfApplied = false;
        }
    }

    // Object IDs of shader triggers in GD 2.2.
    // If a shader still shows up in some level, add its trigger ID here
    // (check the Geometry Dash wiki for the full object ID list).
    bool isShaderTrigger(int id) {
        return (id >= 2904 && id <= 2917) || (id >= 2919 && id <= 2923);
    }
}

class $modify(FixLagPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (setting("purge-on-load")) {
            purgeMemory();
        }

        if (setting("performance-mode")) {
            auto gm = GameManager::get();
            if (!g_perfApplied) {
                g_prevPerfMode = gm->m_performanceMode;
                g_perfApplied = true;
            }
            gm->m_performanceMode = true;
        }

        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            restorePerformanceMode();
            return false;
        }
        return true;
    }

    void addObject(GameObject* obj) {
        // Shader triggers are never added to the level, so no shader runs.
        if (g_noShaders && obj && isShaderTrigger(obj->m_objectID)) {
            return;
        }
        PlayLayer::addObject(obj);
    }

    void onQuit() {
        PlayLayer::onQuit();
        restorePerformanceMode();
        if (setting("purge-on-quit")) {
            purgeMemory();
        }
    }
};

class $modify(FixLagBaseGameLayer, GJBaseGameLayer) {
    void shakeCamera(float duration, float strength, float interval) {
        if (setting("disable-shake")) {
            return;
        }
        GJBaseGameLayer::shakeCamera(duration, strength, interval);
    }
};

// Skips drawing particles, but only while inside a level so menus look normal.
class $modify(FixLagParticles, CCParticleSystemQuad) {
    void draw() {
        if (g_noParticles && PlayLayer::get()) {
            return;
        }
        CCParticleSystemQuad::draw();
    }
};

$on_mod(Loaded) {
    g_noParticles = setting("disable-particles");
    g_noShaders = setting("disable-shaders");

    listenForSettingChanges<bool>("disable-particles", [](bool value) {
        g_noParticles = value;
    });
    listenForSettingChanges<bool>("disable-shaders", [](bool value) {
        g_noShaders = value;
    });

    log::info("Fix Lag Super loaded");
}
