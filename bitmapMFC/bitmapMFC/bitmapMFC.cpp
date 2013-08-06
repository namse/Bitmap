// bitmapMFC.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "bitmapMFC.h"
#include<sstream>
#include "bitmap.h"
#include<list>
#include<bitset>
#include<math.h>
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
	std::string str;
	std::ostringstream oss;
	for(int i=0; i<100000; i++)
	{
		oss << rand() << " ";
	}
	str = oss.str();

	std::list<bool>bitList;
	for(std::size_t i = 0; i < str.size(); i++)
	{
		std::bitset<8>bs ((int)str.c_str()[i]);
		for(int l=0; l<8; l++)
		{/*
		 bool *pBit = (bool*)malloc(sizeof(bool));
		 if(bs[l] == 0)
		 *pBit = false;
		 else
		 *pBit = true;
		 */
			bitList.push_back(bs[l]);
		}
	}

	int x, y;
	long size;
	BYTE* Buffer = LoadBMP ( &x, &y, &size, L"bit.bmp" );
	RGBTriplet* RGBbuffer = (RGBTriplet*)ConvertBMPToRGBBuffer( Buffer, x, y );
	/*for(int i=0; i<y; i++)
	{
	for(int l=0; l<x; l++)
	{
	//printf("%d/%d/%d || ",(buffer+l+i*x)->red ,(buffer+l+i*x)->green ,(buffer+l+i*x)->blue);
	(RGBbuffer+l+i*x)->red = rand()%255;
	(RGBbuffer+l+i*x)->green = rand()%255;
	(RGBbuffer+l+i*x)->blue = rand()%255;
	}
	}*/
	//SaveBMP(ConvertRGBToBMPBuffer((BYTE*)RGBbuffer,x,y,&size),x,y,size,L"savebmp.bmp");
	RECT rect;
	SetRect(&rect,0,0,x,y);
	BYTE* TitleBuffer = CreateTextImage(BOLD, CENTER, 120, L"arial", &rect, L"HelloWorld!");


	RGBTriplet* TitleRGBbuffer = (RGBTriplet*)ConvertBMPToRGBBuffer( TitleBuffer, x, y );
	FILE *fout;
	fopen_s(&fout,"text.txt","w");
	/*
	int preR, preG, preB;
	preR = preG = preB = -1;
	
	
	for(int i=0; i<y; i++)
	{
		for(int l=0; l<x; l++)
		{
			int R, G, B;
			// Data Section
			if((TitleRGBbuffer+l+i*x)->red == 255 && (TitleRGBbuffer+l+i*x)->green == 255 && (TitleRGBbuffer+l+i*x)->blue == 255)
			{
				R = GetBitsByPoppingWithNumber(&bitList,6);
				G = GetBitsByPoppingWithNumber(&bitList,6);
				B = GetBitsByPoppingWithNumber(&bitList,6);
				
				R+= pow(2,6) + pow(2,7);
				G+= pow(2,6) + pow(2,7);
				B+= pow(2,6) + pow(2,7);
			}

			//Title Section
			else
			{

				R = GetBitsByPoppingWithNumber(&bitList,6);
				G = GetBitsByPoppingWithNumber(&bitList,6);
				B = GetBitsByPoppingWithNumber(&bitList,6);

			}
			(TitleRGBbuffer+l+i*x)->red		= R;
			(TitleRGBbuffer+l+i*x)->green	= G;
			(TitleRGBbuffer+l+i*x)->blue	= B;

		}
		printf("line : %d\n",i);
	}
	for(int i=0; i<y; i++)
	{
		for(int l=0; l<x; l++)
		{
			if(preR != (TitleRGBbuffer+l+i*x)->red || (TitleRGBbuffer+l+i*x)->green != preG && (TitleRGBbuffer+l+i*x)->blue != preB)
			{
				preR = (TitleRGBbuffer+l+i*x)->red;
				preG = (TitleRGBbuffer+l+i*x)->green;
				preB = (TitleRGBbuffer+l+i*x)->blue;

				fprintf(fout,"int %d : %d -> %d/%d/%d\n",l,i,(TitleRGBbuffer+l+i*x)->red ,(TitleRGBbuffer+l+i*x)->green ,(TitleRGBbuffer+l+i*x)->blue);
			}
		}
	}*/
	SaveBMP(ConvertRGBToBMPBuffer((BYTE*)TitleRGBbuffer,x,y,&size),x,y,size,L"savebmp.bmp");
	fprintf(fout,"\n\nsize : %d\n\n",str.size());
	printf_s("over");
	//getchar();
	return nRetCode;
}
