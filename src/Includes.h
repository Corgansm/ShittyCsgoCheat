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
}

DWORD pid = memory::get_process_id(L"csgo.exe");
const DWORD client = memory::get_module_address(pid, L"client.dll");
const auto engine = memory::get_module_address(pid, L"engine.dll");
const HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
const auto LocalP = memory::read<DWORD>(handle, client + offsets::local_player);
DWORD LocalPlayer;




using namespace std;
using namespace std::chrono;






int GetWeaponId(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) // You could add dwBaseAddr and other params here to pass with the function or just call them separately in the function itself
{
    DWORD pid = memory::get_process_id(L"csgo.exe");
    const DWORD client = memory::get_module_address(pid, L"client.dll");
    const HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    DWORD dwBaseCombatWeaponIndex = memory::read<DWORD>(handle, client + offsets::dwWeaponTableIndex);
    dwBaseCombatWeaponIndex &= 0xFFF;

    DWORD dwBaseCombatWeapon = memory::read<DWORD>(handle, client + offsets::entity_list + dwBaseCombatWeaponIndex * 0x10) - 0x10;

    return memory::read<int>(handle, dwBaseCombatWeapon + 0x1148 + 0x40 + 0x194);
}





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

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK window_procedure(HWND window, UINT message, WPARAM w_param, LPARAM l_param) {
    if (ImGui_ImplWin32_WndProcHandler(window, message, w_param, l_param)) {
        return 1L;
    }

    switch (message) {
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0L;
    }
    }

    return DefWindowProc(window, message, w_param, l_param);
}

bool create_directx(HWND window) {

}



