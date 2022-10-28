#include "CApp.h"

CApp::CApp():
    m_strDllPath(""),
    m_strProcName("")
{}

CApp::~CApp() {

}

DWORD CApp::GetProcIdent() {
    DWORD dProcId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);

        //This loop basically iterates on every process running until it finds one whose executable name loosely matches our target process
        if (Process32First(hSnap, &procEntry)) {
            do {

                //Tedious conversion step to convert our Unicode16 string into the regular ANSI strings we know...
                _bstr_t bstrCvrtProcName(procEntry.szExeFile);
                const char* strCvrtProcName = bstrCvrtProcName;

                if (!_stricmp(strCvrtProcName, m_strProcName.c_str())) {
                    dProcId = procEntry.th32ProcessID;
                    break;
                }
                
            } while (Process32Next(hSnap, &procEntry));
        }
    }

    CloseHandle(hSnap);
    return dProcId;
}

DWORD WINAPI CApp::InjectToProc(LPVOID lpParam) {
    DWORD dProcID = 0;

    CApp* app = (CApp*)lpParam;

    char buf[128];
    snprintf(buf, 128, "DLL Path: %s\nProcess Name: %s", app->m_strDllPath.c_str(), app->m_strProcName.c_str());

    MessageBoxA(NULL, buf, "DLL Injection", MB_OK);
    while (!dProcID) {
        dProcID = app->GetProcIdent();
        Sleep(30);
    }

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, dProcID);

    if (hProc && hProc != INVALID_HANDLE_VALUE) {
        void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        WriteProcessMemory(hProc, loc, app->m_strDllPath.c_str(), strlen(app->m_strDllPath.c_str()) + 1, 0);

        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);

        if (hThread)
            CloseHandle(hThread);
    }

    if (hProc)
        CloseHandle(hProc);
    
    return 0;
}


void CApp::SetDllPath(std::string strDllPath) { m_strDllPath = strDllPath; }
void CApp::SetProcName(std::string strProcName) { m_strProcName = strProcName; }