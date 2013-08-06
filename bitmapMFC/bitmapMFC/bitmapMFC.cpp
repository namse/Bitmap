// bitmapMFC.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "bitmapMFC.h"

#include "bitmap.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 유일한 응용 프로그램 개체입니다.

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 오류 코드를 필요에 따라 수정합니다.
			_tprintf(_T("심각한 오류: MFC를 초기화하지 못했습니다.\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 응용 프로그램의 동작은 여기에서 코딩합니다.
		}
	}
	else
	{
		// TODO: 오류 코드를 필요에 따라 수정합니다.
		_tprintf(_T("심각한 오류: GetModuleHandle 실패\n"));
		nRetCode = 1;
	}




	int x, y;
	long size;
	BYTE* Buffer = LoadBMP ( &x, &y, &size, L"bit.bmp" );
	RGBTriplet* RGBbuffer = (RGBTriplet*)ConvertBMPToRGBBuffer( Buffer, x, y );
	for(int i=0; i<y; i++)
	{
		for(int l=0; l<x; l++)
		{
			//printf("%d/%d/%d || ",(buffer+l+i*x)->red ,(buffer+l+i*x)->green ,(buffer+l+i*x)->blue);
			(RGBbuffer+l+i*x)->red = rand()%255;
			(RGBbuffer+l+i*x)->green = rand()%255;
			(RGBbuffer+l+i*x)->blue = rand()%255;
		}
	}
	SaveBMP(ConvertRGBToBMPBuffer((BYTE*)RGBbuffer,x,y,&size),x,y,size,L"savebmp.bmp");
	RECT rect;
	SetRect(&rect,0,0,x,y);
	BYTE* TitleBuffer = CreateTextImage(BOLD, CENTER, 20, L"arial", &rect, L"HelloWorld!");
	

	RGBTriplet* TitleRGBbuffer = (RGBTriplet*)ConvertBMPToRGBBuffer( TitleBuffer, x, y );
	
	for(int i=0; i<y; i++)
	{
		for(int l=0; l<x; l++)
		{
			//printf("%d/%d/%d || ",(TitleRGBbuffer+l+i*x)->red ,(TitleRGBbuffer+l+i*x)->green ,(TitleRGBbuffer+l+i*x)->blue);
			
			if((TitleRGBbuffer+l+i*x)->red == 255 && (TitleRGBbuffer+l+i*x)->green == 0 && (TitleRGBbuffer+l+i*x)->blue == 0)
			{
				(TitleRGBbuffer+l+i*x)->red = 255;
				(TitleRGBbuffer+l+i*x)->green = 0;//rand()%255;
				(TitleRGBbuffer+l+i*x)->blue = 0;//rand()%255;
			}
			else
			{
				(TitleRGBbuffer+l+i*x)->red = (RGBbuffer+l+i*x)->red;
				(TitleRGBbuffer+l+i*x)->green = (RGBbuffer+l+i*x)->green;
				(TitleRGBbuffer+l+i*x)->blue = (RGBbuffer+l+i*x)->blue;
			}
		}
	}
	
	SaveBMP(ConvertRGBToBMPBuffer((BYTE*)TitleRGBbuffer,x,y,&size),x,y,size,L"savebmp.bmp");

	getchar();
	return nRetCode;
}
