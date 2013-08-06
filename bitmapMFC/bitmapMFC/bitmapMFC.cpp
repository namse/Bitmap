// bitmapMFC.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "bitmapMFC.h"

#include "bitmap.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������ ���� ���α׷� ��ü�Դϴ�.

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// MFC�� �ʱ�ȭ�մϴ�. �ʱ�ȭ���� ���� ��� ������ �μ��մϴ�.
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: ���� �ڵ带 �ʿ信 ���� �����մϴ�.
			_tprintf(_T("�ɰ��� ����: MFC�� �ʱ�ȭ���� ���߽��ϴ�.\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: ���� ���α׷��� ������ ���⿡�� �ڵ��մϴ�.
		}
	}
	else
	{
		// TODO: ���� �ڵ带 �ʿ信 ���� �����մϴ�.
		_tprintf(_T("�ɰ��� ����: GetModuleHandle ����\n"));
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
