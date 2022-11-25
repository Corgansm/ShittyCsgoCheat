#include <iostream>
#include <format>

#include "memory.hpp"
#include "Main.h"


#include <Windows.h>
#include <d3d11.h>
#include <dwmapi.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <chrono>
#include <thread>



using namespace std;
using namespace std::chrono;

namespace offsets {
    constexpr auto local_player = 0xDE7964;
    constexpr auto entity_list = 0x4DFCE74;
    constexpr auto view_matrix = 0x4DEDCA4;
    constexpr auto dwClientState = 0x59F194;
    constexpr auto m_iFov = 0x31F4;

    constexpr auto bone_matrix = 0x26A8;
    constexpr auto team_num = 0xF4;
    constexpr auto life_state = 0x25F;
    constexpr auto origin = 0x138;
    constexpr auto dormant = 0xED;
    constexpr auto Health = 0x100;
    constexpr auto Crosshair = 0x11838;
    constexpr auto ForceAttack = 0x322AC7C;
    constexpr auto Scoped_1 = 0x9974;
    constexpr auto m_zoomLevel = 0x33E0;
    constexpr auto MyWeapons = 0x2E08;
    constexpr auto m_iItemDefinitionIndex = 0x2FBA;
    constexpr auto ActiveWeapon = 0x2F08;
    constexpr auto Flags = 0x104;
    constexpr auto Jump = 0x52B8BFC;
    constexpr auto dwClientState_ViewAngles = 0x4D90;
    constexpr auto m_aimPunchAngle = 0x303C;
    constexpr auto m_iShotsFired = 0x103E0;
    constexpr auto ModelIndex = 0x258;

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








// application entry point
INT APIENTRY WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show) {
    // allocate this program a console
    if (!AllocConsole()) {
        return FALSE;
    }









    // redirect stream IO
    FILE* file{ nullptr };
    freopen_s(&file, "CONIN$", "r", stdin);
    freopen_s(&file, "CONOUT$", "w", stdout);
    freopen_s(&file, "CONOUT$", "w", stderr);

    // try get the process ID
    DWORD pid = memory::get_process_id(L"csgo.exe");

    // wait for the game
    if (!pid) {
        std::cout << "Waiting for CS:GO...\n";

        do {
            pid = memory::get_process_id(L"csgo.exe");
            Sleep(200UL);
        } while (!pid);
    }

    std::cout << std::format("Game found. Process ID = {}\n", pid);

    const DWORD client = memory::get_module_address(pid, L"client.dll");
    const auto engine = memory::get_module_address(pid, L"engine.dll");

    if (!client) {
        std::cout << "Failed to get game module.\n";
        FreeConsole();
        return FALSE;
    }

    std::cout << std::format("Client -> {:#x}\n", client);
    std::cout << "" << endl;
    std::cout << "" << endl;
    std::cout << "" << endl;
    std::cout << "" << endl;

    std::cout <<
        "   ______      ______                    _____                                                            " << endl <<
        " .~      ~.  .~      ~.  |`````````,  .-~     ~.        .'.       |..          |                          " << endl <<
        "|           |          | |'''|'''''  :                .''```.     |  ``..      |                          " << endl <<
        "|           |          | |    `.     :     _____    .'       `.   |      ``..  |                          " << endl <<
        " `.______.'  `.______.'  |      `.    `-._____.'| .'           `. |          ``|                          " << endl <<
        "                                                                                                          " << endl <<
        " ____                                     ____                                                            " << endl <<
        "|             ``..      ..'' `````|````` |            |`````````, |..          |       .'.       |        " << endl <<
        "|______           ``..''          |      |______      |'''|'''''  |  ``..      |     .''```.     |        " << endl <<
        "|                 ..'`..          |      |            |    `.     |      ``..  |   .'       `.   |        " << endl <<
        "|___________  ..''      ``..      |      |___________ |      `.   |          ``| .'           `. |_______ ";
    std::cout << "" << endl;
    std::cout << "" << endl;
    std::cout << "" << endl;
    std::cout << "" << endl;


    //* // just add a flash at the start to get the code back

    std::cout << "Would you like a crosshair? 0 No, 1 Yes" << endl;

    std::cin >> crosshair;

    std::cout << "Thanks! Your crosshair is " << crosshair << endl;
    std::cout << "" << endl;
    Sleep(200);


    std::cout << "Would you like Box ESP?" << endl;

    std::cin >> BoxESP;

    std::cout << "Thanks! Your Box ESP is " << BoxESP << endl;
    std::cout << "" << endl;

    Sleep(200);


    std::cout << "Would you like a Head Dot?" << endl;

    std::cin >> Headdot;

    std::cout << "Thanks! Your Head Dot is " << Headdot << endl;
    std::cout << "" << endl;
    Sleep(200);


    std::cout << "Would you like to change your fov?" << endl;
    std::cout << "" << endl;
    std::cin >> FOV;

    if (FOV == 1) {
        std::cout << "Enter your Fov" << endl;
        std::cin >> FOV2;
        std::cout << "Thanks! Your FOV is " << FOV2 << endl;
        std::cout << "" << endl;
        std::cout << "Enter your Scoped Fov" << endl;
        std::cin >> FOV3;

    }




    Sleep(200);
    //*/
    std::cout << "Restart to change your settings..." << endl;













    if (!FreeConsole()) {
        return FALSE;
    }

    const HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (!handle) {
        return FALSE;
    }

    // create our window class to specify options for our window
    const WNDCLASSEXW wc{
        .cbSize = sizeof(WNDCLASSEXW),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = window_procedure,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = instance,
        .hIcon = nullptr,
        .hCursor = nullptr,
        .hbrBackground = nullptr,
        .lpszMenuName = nullptr,
        .lpszClassName = L"External Overlay Class",
        .hIconSm = nullptr
    };

    // register it and make sure it succeeded
    if (!RegisterClassExW(&wc)) {
        return FALSE;
    }

    // create our window
    const HWND window = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
        wc.lpszClassName,
        L"External Overlay",
        WS_POPUP,
        0,
        0,
        1920,
        1080,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );

    // make sure the window was created successfully
    if (!window) {
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return FALSE;
    }

    // set the window's opacity
    if (!SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA)) {
        DestroyWindow(window);
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return FALSE;
    }

    {
        RECT client_area{};
        if (!GetClientRect(window, &client_area)) {
            DestroyWindow(window);
            UnregisterClassW(wc.lpszClassName, wc.hInstance);
            return FALSE;
        }

        RECT window_area{};
        if (!GetWindowRect(window, &window_area)) {
            DestroyWindow(window);
            UnregisterClassW(wc.lpszClassName, wc.hInstance);
            return FALSE;
        }

        POINT diff{};
        if (!ClientToScreen(window, &diff)) {
            DestroyWindow(window);
            UnregisterClassW(wc.lpszClassName, wc.hInstance);
            return FALSE;
        }

        const MARGINS margins{
            window_area.left + (diff.x - window_area.left),
            window_area.top + (diff.y - window_area.top),
            client_area.right,
            client_area.bottom
        };

        if (FAILED(DwmExtendFrameIntoClientArea(window, &margins))) {
            DestroyWindow(window);
            UnregisterClassW(wc.lpszClassName, wc.hInstance);
            return FALSE;
        }
    }

    // create our directx swap chain description
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferDesc.Width = 0U;
    sd.BufferDesc.Height = 0U;
    sd.BufferDesc.RefreshRate.Numerator = 30U;
    sd.BufferDesc.RefreshRate.Denominator = 1U;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    sd.SampleDesc.Count = 1U;
    sd.SampleDesc.Quality = 0U;

    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2U;
    sd.OutputWindow = window;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    constexpr D3D_FEATURE_LEVEL feature_levels[2]{
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0
    };

    D3D_FEATURE_LEVEL feature_level{};

    // directx variables
    ID3D11Device* device{ nullptr };
    ID3D11DeviceContext* device_context{ nullptr };
    IDXGISwapChain* swap_chain{ nullptr };
    ID3D11RenderTargetView* render_target_view{ nullptr };

    // create the swap chain & device
    if (FAILED(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0U,
        feature_levels,
        2U,
        D3D11_SDK_VERSION,
        &sd,
        &swap_chain,
        &device,
        &feature_level,
        &device_context))) {
        DestroyWindow(window);
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return FALSE;
    }

    ID3D11Texture2D* back_buffer{ nullptr };

    if (FAILED(swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer)))) {
        return FALSE;
    }

    // create our render target
    if (FAILED(device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view))) {
        return FALSE;
    }

    if (back_buffer) {
        back_buffer->Release();
    }

    // tell windows to show this window
    ShowWindow(window, cmd_show);
    UpdateWindow(window);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(device, device_context);

    bool running = true;
    while (running) {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                running = false;
            }
        }

        if (!running) {
            break;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();


        const auto local_player = memory::read<DWORD>(handle, client + offsets::local_player);
        if (local_player) {
            const auto local_team = memory::read<int>(handle, local_player + offsets::team_num);
            const auto view_matrix = memory::read<ViewMatrix>(handle, client + offsets::view_matrix);

            for (int i = 1; i < 32; ++i) {
                const auto player = memory::read<DWORD>(handle, client + offsets::entity_list + i * 0x10);


                if (!player) {
                    continue;
                }

                DWORD LocalPlayer;
                ReadProcessMemory(handle, (LPVOID)(client + offsets::local_player), &LocalPlayer, sizeof(DWORD), 0);
                int LocalPlayerHealth;
                ReadProcessMemory(handle, (LPVOID)(LocalPlayer + offsets::Health), &LocalPlayerHealth, sizeof(int), 0);

                if (LocalPlayerHealth <= 0) {
                    continue;
                }

                const auto bones = memory::read<DWORD>(handle, player + offsets::bone_matrix);

                if (!bones) {
                    continue;
                }


                Vector head_pos{
                    memory::read<float>(handle, bones + 0x30 * 8 + 0x0C),
                    memory::read<float>(handle, bones + 0x30 * 8 + 0x1C),
                    memory::read<float>(handle, bones + 0x30 * 8 + 0x2C)
                };

                Vector top;
                Vector bottom;


                const auto LocalP = memory::read<DWORD>(handle, client + offsets::local_player);
                const auto EntityP = player;

                Vector local_pos{
                    memory::read<float>(handle, LocalP + offsets::origin),
                    memory::read<float>(handle, LocalP + offsets::origin + 0x4),
                    memory::read<float>(handle, LocalP + offsets::origin + 0x8),
                };

                Vector entity_pos{
                    memory::read<float>(handle, EntityP + offsets::origin),
                    memory::read<float>(handle, EntityP + offsets::origin + 0x4),
                    memory::read<float>(handle, EntityP + offsets::origin + 0x8),
                };


                float _dist = (float)trunc(sqrt(pow((entity_pos.x - local_pos.x), 2) + pow((entity_pos.y - local_pos.y), 2)) / 100);


                if (crosshair == 1) {
                    {
                        ImGui::GetBackgroundDrawList()->AddRectFilled({ 962, 539 }, { 968 , 541 }, ImColor(.8f, 0.f, 0.f));
                        ImGui::GetBackgroundDrawList()->AddRectFilled({ 958, 539 }, { 952 , 541 }, ImColor(.8f, 0.f, 0.f));
                        ImGui::GetBackgroundDrawList()->AddRectFilled({ 959, 537 }, { 961, 532 }, ImColor(.8f, 0.f, 0.f));
                        ImGui::GetBackgroundDrawList()->AddRectFilled({ 959, 543 }, { 961, 548 }, ImColor(.8f, 0.f, 0.f));

                    }

                    if (FOV == 1) {

                        DWORD LocalBase = memory::read<DWORD>(handle, client + offsets::local_player);
                        DWORD Scoped = memory::read<DWORD>(handle, LocalP + offsets::Scoped_1);
                        

                        auto zoom = memory::read<int>(handle, LocalPlayer + offsets::m_zoomLevel);
                        if (Scoped) {

                            memory::write(handle, LocalBase + offsets::m_iFov, FOV3);
                        }
                        else {
                            memory::write(handle, LocalBase + offsets::m_iFov, FOV2);
                        }
                    }









                    if (memory::read<bool>(handle, player + offsets::dormant)) {
                        continue;
                    }

                    if (memory::read<int>(handle, player + offsets::team_num) == local_team) {
                        continue;
                    }

                    if (memory::read<int>(handle, player + offsets::life_state) != 0) {
                        continue;
                    }












                    auto feet_pos = memory::read<Vector>(handle, player + offsets::origin);






                    if (Headdot == 1) {
                        if (world_to_screen(head_pos + Vector{ 0, 0, 0.f }, top, view_matrix) && world_to_screen(head_pos - Vector{ 0, 0, 0.f }, bottom, view_matrix)) {
                            const float h = bottom.y - top.y;
                            const float w = h * 0.35f;
                            const float distance = _dist * 0.0254f;


                            ImGui::GetBackgroundDrawList()->AddCircle({ top.x - w, top.y }, static_cast<int>(1.5 / sqrt(distance)), ImColor(1.f, 1.f, 1.f));

                        }
                    }
                    if (BoxESP == 1) {
                        if (world_to_screen(head_pos + Vector{ 0, 0, 11.f }, top, view_matrix) && world_to_screen(feet_pos - Vector{ 0, 0, 7.f }, bottom, view_matrix)) {
                            const float h = bottom.y - top.y;
                            const float w = h * 0.35f;


                            ImGui::GetBackgroundDrawList()->AddRect({ top.x - w, top.y }, { top.x + w, bottom.y }, ImColor(1.f, 1.f, 1.f));
                        }
                    }
                    if (bhop == 1) {
                        const auto& flags = memory::read<bool>(handle, LocalPlayer + offsets::Flags);


                        if (GetAsyncKeyState(VK_SPACE)) {


                            time_t updateTimer = 0;
                            time_t curtime = time(NULL);

                            updateTimer = curtime + 1;

                            if (flags & (1 << 0) && rand() % 100 + 1 <= 100)
                                memory::write<BYTE>(handle, client + offsets::Jump, 6);
                        }
                    }

                    if (skeleton == 1) {
                        //bones
                        const auto playermodel = memory::read<int>(handle, player + offsets::ModelIndex);
                        Vector head_pos{
                            memory::read<float>(handle, bones + 0x30 * 8 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 8 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 8 + 0x2C),
                        };

                        Vector neck_pos{
                            memory::read<float>(handle, bones + 0x30 * 7 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 7 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 7 + 0x2C),
                        };

                        Vector chest_pos{
                            memory::read<float>(handle, bones + 0x30 * 6 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 6 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 6 + 0x2C),
                        };
                        Vector chest1_pos{
                            memory::read<float>(handle, bones + 0x30 * 5 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 5 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 5 + 0x2C),
                        };
                        Vector chest2_pos{
                            memory::read<float>(handle, bones + 0x30 * 4 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 4 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 4 + 0x2C),
                        };
                        Vector bully_pos{
                            memory::read<float>(handle, bones + 0x30 * 3 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 3 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 3 + 0x2C),
                        };
                        Vector bully1_pos{
                            memory::read<float>(handle, bones + 0x30 * 0 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 0 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 0 + 0x2C),
                        };
                        Vector rightleg_pos{
                            memory::read<float>(handle, bones + 0x30 * 70 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 70 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 70 + 0x2C),
                        };
                        Vector leftleg_pos{
                            memory::read<float>(handle, bones + 0x30 * 77 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 77 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 77 + 0x2C),
                        };
                        Vector rightleg1_pos{
                            memory::read<float>(handle, bones + 0x30 * 71 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 71 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 71 + 0x2C),
                        };
                        Vector rightleg5_pos{
                            memory::read<float>(handle, bones + 0x30 * 72 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 72 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 72 + 0x2C),
                        };
                        Vector leftleg1_pos{
                            memory::read<float>(handle, bones + 0x30 * 78 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 78 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 78 + 0x2C),
                        };
                        Vector leftleg5_pos{
                            memory::read<float>(handle, bones + 0x30 * 79 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 79 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 79 + 0x2C),
                        };
                        Vector rightleg2_pos{
                            memory::read<float>(handle, bones + 0x30 * 66 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 66 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 66 + 0x2C),
                        };
                        Vector leftleg2_pos{
                            memory::read<float>(handle, bones + 0x30 * 73 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 73 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 73 + 0x2C),
                        };
                        Vector leftleg3_pos{
                            memory::read<float>(handle, bones + 0x30 * 74 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 74 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 74 + 0x2C),
                        };
                        Vector leftleg4_pos{
                            memory::read<float>(handle, bones + 0x30 * 75 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 75 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 75 + 0x2C),
                        };
                        Vector leftleg6_pos{
                            memory::read<float>(handle, bones + 0x30 * 76 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 76 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 76 + 0x2C),
                        };
                        Vector rightleg3_pos{
                            memory::read<float>(handle, bones + 0x30 * 67 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 67 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 67 + 0x2C),
                        };
                        Vector rightleg4_pos{
                            memory::read<float>(handle, bones + 0x30 * 68 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 68 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 68 + 0x2C),
                        };
                        Vector rightleg6_pos{
                            memory::read<float>(handle, bones + 0x30 * 69 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 69 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 69 + 0x2C),
                        };
                        Vector rightarm_pos{
                            memory::read<float>(handle, bones + 0x30 * 11 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 11 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 11 + 0x2C),
                        };
                        Vector rightarm1_pos{
                            memory::read<float>(handle, bones + 0x30 * 12 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 12 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 12 + 0x2C),
                        };
                        Vector rightarm2_pos{
                            memory::read<float>(handle, bones + 0x30 * 13 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 13 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 13 + 0x2C),
                        };
                        Vector leftarm_pos{
                            memory::read<float>(handle, bones + 0x30 * 41 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 41 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 41 + 0x2C),
                        };
                        Vector leftarm1_pos{
                            memory::read<float>(handle, bones + 0x30 * 42 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 42 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 42 + 0x2C),
                        };
                        Vector leftarm2_pos{
                            memory::read<float>(handle, bones + 0x30 * 43 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 43 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 43 + 0x2C),
                        };
                        Vector rightshoulder_pos{
                            memory::read<float>(handle, bones + 0x30 * 39 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 39 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 39 + 0x2C),
                        };
                        Vector righthand_pos{
                            memory::read<float>(handle, bones + 0x30 * 65 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 65 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 65 + 0x2C),
                        };
                        Vector leftpelvis_pos{
                            memory::read<float>(handle, bones + 0x30 * 86 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 86 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 86 + 0x2C),
                        };
                        Vector leftknee_pos{
                            memory::read<float>(handle, bones + 0x30 * 82 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 82 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 82 + 0x2C),
                        };
                        Vector leftfoot_pos{
                            memory::read<float>(handle, bones + 0x30 * 83 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 83 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 83 + 0x2C),
                        };
                        Vector leftfoot1_pos{
                            memory::read<float>(handle, bones + 0x30 * 84 + 0x0C),
                            memory::read<float>(handle, bones + 0x30 * 84 + 0x1C),
                            memory::read<float>(handle, bones + 0x30 * 84 + 0x2C),
                        };


                        //Vectors
                        Vector top;
                        Vector bottom;
                        Vector neck;
                        Vector chest;
                        Vector chest1;
                        Vector chest2;
                        Vector bully;
                        Vector bully1;
                        Vector rightleg;
                        Vector rightleg1;
                        Vector rightleg2;
                        Vector leftleg;
                        Vector leftleg1;
                        Vector leftleg2;
                        Vector leftleg3;
                        Vector leftleg4;
                        Vector leftleg5;
                        Vector leftleg6;
                        Vector rightleg3;
                        Vector rightleg4;
                        Vector rightleg5;
                        Vector rightarm;
                        Vector rightarm1;
                        Vector rightarm2;
                        Vector leftarm;
                        Vector leftarm1;
                        Vector leftarm2;
                        Vector head;
                        Vector life_vector;
                        Vector rightshoulder;
                        Vector righthand;
                        Vector rightleg6;
                        Vector leftpelvis;
                        Vector leftknee;
                        Vector leftfoot;
                        Vector leftfoot1;

                        //world_to_screen

                        if (world_to_screen(head_pos + Vector{ 0, 0, 8.f }, top, view_matrix) && world_to_screen(feet_pos + Vector{ 0, 0, 0.f }, bottom, view_matrix) && world_to_screen(leftfoot1_pos, leftfoot1, view_matrix) && world_to_screen(leftfoot_pos, leftfoot, view_matrix) && world_to_screen(leftknee_pos, leftknee, view_matrix) && world_to_screen(leftpelvis_pos, leftpelvis, view_matrix) && world_to_screen(leftleg6_pos, leftleg6, view_matrix) && world_to_screen(rightleg6_pos, rightleg6, view_matrix) && world_to_screen(righthand_pos, righthand, view_matrix) && world_to_screen(rightshoulder_pos, rightshoulder, view_matrix) && world_to_screen(leftleg5_pos, leftleg5, view_matrix) && world_to_screen(rightleg5_pos, rightleg5, view_matrix) && world_to_screen(neck_pos, neck, view_matrix) && world_to_screen(chest_pos, chest, view_matrix) && world_to_screen(chest1_pos, chest1, view_matrix) && world_to_screen(chest2_pos, chest2, view_matrix) && world_to_screen(bully_pos, bully, view_matrix) && world_to_screen(bully1_pos, bully1, view_matrix) && world_to_screen(rightleg_pos, rightleg, view_matrix) && world_to_screen(leftleg_pos, leftleg, view_matrix) && world_to_screen(rightleg1_pos, rightleg1, view_matrix) && world_to_screen(leftleg1_pos, leftleg1, view_matrix) && world_to_screen(rightleg2_pos, rightleg2, view_matrix) && world_to_screen(leftleg2_pos, leftleg2, view_matrix) && world_to_screen(leftleg3_pos, leftleg3, view_matrix) && world_to_screen(leftleg4_pos, leftleg4, view_matrix) && world_to_screen(rightleg3_pos, rightleg3, view_matrix) && world_to_screen(rightleg4_pos, rightleg4, view_matrix) && world_to_screen(rightarm_pos, rightarm, view_matrix) && world_to_screen(rightarm1_pos, rightarm1, view_matrix) && world_to_screen(rightarm2_pos, rightarm2, view_matrix) && world_to_screen(leftarm_pos, leftarm, view_matrix) && world_to_screen(leftarm1_pos, leftarm1, view_matrix) && world_to_screen(leftarm2_pos, leftarm2, view_matrix) && world_to_screen(head_pos + Vector{ 2, 0, -8.f }, head, view_matrix)) {

                            //Skeletons

                            if (skeleton)
                            {
                                //ct = 349 *
                                // 340 *
                                // 351 *
                                // 353 *
                                // 344 *
                                //terrorist = 355 *
                                //347 *

                                if (playermodel == 349 || playermodel == 355)
                                {
                                    ImGui::GetBackgroundDrawList()->AddLine({ neck.x, neck.y }, { chest.x, chest.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { chest1.x, chest1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { head.x, head.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest1.x, chest1.y }, { chest2.x, chest2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest2.x, chest2.y }, { bully.x, bully.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { bully1.x, bully1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { rightleg.x, rightleg.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { leftleg.x, leftleg.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { rightarm.x, rightarm.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm.x, rightarm.y }, { rightarm1.x, rightarm1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm1.x, rightarm1.y }, { rightarm2.x, rightarm2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { leftarm.x, leftarm.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftarm.x, leftarm.y }, { leftarm1.x, leftarm1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftarm1.x, leftarm1.y }, { leftarm2.x, leftarm2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg.x, rightleg.y }, { rightleg1.x, rightleg1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg1.x, rightleg1.y }, { rightleg2.x, rightleg2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg2.x, rightleg2.y }, { rightleg3.x, rightleg3.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg3.x, rightleg3.y }, { rightleg4.x, rightleg4.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg.x, leftleg.y }, { leftleg1.x, leftleg1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg1.x, leftleg1.y }, { leftleg2.x, leftleg2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg2.x, leftleg2.y }, { leftleg3.x, leftleg3.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg3.x, leftleg3.y }, { leftleg4.x, leftleg4.y }, ImColor(255, 0, 0), 1.f);
                                }
                                else if (playermodel == 344 || playermodel == 103 || playermodel == 101 || playermodel == 102 || playermodel == 104 || playermodel == 98)
                                {
                                    ImGui::GetBackgroundDrawList()->AddLine({ neck.x, neck.y }, { chest.x, chest.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { chest1.x, chest1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { head.x, head.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest1.x, chest1.y }, { chest2.x, chest2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest2.x, chest2.y }, { bully.x, bully.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { bully1.x, bully1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { rightleg.x, rightleg.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { leftleg.x, leftleg.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { rightarm.x, rightarm.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm.x, rightarm.y }, { rightarm1.x, rightarm1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm1.x, rightarm1.y }, { rightarm2.x, rightarm2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { leftarm.x, leftarm.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftarm.x, leftarm.y }, { leftarm1.x, leftarm1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftarm1.x, leftarm1.y }, { leftarm2.x, leftarm2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg.x, rightleg.y }, { rightleg1.x, rightleg1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg1.x, rightleg1.y }, { rightleg5.x, rightleg5.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg.x, leftleg.y }, { leftleg1.x, leftleg1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg1.x, leftleg1.y }, { leftleg5.x, leftleg5.y }, ImColor(255, 0, 0), 1.f);
                                }
                                else if (playermodel == 353)
                                {
                                    ImGui::GetBackgroundDrawList()->AddLine({ neck.x, neck.y }, { chest.x, chest.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { chest1.x, chest1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { head.x, head.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest1.x, chest1.y }, { chest2.x, chest2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest2.x, chest2.y }, { bully.x, bully.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { bully1.x, bully1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { leftleg2.x, leftleg2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { leftleg3.x, leftleg3.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { rightarm.x, rightarm.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm.x, rightarm.y }, { rightarm1.x, rightarm1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm1.x, rightarm1.y }, { rightarm2.x, rightarm2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { leftarm.x, leftarm.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftarm.x, leftarm.y }, { leftarm1.x, leftarm1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftarm1.x, leftarm1.y }, { leftarm2.x, leftarm2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg2.x, leftleg2.y }, { rightleg4.x, rightleg4.y }, ImColor(255, 0, 0), 1.f); //rodilla derecha
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg4.x, rightleg4.y }, { rightleg.x, rightleg.y }, ImColor(255, 0, 0), 1.f); //pie derecho
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg3.x, leftleg3.y }, { leftleg4.x, leftleg4.y }, ImColor(255, 0, 0), 1.f); //rodilla izquierda
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg4.x, leftleg4.y }, { leftleg.x, leftleg.y }, ImColor(255, 0, 0), 1.f); //pie izquierdo
                                }
                                else if (playermodel == 347)
                                {
                                    ImGui::GetBackgroundDrawList()->AddLine({ neck.x, neck.y }, { chest.x, chest.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { chest1.x, chest1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { head.x, head.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest1.x, chest1.y }, { chest2.x, chest2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest2.x, chest2.y }, { bully.x, bully.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { bully1.x, bully1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { rightleg2.x, rightleg2.y }, ImColor(255, 0, 0), 1.f); //pelvis derecha
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { leftleg3.x, leftleg3.y }, ImColor(255, 0, 0), 1.f); //pelvis izquierda
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { rightarm.x, rightarm.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm.x, rightarm.y }, { rightarm1.x, rightarm1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm1.x, rightarm1.y }, { rightarm2.x, rightarm2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { rightshoulder.x, rightshoulder.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightshoulder.x, rightshoulder.y }, { leftarm1.x, leftarm1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftarm1.x, leftarm1.y }, { righthand.x, righthand.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg2.x, rightleg2.y }, { rightleg3.x, rightleg3.y }, ImColor(255, 0, 0), 1.f); //rodilla derecha
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg3.x, rightleg3.y }, { rightleg.x, rightleg.y }, ImColor(255, 0, 0), 1.f); //pie derecho
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg3.x, leftleg3.y }, { leftleg4.x, leftleg4.y }, ImColor(255, 0, 0), 1.f); //rodilla izquierda
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg4.x, leftleg4.y }, { leftleg.x, leftleg.y }, ImColor(255, 0, 0), 1.f); //pie izquierdo
                                }
                                else if (playermodel == 351)
                                {
                                    ImGui::GetBackgroundDrawList()->AddLine({ neck.x, neck.y }, { chest.x, chest.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { chest1.x, chest1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { head.x, head.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest1.x, chest1.y }, { chest2.x, chest2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest2.x, chest2.y }, { bully.x, bully.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { bully1.x, bully1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { leftleg2.x, leftleg2.y }, ImColor(255, 0, 0), 1.f); //pelvis derecha
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg2.x, leftleg2.y }, { rightleg6.x, rightleg6.y }, ImColor(255, 0, 0), 1.f); //rodilla derecha
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { leftleg4.x, leftleg4.y }, ImColor(255, 0, 0), 1.f); //pelvis izquierda
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg4.x, leftleg4.y }, { leftleg6.x, leftleg6.y }, ImColor(255, 0, 0), 1.f); //rodilla izquierda
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg6.x, leftleg6.y }, { leftleg.x, leftleg.y }, ImColor(255, 0, 0), 1.f); //pie izquierdo1
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { rightarm.x, rightarm.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm.x, rightarm.y }, { rightarm1.x, rightarm1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm1.x, rightarm1.y }, { rightarm2.x, rightarm2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { leftarm.x, leftarm.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftarm.x, leftarm.y }, { leftarm1.x, leftarm1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftarm1.x, leftarm1.y }, { leftarm2.x, leftarm2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg6.x, rightleg6.y }, { rightleg.x, rightleg.y }, ImColor(255, 0, 0), 1.f); //pie derecho
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg.x, rightleg.y }, { rightleg1.x, rightleg1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg.x, leftleg.y }, { leftleg1.x, leftleg1.y }, ImColor(255, 0, 0), 1.f); //pie izquierdo2
                                }
                                else if (playermodel == 340)
                                {
                                    ImGui::GetBackgroundDrawList()->AddLine({ neck.x, neck.y }, { chest.x, chest.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { chest1.x, chest1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { head.x, head.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest1.x, chest1.y }, { chest2.x, chest2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest2.x, chest2.y }, { bully.x, bully.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { bully1.x, bully1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { leftleg.x, leftleg.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { leftpelvis.x, leftpelvis.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { rightarm.x, rightarm.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm.x, rightarm.y }, { rightarm1.x, rightarm1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm1.x, rightarm1.y }, { rightarm2.x, rightarm2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { leftarm.x, leftarm.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftarm.x, leftarm.y }, { leftarm1.x, leftarm1.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftarm1.x, leftarm1.y }, { leftarm2.x, leftarm2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg.x, leftleg.y }, { leftleg2.x, leftleg2.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg2.x, leftleg2.y }, { leftleg3.x, leftleg3.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg3.x, leftleg3.y }, { leftleg4.x, leftleg4.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftpelvis.x, leftpelvis.y }, { leftknee.x, leftknee.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftknee.x, leftknee.y }, { leftfoot.x, leftfoot.y }, ImColor(255, 0, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftfoot.x, leftfoot.y }, { leftfoot1.x, leftfoot1.y }, ImColor(255, 0, 0), 1.f);
                                }
                                else
                                {
                                    ImGui::GetBackgroundDrawList()->AddLine({ neck.x, neck.y }, { chest.x, chest.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { chest1.x, chest1.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { head.x, head.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest1.x, chest1.y }, { chest2.x, chest2.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest2.x, chest2.y }, { bully.x, bully.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { bully1.x, bully1.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { rightleg.x, rightleg.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ bully.x, bully.y }, { leftleg.x, leftleg.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { rightarm.x, rightarm.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm.x, rightarm.y }, { rightarm1.x, rightarm1.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightarm1.x, rightarm1.y }, { rightarm2.x, rightarm2.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ chest.x, chest.y }, { leftarm.x, leftarm.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftarm.x, leftarm.y }, { leftarm1.x, leftarm1.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftarm1.x, leftarm1.y }, { leftarm2.x, leftarm2.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg.x, rightleg.y }, { rightleg1.x, rightleg1.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg1.x, rightleg1.y }, { rightleg2.x, rightleg2.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg2.x, rightleg2.y }, { rightleg3.x, rightleg3.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ rightleg3.x, rightleg3.y }, { rightleg4.x, rightleg4.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg.x, leftleg.y }, { leftleg1.x, leftleg1.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg1.x, leftleg1.y }, { leftleg2.x, leftleg2.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg2.x, leftleg2.y }, { leftleg3.x, leftleg3.y }, ImColor(0, 255, 0), 1.f);
                                    ImGui::GetBackgroundDrawList()->AddLine({ leftleg3.x, leftleg3.y }, { leftleg4.x, leftleg4.y }, ImColor(0, 255, 0), 1.f);
                                }
                            }
                        }
                    }








            }
        }
    }
    

        ImGui::Render();



        constexpr float clear_color[4] = { 0.f, 0.f, 0.f, 0.f };
        device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
        device_context->ClearRenderTargetView(render_target_view, clear_color);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        swap_chain->Present(0U, 0U);



    }





    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (swap_chain) {
        swap_chain->Release();
        swap_chain = nullptr;
    }

    if (device_context) {
        device_context->Release();
        device_context = nullptr;
    }

    if (device) {
        device->Release();
        device = nullptr;
    }

    if (render_target_view) {
        render_target_view->Release();
        render_target_view = nullptr;
    }

    DestroyWindow(window);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);

    CloseHandle(handle);

    return TRUE;

    
}