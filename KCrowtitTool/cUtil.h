#pragma once
class cUtil
{
public:
	/// Wide string�� �Ϲ� string���� ��ȯ�Ѵ�. �� �޼ҵ�� ���� ���ĺ� ��ȯ����(ANSI) ����ؾ� �Ѵ�.
	static const std::string& WStringToString(const std::wstring& in, std::string& out);

	/// �Ϲ� string�� Wide string���� ��ȯ�Ѵ�. �� �޼ҵ�� ���� ���ĺ� ��ȯ����(ANSI) ����ؾ� �Ѵ�.
	static const std::wstring& StringToWString(const std::string& in, std::wstring& out);

/////////////////////////////////////////////////////
	
	static std::wstring PickFileNameFromFullPath(std::wstring fullpath)
	{
		std::wstring separate = L"\\";

		std::wstring::iterator iter;
		iter = std::find_end(fullpath.begin(), fullpath.end(), separate.begin(), separate.end());

		std::wstring fileName(iter + 1, fullpath.end());

		return fileName;
	}
	//Ȯ���� ����
	static std::wstring PickNameFromFileName(std::wstring filename)
	{
		int find = filename.find('.');
		std::wstring name(filename.begin(), filename.begin() + find);

		return name;
	}
	//Ȯ���� ����
	static std::wstring PickFormatFromFileName(std::wstring filename)
	{
		int find = filename.find('.');
		std::wstring format(filename.begin() + find + 1, filename.end());

		return format;
	}
	//Ǯ��ο��� ���ϰ�� ����(���ϸ� ������ ������)
	static std::wstring PickPathFromFullPath(std::wstring fullpath)
	{
		std::wstring separate = L"\\";
		std::wstring::iterator iter;
		iter = std::find_end(fullpath.begin(), fullpath.end(), separate.begin(), separate.end());

		std::wstring pathName(fullpath.begin(), iter);

		pathName += L"\\";

		return pathName;
	}

	static BOOL OpenFile(IN TCHAR* filterName, OUT std::wstring* pDirectory)
	{
		/////////�����///////////////
		//std::string temp;
		//if (cUtil::OpenFile("OBJ File(*.obj)\0*.obj\0", &temp))
		//{
		//	if (g_pOBJManager->AddOBJ(temp.c_str()))
		//	{
		//		std::string OBJKeyName = cUtil::PickFileNameFromFullPath(temp);
		//		SendMessage(GetDlgItem(hDlg, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)OBJKeyName.c_str());
		//	}
		//}
		//��ȣ���� ���ľ� ���ε尡 �ϼ��˴ϴ�!
		TCHAR cDir[MAX_PATH + 1];
		GetCurrentDirectory(MAX_PATH + 1, cDir);

		OPENFILENAME opf;
		TCHAR str[300];
		TCHAR lpstrFile[MAX_PATH] = TEXT("");
		memset(&opf, 0, sizeof(OPENFILENAME));
		opf.lStructSize = sizeof(OPENFILENAME);
		opf.hwndOwner = g_hWnd;
		opf.lpstrFilter = filterName;
		opf.lpstrFile = lpstrFile;
		opf.nMaxFile = 256;
		opf.lpstrInitialDir = cDir;

		if (GetOpenFileName(&opf) != 0)
		{
			SetCurrentDirectory(cDir);
			*pDirectory = lpstrFile;
			return TRUE;
		}

		SetCurrentDirectory(cDir);
		return FALSE;
	}

	static BOOL SaveFile(IN TCHAR* filterName, OUT std::wstring* pDirectory)
	{
		///////��� ���//////////////
		//std::string temp;
		//if (cUtil::SaveFile("SCENE File(*.scene)\0*.scene\0", &temp))
		//{
		//	temp += ".scene";
		//	MessageBox(g_hWnd, temp.c_str(), "test", MB_OK);
		//}

		TCHAR cDir[MAX_PATH + 1];
		GetCurrentDirectory(MAX_PATH + 1, cDir);

		OPENFILENAME opf;
		TCHAR str[300];
		TCHAR lpstrFile[MAX_PATH] = TEXT("");
		memset(&opf, 0, sizeof(OPENFILENAME));
		opf.lStructSize = sizeof(OPENFILENAME);
		opf.hwndOwner = g_hWnd;
		opf.lpstrFilter = filterName;
		opf.lpstrFile = lpstrFile;
		opf.nMaxFile = 256;
		opf.lpstrInitialDir = cDir;

		if (GetSaveFileName(&opf) != 0)
		{
			SetCurrentDirectory(cDir);
			*pDirectory = lpstrFile;
			return TRUE;
		}

		SetCurrentDirectory(cDir);
		return FALSE;
	}


	cUtil();
	~cUtil();
};

