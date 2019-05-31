#include "stdafx.h"
#include "cUtil.h"


cUtil::cUtil()
{
}


cUtil::~cUtil()
{
}

/** @param  in  wide string
*  @param  out ANSI string
*  @return ���ϰ����� ��ȯ�� �Ϲ� ANSI string. ��� ���� ���Ǹ� ���� ���ϰ�
*  @warning  �� �޼ҵ�� ��Ƽ ����Ʈ ��ȯ�� �������� �����Ƿ� ���ĺ� ��ȯ����
*            ����ؾ� �Ѵ�. �׷��� ���� ��� �ջ�� �� �� ���� ���ڿ��� ���
*            �� ���̴�.
*/
const std::string& cUtil::WStringToString(const std::wstring& in, std::string& out)
{
	out.clear();

	size_t inSize = in.size();
	for (size_t i = 0; i < inSize; i++)
	{
		out.insert(out.end(), 1, static_cast<const char>(in.at(i) & 0xFF));
	}
	return out;
}

//------------------------------------------------------------------

/** @param  in  ANSI string
*  @param  out wide string
*  @return ���ϰ����� ��ȯ�� wide string. ��� ���� ���Ǹ� ���� ���ϰ�
*  @warning  �� �޼ҵ�� ��Ƽ ����Ʈ ��ȯ�� �������� �����Ƿ� ���ĺ� ��ȯ����
*            ����ؾ� �Ѵ�. �׷��� ���� ��� �ջ�� �� �� ���� ���ڿ��� ���
*            �� ���̴�.
*/
const std::wstring& cUtil::StringToWString(const std::string& in, std::wstring& out)
{
	out.clear();

	size_t inSize = in.size();
	for (size_t i = 0; i < inSize; i++)
	{
		out.insert(out.end(), 1, static_cast<const wchar_t>(in.at(i)));
	}
	return out;
}