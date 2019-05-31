#pragma once
class cInput
{
public:
	enum KEY_STATE{ KEY_DOWN_ONCE = 1, KEY_UP_ONCE = 2 };
	enum eVirtualKey{
		NUM_0 = 0x30,
		NUM_1 = 0x31,
		NUM_2 = 0x32,
		NUM_3 = 0x33,
		NUM_4 = 0x34,
		NUM_5 = 0x35,
		NUM_6 = 0x36,
		NUM_7 = 0x37,
		NUM_8 = 0x38,
		NUM_9 = 0x39,
		/////////////A~Z (0x41 - 0x5A)
		KEY_A = 0x41,
		KEY_B = 0x42,
		KEY_C = 0x43,
		KEY_D = 0x44,
		KEY_E = 0x45,
		KEY_F = 0x46,
		KEY_G = 0x47,
		KEY_H = 0x48,
		KEY_I = 0x49,
		KEY_J = 0x4A,
		KEY_K = 0x4B,
		KEY_L = 0x4C,
		KEY_M = 0x4D,
		KEY_N = 0x4E,
		KEY_O = 0x4F,
		KEY_P = 0x50,
		KEY_Q = 0x51,
		KEY_R = 0x52,
		KEY_S = 0x53,
		KEY_T = 0x54,
		KEY_U = 0x55,
		KEY_V = 0x56,
		KEY_W = 0x57,
		KEY_X = 0x58,
		KEY_Y = 0x59,
		KEY_Z = 0x5A,

	};
	cInput();
	~cInput();
	static int kDown[256];// = { 0, };//Flag
	static int kUp[256];// = { 0, };//Flag
	//static int k[256];// = { 0, };//Flag
	//static bool GetKeyState(int vKey, int &nState)
	//{
	//	//check if the key was pressed (key_down)
	//	if ((GetAsyncKeyState(vKey) & 0x8000) && (k[vKey] == 0)){
	//		k[vKey] = 1;
	//		//call OnKeyDown('0') once
	//		nState = KEY_DOWN_ONCE;
	//		return true;
	//	}
	//	//check if the key was released (key up)
	//	else if (GetAsyncKeyState(vKey) == 0){

	//		if (k[vKey] == 1){
	//			k[vKey] = 0;//reset the flag				
	//			//call OnKeyUp('0') once!!
	//			nState = KEY_UP_ONCE;
	//			return true;
	//		}

	//	}
	//	
	//	return false;
	//}

	static bool GetKeyDown(int vKey)
	{
		if (!::GetFocus())
		{
			return false;
		}
		//check if the key was pressed (key_down)
		if ((GetAsyncKeyState(vKey) & 0x8000) && (kDown[vKey] == 0)){
			kDown[vKey] = 1;
			//call OnKeyDown('0') once
			//nState = KEY_DOWN_ONCE;
			return true;
		}
		//check if the key was released (key up)
		else if (GetAsyncKeyState(vKey) == 0){

			if (kDown[vKey] == 1){
				kDown[vKey] = 0;//reset the flag				
				//call OnKeyUp('0') once!!
				//nState = KEY_UP_ONCE;
				return false;
			}

		}

		return false;
	}

	static bool GetKeyUp(int vKey)
	{
		if (!::GetFocus())
		{
			return false;
		}
		//check if the key was pressed (key_down)
		if ((GetAsyncKeyState(vKey) & 0x8000) && (kUp[vKey] == 0)){
			kUp[vKey] = 1;
			//call OnKeyDown('0') once
			//nState = KEY_DOWN_ONCE;
			return false;
		}
		//check if the key was released (key up)
		else if (GetAsyncKeyState(vKey) == 0){

			if (kUp[vKey] == 1){
				kUp[vKey] = 0;//reset the flag				
				//call OnKeyUp('0') once!!
				//nState = KEY_UP_ONCE;
				return true;
			}

		}

		return false;
	}

	static bool GetKey(int vKey)
	{
		if (!::GetFocus())
		{
			return false;
		}

		//return true every frame while get key down 
		if ((GetAsyncKeyState(vKey) & 0x8000))
		{
			return true;
		}

		return false;
	}

};

