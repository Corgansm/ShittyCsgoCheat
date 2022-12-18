

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