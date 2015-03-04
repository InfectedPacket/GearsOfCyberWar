
//#include <WinGDI.h>
//#undef ERROR
int TestScreenshot();
int SaveScreenshotToFile(const wchar_t* Filename);
void CreateBMPFile(HWND hwnd, HANDLE hf, PBITMAPINFO pbi, 
                  HBITMAP hBMP, HDC hDC);

//typedef void (*BitmapAction)(HBITMAP BitmapImage);