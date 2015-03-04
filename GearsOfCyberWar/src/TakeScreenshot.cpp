#include <stdio.h>
#include <Windows.h>
#include "TakeScreenshot.h"
#include <iostream>
#include <ole2.h>
#include <olectl.h>
//LPCWSTR filename
bool SaveScreenCaptureToBitmap(HANDLE hOutput, HBITMAP bmp, HPALETTE pal)
{
    bool result = false;
    PICTDESC pictureDescription;

    pictureDescription.cbSizeofstruct   = sizeof(PICTDESC);
    pictureDescription.picType      = PICTYPE_BITMAP;
    pictureDescription.bmp.hbitmap  = bmp;
    pictureDescription.bmp.hpal     = pal;

    LPPICTURE picture;
    HRESULT hResult = OleCreatePictureIndirect(&pictureDescription, IID_IPicture, false,
                       reinterpret_cast<void**>(&picture));

    if (!SUCCEEDED(hResult)) {
		return false;
	}

    LPSTREAM stream;
    hResult = CreateStreamOnHGlobal(0, true, &stream);

    if (!SUCCEEDED(hResult))
    {
		picture->Release();
		return false;
    }

    LONG bytes_streamed;
    hResult = picture->SaveAsFile(stream, true, &bytes_streamed);

  //  HANDLE hOutput = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, 0,
  //               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

    if (!SUCCEEDED(hResult) || hOutput == INVALID_HANDLE_VALUE)
    {
		stream->Release();
		picture->Release();
		return false;
    }

    HGLOBAL mem = 0;
    GetHGlobalFromStream(stream, &mem);
    LPVOID data = GlobalLock(mem);

    DWORD bytes_written;

    result   = !!WriteFile(hOutput, data, bytes_streamed, &bytes_written, 0);
    result  &= (bytes_written == static_cast<DWORD>(bytes_streamed));

    GlobalUnlock(mem);
   // CloseHandle(hOutput);

    stream->Release();
    picture->Release();

    return result;
}


bool TakeScreenCapture(HANDLE fname){
    HDC hdcSource = GetDC(NULL);					//Get context to desktop window
    HDC hdcMemory = CreateCompatibleDC(hdcSource);  //Get context to memory

	int ScreenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    int capX = GetDeviceCaps(hdcSource, HORZRES);
    int capY = GetDeviceCaps(hdcSource, VERTRES);

    HBITMAP hBitmap = CreateCompatibleBitmap(hdcSource, 
		ScreenWidth, ScreenHeight);
    HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMemory, hBitmap);

    BitBlt(hdcMemory, 0, 0, ScreenWidth, ScreenHeight, hdcSource, 0, 0, SRCCOPY);
    hBitmap = (HBITMAP)SelectObject(hdcMemory, hBitmapOld);

    DeleteDC(hdcSource);
    DeleteDC(hdcMemory);

    //HPALETTE hpal = NULL;
    return SaveScreenCaptureToBitmap(fname, hBitmap, NULL);
}



int TestScreenshot() {
	int result = 0;
	const wchar_t* ScreenshotFile = L"C:\\tmp\\screenshot.bmp";
	HANDLE hOutput = CreateFile(ScreenshotFile, GENERIC_WRITE, FILE_SHARE_READ, 0,
                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	printf("[*] Taking capture of screen...\n");
	//-result = SaveScreenshotToFile(ScreenshotFile);
	result = TakeScreenCapture(hOutput);
	CloseHandle(hOutput);
	return result;
}