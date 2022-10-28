#include <Windows.h>
#include <TlHelp32.h>
#include <comdef.h>
#include <iostream>

class CApp {
private:
	std::string m_strDllPath;
	std::string m_strProcName;

    DWORD GetProcIdent();

public:
	CApp();
	~CApp();
    static DWORD WINAPI InjectToProc(LPVOID lpParam);
	void SetDllPath(std::string strDllPath);
	void SetProcName(std::string strProcName);
};
