#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <chrono>
#include <thread>
#include <dos.h>
#include "memory.hpp"


namespace offsets {
    constexpr auto local_player = 0xDEA964;
    constexpr auto entity_list = 0x4DFFF14;
    constexpr auto view_matrix = 0x4DF0D44;
    constexpr auto dwClientState = 0x59F194;
    constexpr auto Jump = 0x52BBC9C;
    constexpr auto dwRadarBase = 0x52369EC;
    constexpr auto ForceAttack = 0x322DD10;
    constexpr auto dwGlowObjectManager = 0x535A9D8;
    constexpr auto model_ambient_min = 0x5A118C;
    constexpr auto force_update_spectator_glow = 0x3D91CA;
    constexpr auto r3dSky = 0xDC48A0;

    constexpr auto m_iFov = 0x31F4;
    constexpr auto bone_matrix = 0x26A8;
    constexpr auto team_num = 0xF4;
    constexpr auto life_state = 0x25F;
    constexpr auto origin = 0x138;
    constexpr auto dormant = 0xED;
    constexpr auto Health = 0x100;
    constexpr auto Crosshair = 0x11838;
    constexpr auto Scoped_1 = 0x9974;
    constexpr auto m_zoomLevel = 0x33E0;
    constexpr auto MyWeapons = 0x2E08;
    constexpr auto m_iItemDefinitionIndex = 0x2FBA;
    constexpr auto ActiveWeapon = 0x2F08;
    constexpr auto Flags = 0x104;
    constexpr auto dwClientState_ViewAngles = 0x4D90;
    constexpr auto m_aimPunchAngle = 0x303C;
    constexpr auto m_iShotsFired = 0x103E0;
    constexpr auto ModelIndex = 0x258;
    constexpr auto dwWeaponTableIndex = 0x326C;
    constexpr auto m_hViewModel = 0x3308;
    constexpr auto dwClientState_PlayerInfo = 0x52C0;
    constexpr auto m_clrRender = 0x70;
    constexpr auto glowIndex = 0x10488;
    constexpr auto m_iObserverMode = 0x3388;
    constexpr auto m_thirdPersonViewAngles = 0x31E8;
    constexpr auto ViewmodelFov = 0xCD5624;
    constexpr auto m_bUseCustomAutoExposureMin = 0x9D8;
    constexpr auto m_bUseCustomAutoExposureMax = 0x9D9;
    constexpr auto m_flCustomAutoExposureMin = 0x9DC;
    constexpr auto m_flCustomAutoExposureMax = 0x9E0;
}

DWORD pid = memory::get_process_id(L"csgo.exe");
const DWORD client = memory::get_module_address(pid, L"client.dll");
const auto engine = memory::get_module_address(pid, L"engine.dll");
const HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
const auto LocalP = memory::read<DWORD>(handle, client + offsets::local_player);
DWORD LocalPlayer;




using namespace std;
using namespace std::chrono;


class Cham
{
public:
    uint8_t red_;
    uint8_t green_;
    uint8_t blue_;
    uint8_t alpha_;

    Cham() = default;
    constexpr Cham(uint8_t new_red, uint8_t new_green, uint8_t new_blue, uint8_t new_alpha) : red_(new_red), green_(new_green), blue_(new_blue), alpha_(new_alpha)
    {
        //empty
    }
};


struct glow_t
{
    int next_free_slot_;
    DWORD entity_address_;
    float red;
    float green_;
    float blue_;
    float alpha_;

    uint32_t pad0_[2];

    float bloom_amount_;

    uint32_t u1_;

    bool render_occluded_;
    bool render_unocculuded_;

    bool full_bloom_render_;
    uint8_t pad1_;

    int full_bloom_stencil_test_value_;
    int glow_style_; //0 full-body  1 inline && flicker  2 inline glow  3 flicker
    int split_screen_shot_;
};



struct Vector {
    Vector() noexcept
        : x(), y(), z() {}

    Vector(float x, float y, float z) noexcept
        : x(x), y(y), z(z) {}

    Vector& operator+(const Vector& v) noexcept {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector& operator-(const Vector& v) noexcept {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    float x, y, z;
};


struct Vector2 {
    float x = { }; float y = { };
};

struct ViewMatrix {
    ViewMatrix() noexcept
        : data() {}

    float* operator[](int index) noexcept {
        return data[index];
    }

    const float* operator[](int index) const noexcept {
        return data[index];
    }

    float data[4][4];
};

static bool world_to_screen(const Vector& world, Vector& screen, const ViewMatrix& vm) noexcept {
    float w = vm[3][0] * world.x + vm[3][1] * world.y + vm[3][2] * world.z + vm[3][3];

    if (w < 0.001f) {
        return false;
    }

    const float x = world.x * vm[0][0] + world.y * vm[0][1] + world.z * vm[0][2] + vm[0][3];
    const float y = world.x * vm[1][0] + world.y * vm[1][1] + world.z * vm[1][2] + vm[1][3];

    w = 1.f / w;
    float nx = x * w;
    float ny = y * w;

    const ImVec2 size = ImGui::GetIO().DisplaySize;

    screen.x = (size.x * 0.5f * nx) + (nx + size.x * 0.5f);
    screen.y = -(size.y * 0.5f * ny) + (ny + size.y * 0.5f);

    return true;
}
static bool world_to_screen2(const Vector2& world, Vector2& screen, const ViewMatrix& vm) noexcept {
    float w = vm[3][0] * world.x + vm[3][1] * world.y + vm[3][2];

    if (w < 0.001f) {
        return false;
    }

    const float x = world.x * vm[0][0] + world.y * vm[0][1];
    const float y = world.x * vm[1][0] + world.y * vm[1][1];

    w = 1.f / w;
    float nx = x * w;
    float ny = y * w;

    const ImVec2 size = ImGui::GetIO().DisplaySize;

    screen.x = (size.x * 0.5f * nx) + (nx + size.x * 0.5f);
    screen.y = -(size.y * 0.5f * ny) + (ny + size.y * 0.5f);

    return true;
}






