#include <ZXBase/Export_ZXBase.h>
#include <Windows.h>

namespace zx{
	ZXWString TranslateString(ZXString str)
	{
		int dwNum = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		wchar_t *pwText = new wchar_t[dwNum];
		memset(pwText,0, dwNum*sizeof(wchar_t));
		MultiByteToWideChar (CP_ACP, 0, str.c_str(), -1, pwText, dwNum);
		ZXWString res = pwText;
		if(!pwText) delete []pwText;
		return res;
	}
};
