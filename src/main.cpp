#include <iostream>
#include <format>

#include "includes.h"
#include "Main.h"

#include <Windows.h>
#include <d3d11.h>
#include <dwmapi.h>
#include <chrono>
#include <thread>
#include <dos.h>
#include <vector>





struct Color {
    constexpr Color(float r, float g, float b, float a = 1.f) noexcept :
        r(r), g(g), b(b), a(a) { }

    float r, g, b, a;
};








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


    // wait for the game
    if (!pid) {
        std::cout << "Waiting for CS:GO...\n";

        do {
            pid = memory::get_process_id(L"csgo.exe");
            Sleep(200UL);
        } while (!pid);
    }

    std::cout << std::format("Game found. Process ID = {}\n", pid);



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

    std::cout << "Would you like stream proof settings? 0 No, 1 Yes" << endl;
    int question;
    std::cin >> question;


    




    Sleep(200);
    //*/
    std::cout << "Restart to change your settings..." << endl;







    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

    





    if (!FreeConsole()) {
        return FALSE;
    }



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


        if (crosshair == 1) {
            const ImVec2 p0 = ImGui::GetItemRectMin();
            const ImVec2 p1 = ImGui::GetItemRectMax();
            const char* text_str = "Corgan's External", GetWeaponId();
            const ImVec2 text_pos = ImVec2(60, 40);
            ImGui::GetBackgroundDrawList()->AddText(text_pos, IM_COL32_WHITE, text_str);
        }

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
                    ImGui::GetBackgroundDrawList()->AddRectFilled({ 962, 539 }, { 968 , 541 }, ImColor(.8f, 0.f, 0.f));
                    ImGui::GetBackgroundDrawList()->AddRectFilled({ 958, 539 }, { 952 , 541 }, ImColor(.8f, 0.f, 0.f));
                    ImGui::GetBackgroundDrawList()->AddRectFilled({ 959, 537 }, { 961, 532 }, ImColor(.8f, 0.f, 0.f));
                    ImGui::GetBackgroundDrawList()->AddRectFilled({ 959, 543 }, { 961, 548 }, ImColor(.8f, 0.f, 0.f));
                }



                if (FOV == 1) {

                    DWORD LocalBase = memory::read<DWORD>(handle, client + offsets::local_player);
                    DWORD Scoped = memory::read<DWORD>(handle, LocalP + offsets::Scoped_1);
                    const auto& weapons = memory::read<DWORD>(handle, LocalPlayer + offsets::MyWeapons);


                    auto zoom = memory::read<int>(handle, LocalPlayer + offsets::m_zoomLevel);
                    if (!Scoped && (!GetAsyncKeyState(VK_XBUTTON2)))
                    {
                        memory::write(handle, LocalBase + offsets::m_iFov, FOV2);
                    }
                    if (!Scoped && (GetAsyncKeyState(VK_XBUTTON2)))
                    {
                        memory::write(handle, LocalBase + offsets::m_iFov, FOV3 * 3);
                    }
                    if (Scoped && (GetAsyncKeyState(VK_XBUTTON2)))
                    {
                        memory::write(handle, LocalBase + offsets::m_iFov, FOV3 / 2);
                    }

                }

                if (bhop == 1) {
                    if (GetAsyncKeyState(VK_SPACE)) {


                        time_t updateTimer = 0;
                        time_t curtime = time(NULL);

                        updateTimer = curtime + 1;

                        const auto& flags = memory::read<bool>(handle, LocalPlayer + offsets::Flags);
                        if (flags & (1 << 0) && rand() % 100 + 1 <= 100) {
                            memory::write<BYTE>(handle, client + offsets::Jump, 6);
                        }
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


                if (triggerbot == 1) {
                    if (GetAsyncKeyState(VK_XBUTTON1)) {

                        const auto& localHealth = memory::read<int32_t>(handle, LocalPlayer + offsets::Health);
                        const auto& localTeam = memory::read<uintptr_t>(handle, LocalPlayer + offsets::team_num);
                        const auto& shotsFired = memory::read<int32_t>(handle, LocalPlayer + offsets::m_iShotsFired);

                        time_t updateTimer = 0;
                        time_t curtime = time(NULL);
                        updateTimer = curtime + 1;
                        if (GetAsyncKeyState(VK_LBUTTON))
                        {
                            memory::write<uintptr_t>(handle, client + offsets::ForceAttack, 6);
                        }

                        memory::write<uintptr_t>(handle, client + offsets::ForceAttack, 4);
                        const auto& crosshairID = memory::read<int32_t>(handle, LocalPlayer + offsets::Crosshair);
                        const auto& entity = memory::read<uintptr_t>(handle, client + offsets::entity_list + (crosshairID - 1) * 0x10);



                        if (!crosshairID || crosshairID > 64)
                            goto urmom;

                        if (!localHealth)
                            goto urmom;

                        if (localTeam == memory::read<uintptr_t>(handle, entity + offsets::team_num))
                            goto urmom;





                        memory::write<uintptr_t>(handle, client + offsets::ForceAttack, 6);
                    };
                urmom:
                    auto feet_pos = memory::read<Vector>(handle, player + offsets::origin);
                }



                auto feet_pos = memory::read<Vector>(handle, player + offsets::origin);

                if (Headdot == 1) {
                    if (world_to_screen(head_pos + Vector{ 0, 0, 0.f }, top, view_matrix) && world_to_screen(head_pos - Vector{ 0, 5.f, 0.f }, bottom, view_matrix)) {
                        const float h = bottom.y - top.y;
                        const float w = h * 0.35f;
                        const float distance = _dist * 0.0254f;


                        ImGui::GetBackgroundDrawList()->AddCircle({ top.x - w, top.y }, static_cast<int>(1.5 / sqrt(distance)), ImColor(1.f, 1.f, 1.f));

                    }
                }





                if (BoxESP == 1) {

                    if (world_to_screen(head_pos + Vector{ 0, 0, 10.f }, top, view_matrix) && world_to_screen(feet_pos - Vector{ -5.f, 0, 5.f }, bottom, view_matrix)) {

                        const float h = abs(bottom.y - top.y);
                        int dX = (bottom.x - top.x);
                        const float w = h * 0.35f;
                        ImGui::GetBackgroundDrawList()->AddRect({ top.x - w, top.y }, { top.x + w, bottom.y }, ImColor(1.f, 1.f, 1.f));

                    }

                }



                if (health == 1) {
                    if (world_to_screen(head_pos + Vector{ 0, 0 , 0.f }, top, view_matrix) && world_to_screen(feet_pos - Vector{ 0, 0 , 0.f }, bottom, view_matrix)) {
                        const float h = bottom.y - top.y;
                        const float w = h * 0.35f;
                        const float distance = _dist * 0.03f;
                        const int test = static_cast<int>(1.5 / sqrt(distance));
                        const int Health = memory::read<int>(handle, player + offsets::Health);

                        ImGuiIO& io = ImGui::GetIO();
                        io.Fonts->AddFontFromFileTTF("C:\Windows\Fonts\calibri.ttf", 15);
                        ImFont* Small = io.Fonts->AddFontFromFileTTF("C:\Windows\Fonts\calibri.ttf", 15);

                        std::string s = std::to_string(Health);
                        char const* pchar = s.c_str();
                        const ImVec2 text_pos = ImVec2(top.x - w, top.y + h);



                        ImGui::GetBackgroundDrawList()->AddText(Small, static_cast<int>(5.5 / sqrt(distance)), text_pos, IM_COL32_WHITE, pchar);
                        ImVec2 BotHealth, TopHealth;
                        float health_percent = Health / 100.f;
                        float health_height = h * health_percent;
                        TopHealth.y = bottom.y - health_height;



                        ImGui::GetBackgroundDrawList()->AddLine({ top.x - w - 4, TopHealth.y }, { top.x - w - 4 , bottom.y }, ImColor(0.f, 1.f, 0.f));
                    }  
                }


                if (question == 0)
                {
                    if (world_to_screen(head_pos + Vector{ 0, 0, 11.f }, top, view_matrix) && world_to_screen(feet_pos - Vector{ 0, 0, 7.f }, bottom, view_matrix)) {
                       


                        // MODEL COLOR CHANGER
                            static constexpr Cham kEnemyColor{ 0, 0, 0 , 255 }; // black
                            memory::write(handle, player + offsets::m_clrRender, kEnemyColor);
                            const auto model_ambient_min = static_cast<uintptr_t>(engine + offsets::model_ambient_min - 0x2c);
                            float brightness = -100.f;


                        // VIEW MODEL COLOR CHANGER
                        const auto ViewModelHandler = memory::read<int>(handle, LocalPlayer + offsets::m_hViewModel);
                        const auto CurrentViewModelEntity = memory::read<DWORD>(handle, client + offsets::entity_list + (((ViewModelHandler & 0xFFF) - 1) * 16));
                        memory::write<DWORD>(handle, CurrentViewModelEntity + offsets::m_clrRender, ImColor(255, 255, 255, 255));


                        // GLOW
                        glow_t enemyGlow;
                        const auto glowIndex =          memory::read<int32_t>(handle, player + offsets::glowIndex);
                        const auto glowObjectManager =  memory::read<uintptr_t>(handle, client + offsets::dwGlowObjectManager);
                        enemyGlow.entity_address_ = player;
                        enemyGlow = memory::read<glow_t>(handle, glowObjectManager + (0x38 * glowIndex));
                        enemyGlow.render_occluded_ = true;
                        enemyGlow.render_unocculuded_ = false;
                        enemyGlow.glow_style_ = 1;
                        enemyGlow.red = 1.f;
                        enemyGlow.green_ = 0.7529411765f;
                        enemyGlow.blue_ = 0.7960784314f;
                        enemyGlow.alpha_ = 1;
                        memory::write<glow_t>(handle, glowObjectManager + (0x38 * glowIndex) , enemyGlow);
                        memory::write<BYTE>(handle, client + offsets::force_update_spectator_glow, 0xEB);
                    }
                }
                if (question == 0)
                {
                    if (world_to_screen(head_pos + Vector{ 0, 0, 11.f }, top, view_matrix) && world_to_screen(feet_pos - Vector{ 0, 0, 7.f }, bottom, view_matrix)) {
                        bool modeVal = 1;
                        float brightness;




                        for (int i = 0; i < 1024; i++) {
                            const auto entity = memory::read<std::uintptr_t>(handle, client + offsets::entity_list + i * 0x10);

                            if (entity != NULL) {

                                int clientNetworktable = memory::read<int>(handle, entity + 0x8);
                                int getClientclass = memory::read<int>(handle, clientNetworktable + 2 * 0x4);
                                int classPointer = memory::read<int>(handle, getClientclass + 0x1);
                                int classID = memory::read<int>(handle, classPointer + 0x14);
                                brightness = 2.f;

                                const auto thisptr = static_cast<std::uintptr_t>(engine + offsets::model_ambient_min - 0x2c);   //adding brightness to random shit...good idea!
                                memory::write<int32_t>(handle, engine + offsets::model_ambient_min, *reinterpret_cast<std::uintptr_t*>(&brightness) ^ thisptr);

                                if (classID == 69) //nice
                                {
                                    memory::write<BYTE>(handle, entity + offsets::m_bUseCustomAutoExposureMin, 1);
                                    memory::write<BYTE>(handle, entity + offsets::m_bUseCustomAutoExposureMax, 1);
                                    memory::write<float>(handle, entity + offsets::m_flCustomAutoExposureMax, 0.065f);
                                    memory::write<float>(handle, entity + offsets::m_flCustomAutoExposureMin, 0.065f);
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

        for (int i = 1; i < 32; ++i)
        {
            float brightness;
            const auto player = memory::read<DWORD>(handle, client + offsets::entity_list + i * 0x10);
            static constexpr Cham kEnemyColor{ 255, 255, 255 , 255 };
            memory::write(handle, player + offsets::m_clrRender, kEnemyColor);
        }
            memory::write(handle, client + offsets::force_update_spectator_glow, uint8_t(0x74));
          
        



        DestroyWindow(window);
        UnregisterClassW(wc.lpszClassName, wc.hInstance);

        CloseHandle(handle);

        return TRUE;


    }

    