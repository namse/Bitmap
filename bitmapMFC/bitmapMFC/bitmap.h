#include<memory.h>
#include<Windows.h>
#include<memory.h>
#include<list>

BYTE* LoadBMP ( int* width, int* height, long* size, LPCTSTR bmpfile )
{
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	DWORD bytesread;
	HANDLE file = CreateFile ( bmpfile , GENERIC_READ, FILE_SHARE_READ,
		 NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
	if ( NULL == file )
		return NULL;
	if ( ReadFile ( file, &bmpheader, sizeof ( BITMAPFILEHEADER ), 
		&bytesread, NULL ) == false )
	{
		CloseHandle ( file );
		return NULL;
	}
	if ( ReadFile ( file, &bmpinfo, sizeof ( BITMAPINFOHEADER ), 
		&bytesread, NULL ) == false )
	{
		CloseHandle ( file );
		return NULL;
	}

	if ( bmpheader.bfType != 'MB' )
	{
		CloseHandle ( file );
		return NULL;
	}

	if ( bmpinfo.biCompression != BI_RGB )
	{
		CloseHandle ( file );
		return NULL;
	}

	if ( bmpinfo.biBitCount != 24 )
	{
		CloseHandle ( file );
		return NULL;
	}

	*width   = bmpinfo.biWidth;
	*height  = abs ( bmpinfo.biHeight );
	*size = bmpheader.bfSize - bmpheader.bfOffBits;


	BYTE* Buffer = new BYTE[ *size ];

	SetFilePointer ( file, bmpheader.bfOffBits, NULL, FILE_BEGIN );

	if ( ReadFile ( file, Buffer, *size, &bytesread, NULL ) == false )
	{
		delete [] Buffer;
		CloseHandle ( file );
		return NULL;
	}

	CloseHandle ( file );
	return Buffer;
}



typedef struct tagRGBTriplet
{
	BYTE red;
	BYTE green;
	BYTE blue;
} RGBTriplet;



BYTE* ConvertBMPToRGBBuffer ( BYTE* Buffer, int width, int height )
{
	if ( ( NULL == Buffer ) || ( width == 0 ) || ( height == 0 ) )
		return NULL;

	int padding = 0;
	int scanlinebytes = width * 3;
	while ( ( scanlinebytes + padding ) % 4 != 0 )
		padding++;



	int psw = scanlinebytes + padding;

	BYTE* newbuf = new BYTE[width*height*3];



	long bufpos = 0;   
	long newpos = 0;
	for ( int y = 0; y < height; y++ )
		for ( int x = 0; x < 3 * width; x+=3 )
		{
			newpos = y * 3 * width + x;     
			bufpos = ( height - y - 1 ) * psw + x;

			newbuf[newpos] = Buffer[bufpos + 2];       
			newbuf[newpos + 1] = Buffer[bufpos+1]; 
			newbuf[newpos + 2] = Buffer[bufpos];     
		}



	return newbuf;
}

bool SaveBMP ( BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile )
{

	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER info;
	memset ( &bmfh, 0, sizeof (BITMAPFILEHEADER ) );
	memset ( &info, 0, sizeof (BITMAPINFOHEADER ) );

	bmfh.bfType = 0x4d42;       // 0x4d42 = 'BM'
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + 
		sizeof(BITMAPINFOHEADER) + paddedsize;
	bmfh.bfOffBits = 0x36;

	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = width;
	info.biHeight = height;
	info.biPlanes = 1;	
	info.biBitCount = 24;
	info.biCompression = BI_RGB;	
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 0x0ec4;  
	info.biYPelsPerMeter = 0x0ec4;     
	info.biClrUsed = 0;	
	info.biClrImportant = 0; 

	HANDLE file = CreateFile ( bmpfile , GENERIC_WRITE, FILE_SHARE_READ,
		 NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( NULL == file )
	{
		CloseHandle ( file );
		return false;
	}

	unsigned long bwritten;
	if ( WriteFile ( file, &bmfh, sizeof ( BITMAPFILEHEADER ), 
		&bwritten, NULL ) == false )
	{	
		CloseHandle ( file );
		return false;
	}

	if ( WriteFile ( file, &info, sizeof ( BITMAPINFOHEADER ), 
		&bwritten, NULL ) == false )
	{	
		CloseHandle ( file );
		return false;
	}

	if ( WriteFile ( file, Buffer, paddedsize, &bwritten, NULL ) == false )
	{	
		CloseHandle ( file );
		return false;
	}

	CloseHandle ( file );
	return true;
}

BYTE* ConvertRGBToBMPBuffer ( BYTE* Buffer, int width, int height, 
	long* newsize )
{
	if ( ( NULL == Buffer ) || ( width == 0 ) || ( height == 0 ) )
		return NULL;

	int padding = 0;
	int scanlinebytes = width * 3;
	while ( ( scanlinebytes + padding ) % 4 != 0 ) 
		padding++;
	int psw = scanlinebytes + padding;

	*newsize = height * psw;
	BYTE* newbuf = new BYTE[*newsize];

	memset ( newbuf, 0, *newsize );

	long bufpos = 0;   
	long newpos = 0;
	for ( int y = 0; y < height; y++ )
		for ( int x = 0; x < 3 * width; x+=3 )
		{
			bufpos = y * 3 * width + x;     // position in original buffer
			newpos = ( height - y - 1 ) * psw + x; // position in padded buffer
			newbuf[newpos] = Buffer[bufpos+2];       // swap r and b
			newbuf[newpos + 1] = Buffer[bufpos + 1]; // g stays
			newbuf[newpos + 2] = Buffer[bufpos];     // swap b and r
		}
	return newbuf;
}

enum FONTWEIGHT	
{
	BOLD = 1,
	NORMAL
};
enum FONTALIGN
{
	RIGHT = 1,
	CENTER = 2,
	LEFT = 3
};

BYTE* CreateTextImage(FONTWEIGHT bold, FONTALIGN align, int fontSize, const wchar_t* fontName, RECT *rect, wchar_t *text)
{
	int xsize, ysize;
	xsize = rect->right - rect->left;
	ysize = rect->bottom - rect->top;

	HDC hDC = CreateCompatibleDC(NULL);

	int lineSize = ((xsize * 3 + 3)&~3);
	int dwSizeImage = lineSize * ysize;
	BITMAPINFO bmi;   
	
	ZeroMemory(&bmi, sizeof(BITMAPINFO));    
	
	bmi.bmiHeader.biSize           = sizeof(BITMAPINFOHEADER);         
	bmi.bmiHeader.biWidth          = xsize;         
	bmi.bmiHeader.biHeight         = ysize;         
	bmi.bmiHeader.biPlanes         = 1;         
	bmi.bmiHeader.biBitCount       = 24;         
	bmi.bmiHeader.biCompression    = BI_RGB;         
	bmi.bmiHeader.biSizeImage      = dwSizeImage;         
	bmi.bmiHeader.biXPelsPerMeter  = 0;         
	bmi.bmiHeader.biYPelsPerMeter  = 0;        
	bmi.bmiHeader.biClrUsed        = 0;         
	bmi.bmiHeader.biClrImportant   = 0;    
	
	BYTE *buf;  
	HBITMAP hBitmap = CreateDIBSection(NULL,&bmi,DIB_RGB_COLORS,(void**)&buf,NULL,0);// 배경색   
	
	SelectObject(hDC, hBitmap);

	HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
	FillRect(hDC, rect, hBrush);


	// 폰트를 생성해서 글자를 출력한다.  
	int fnWeight,fnAlign;    // 폰트두께  
	
	switch (bold)
	{
	case BOLD:
		fnWeight = (int) FW_BOLD;
		break;
	case NORMAL:
		fnWeight = (int) FW_NORMAL;
		break;
	default:
		break;
	}
	
	switch (align)
	{
	case RIGHT:
		fnAlign =  DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX;
		break;
	case CENTER:
		fnAlign =  DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX;
		break;
	case LEFT:
		break;
		fnAlign =  DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX;
	default:
		break;
	}
	
	size_t slen = wcslen(text);  
	
	HFONT   hFont =  CreateFont(-fontSize, 0, 0, 0, fnWeight, FALSE, FALSE, 0,        
		ANSI_CHARSET, OUT_STRING_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,        
		DEFAULT_PITCH | FF_SWISS,fontName);  
	
	SelectObject(hDC, hFont);  
	SetTextColor(hDC, RGB(255, 0, 0));   
	
	int ret = DrawText(hDC,text,slen,rect,fnAlign);                 
	
	// 요놈을 복사해서 뭘가지고 써먹든.. 써먹으면 됨...  
	BYTE *pbuf; 
	pbuf = (BYTE*)malloc(sizeof(BYTE)* dwSizeImage); 
	memcpy(pbuf,buf,dwSizeImage);   
	
	// 나머지 정리해보자...  
	if(hFont)   
		DeleteObject(hFont);  
	if(hBrush)  
		DeleteObject(hBrush);  
	if(hBitmap) 
		DeleteObject(hBitmap); 
	if(hDC)  
		DeleteDC(hDC);
	
	return pbuf; 
}

unsigned int GetBitsByPoppingWithNumber(std::list<bool> * bList, int number)
{
	unsigned int rt = 0;
	for(int i=0; i<number; i++)
	{
		rt*=2;
		if(bList->empty() == false)
		{
			if( bList->front() == true)
				rt += 1;
			bList->pop_front();
		}
	}
	return rt;
}