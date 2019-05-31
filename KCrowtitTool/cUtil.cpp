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
*  @return 동일값으로 변환된 일반 ANSI string. 사용 상의 편의를 위한 리턴값
*  @warning  이 메소드는 멀티 바이트 변환을 지원하지 않으므로 알파벳 변환에만
*            사용해야 한다. 그렇지 않은 경우 손상된 알 수 없는 문자열을 얻게
*            될 것이다.
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
*  @return 동일값으로 변환된 wide string. 사용 상의 편의를 위한 리턴값
*  @warning  이 메소드는 멀티 바이트 변환을 지원하지 않으므로 알파벳 변환에만
*            사용해야 한다. 그렇지 않은 경우 손상된 알 수 없는 문자열을 얻게
*            될 것이다.
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