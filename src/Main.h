// LOOK !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	int Auto = 1;
	int crosshair = 1;
	int BoxESP = 1;
	int Headdot = 1;
	int FOV = 1;
	int FOV2 = 110;
	int FOV3 = 90;
	int bhop = 1;
	int bhop_chance = 20;
	int recoil = 1;
	int recoil_smooth_x = 1;
	int recoil_smooth_y = 1;
	int skeleton = 1;
	int test = 1;
	int health = 1;
	int triggerbot = 1;


// LOOK !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	class Options
	{
	private:
		bool b_aim = false;
		int i_aim_smooth = 1;
		int i_aim_smooth_after_shot = 1;
		int i_aim_fov = 1;
		bool i_recoil = false;
		int i_recoil_smooth_x = 1;
		int i_recoil_smooth_y = 1;
		bool b_bhop = false;
		int i_bhop_chance = 100;
		bool b_trigger = false;
		int i_trigger_delay = 25;
		bool b_glow = false;
		

		bool b_radar = false;

		int i_ak_skin = 1;
		int i_m4_skin = 1;
		int i_awp_skin = 1;
		int i_m4_silencer_skin = 1;
	public:
		bool* aim = &b_aim;
		int* aim_smooth = &i_aim_smooth;
		int* aim_smooth_after_shot = &i_aim_smooth_after_shot;
		int* aim_fov = &i_aim_fov;
		bool* recoil = &i_recoil;
		int* recoil_smooth_x = &i_recoil_smooth_x;
		int* recoil_smooth_y = &i_recoil_smooth_y;
		bool* bhop = &b_bhop;
		int* bhop_chance = &i_bhop_chance;
		bool* trigger = &b_trigger;
		int* trigger_delay = &i_trigger_delay;
		bool* glow = &b_glow;

		bool* radar = &b_radar;

		int* ak_skin = &i_ak_skin;
		int* m4_skin = &i_m4_skin;
		int* awp_skin = &i_awp_skin;
		int* m4_silencer_skin = &i_m4_silencer_skin;
	};

	inline Options g_Options;
