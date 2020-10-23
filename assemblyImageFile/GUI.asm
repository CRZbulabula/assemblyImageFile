.386
.Model Flat, StdCall
Option Casemap :None

;==================== INCLUDE =======================
INCLUDE		header.inc
INCLUDE		structure.inc
INCLUDE		images.inc
INCLUDE		dll.inc

;==================== �Ҹ����¸���� =======================
	printf				PROTO C :ptr sbyte, :VARARG

	WinMain				PROTO :DWORD, :DWORD, :DWORD, :DWORD
	WndProc				PROTO :DWORD, :DWORD, :DWORD, :DWORD
	UnicodeStr			PROTO :DWORD, :DWORD
	LoadImageFromFile	PROTO :PTR BYTE, :DWORD
	gdiplusLoadBitmapFromResource proto :HMODULE, :LPSTR, :LPSTR, :DWORD

;==================== DATA =======================
.data

	interfaceID		DWORD 0	; ��ǰ�����Ľ��棬0�ǳ�ʼ���棬1�Ǵ�ͼƬ��2�������
	openStatus		DWORD 0	; ���ư�ť״̬
	cameraStatus	DWORD 0
	exitStatus		DWORD 0

	szClassName		BYTE "MASMPlus_Class",0
	WindowName		BYTE "IMAGE", 0

	;��ʼ��gdi+����
	gdiplusToken	DD ?
	gdiplusSInput	GdiplusStartupInput <1, NULL, FALSE, FALSE>

.data?
	hInstance           DD ?
	hBitmap             DD ?
	pNumbOfBytesRead    DD ?
	StartupInfo         GdiplusStartupInput <?>
	UnicodeFileName     DD 32 DUP(?)
	BmpImage            DD ?
	token               DD ?

	background			DD ?
	emptyBtn			DD ?
	openBtn				DD ?
	openHoverBtn		DD ?
	cameraBtn			DD ?
	cameraHoverBtn		DD ?
	exitBtn				DD ?
	exitHoverBtn		DD ?

	curLocation			location <?>

;=================== CODE =========================
.code
START:
	INVOKE	GetModuleHandle, NULL
	mov		hInstance, eax
	INVOKE	GdiplusStartup, ADDR gdiplusToken, ADDR gdiplusSInput, NULL
	INVOKE	WinMain, hInstance, NULL, NULL, SW_SHOWDEFAULT
	INVOKE	GdiplusShutdown, gdiplusToken
	INVOKE	ExitProcess, 0

WinMain PROC hInst:DWORD, hPrevInst:DWORD, CmdLine:DWORD, CmdShow:DWORD
	LOCAL wc   :WNDCLASSEX
	LOCAL msg  :MSG
	LOCAL hWnd :HWND
	
	mov wc.cbSize, SIZEOF WNDCLASSEX
	mov wc.style, CS_HREDRAW or CS_VREDRAW or CS_BYTEALIGNWINDOW
	mov wc.lpfnWndProc, OFFSET WndProc
	mov wc.cbClsExtra, NULL
	mov wc.cbWndExtra, NULL
	push hInst
	pop wc.hInstance
	mov wc.hbrBackground, COLOR_BTNFACE+1
	mov wc.lpszMenuName, NULL
	mov wc.lpszClassName, OFFSET szClassName
	INVOKE LoadIcon, hInst, 100
	mov wc.hIcon, eax
	INVOKE LoadCursor, NULL, IDC_ARROW
	mov wc.hCursor, eax
	mov wc.hIconSm, 0

	INVOKE RegisterClassEx, ADDR wc
	INVOKE CreateWindowEx, NULL, ADDR szClassName, ADDR WindowName, 
		WS_OVERLAPPEDWINDOW, 460, 20, 1024, 768, NULL, NULL, hInst, NULL
	mov hWnd, eax
	INVOKE ShowWindow, hWnd, SW_SHOWNORMAL
	INVOKE UpdateWindow, hWnd
	
StartLoop:
	INVOKE GetMessage, ADDR msg, NULL, 0, 0
	cmp eax, 0
	je ExitLoop
	INVOKE TranslateMessage, ADDR msg
	INVOKE DispatchMessage, ADDR msg
	jmp StartLoop
ExitLoop:
	INVOKE KillTimer, hWnd, 1
	
	mov eax, msg.wParam
	ret
WinMain ENDP

WndProc PROC hWnd:DWORD, uMsg:DWORD, wParam :DWORD, lParam :DWORD
	LOCAL ps:PAINTSTRUCT
	LOCAL hdc:HDC
	LOCAL hMemDC:HDC
	LOCAL bm:BITMAP
	LOCAL graphics:HANDLE
	local pbitmap:HBITMAP
	local nhb:DWORD

	.IF uMsg == WM_CREATE

		; �򿪼�ʱ��
		INVOKE	SetTimer, hWnd, 1, 10, NULL

		; �����ļ��е�ͼ��
		INVOKE	LoadImageFromFile, OFFSET bkImage, ADDR background
		INVOKE	LoadImageFromFile, OFFSET btnImage, ADDR emptyBtn
		INVOKE	LoadImageFromFile, OFFSET openImage, ADDR openBtn
		INVOKE	LoadImageFromFile, OFFSET openHoverImage, ADDR openHoverBtn
		INVOKE	LoadImageFromFile, OFFSET cameraImage, ADDR cameraBtn
		INVOKE	LoadImageFromFile, OFFSET cameraHoverImage, ADDR cameraHoverBtn
		INVOKE	LoadImageFromFile, OFFSET exitImage, ADDR exitBtn
		INVOKE	LoadImageFromFile, OFFSET exitHoverImage, ADDR exitHoverBtn

	.ELSEIF uMsg == WM_PAINT

		INVOKE  BeginPaint, hWnd, ADDR ps
		mov     hdc, eax

		INVOKE  CreateCompatibleDC, hdc
		mov     hMemDC, eax
		invoke  CreateCompatibleBitmap, hdc, 1024, 768		; ������ʱλͼpbitmap
		mov		pbitmap, eax
		INVOKE  SelectObject, hMemDC, pbitmap
		INVOKE  GdipCreateFromHDC, hMemDC, ADDR graphics	; ������ͼ����graphics


		.IF interfaceID == 0

			; ���Ƴ�ʼ����
			INVOKE	GdipDrawImagePointRectI, graphics, background, 0, 0, 0, 0, 1024, 768, 2
			
			.IF openStatus == 0
				INVOKE	GdipDrawImagePointRectI, graphics, emptyBtn, openLocation.x, openLocation.y, 0, 0, openLocation.w, openLocation.h, 2
				INVOKE	GdipDrawImagePointRectI, graphics, openBtn, openLocation.x, openLocation.y, 0, 0, openLocation.w, openLocation.h, 2
			.ELSE
				INVOKE	GdipDrawImagePointRectI, graphics, emptyBtn, openLocation.x, openLocation.y, 0, 0, openLocation.w, openLocation.h, 2
				INVOKE	GdipDrawImagePointRectI, graphics, openHoverBtn, openLocation.x, openLocation.y, 0, 0, openLocation.w, openLocation.h, 2
				;INVOKE	GdipDrawImagePointRectI, graphics, cameraBtn, openLocation.x, edx, 0, 0, openLocation.w, openLocation.h, 2

				; ����DLL����
				; ����DLL
				INVOKE	LoadLibrary, OFFSET OpenCVDLL
				mov		hLIB, eax
				
				; ���غ���
				INVOKE	GetProcAddress, hLIB, OFFSET smFunction
				mov		smFunc, eax
				
				; ���ú���
				mov		edx, OFFSET smwgyImage
				push	edx
				mov		edx, OFFSET wgyImage
				push	edx
				call	smFunc
				pop		edx
				pop		edx

			.ENDIF
			.IF cameraStatus == 0
				INVOKE	GdipDrawImagePointRectI, graphics, cameraBtn, cameraLocation.x, cameraLocation.y, 0, 0, cameraLocation.w, cameraLocation.h, 2
			.ELSE
				INVOKE	GdipDrawImagePointRectI, graphics, cameraHoverBtn, cameraLocation.x, cameraLocation.y, 0, 0, cameraLocation.w, cameraLocation.h, 2
			.ENDIF
			.IF exitStatus == 0
				INVOKE	GdipDrawImagePointRectI, graphics, exitBtn, exitLocation.x, exitLocation.y, 0, 0, exitLocation.w, exitLocation.h, 2
			.ELSE
				INVOKE	GdipDrawImagePointRectI, graphics, exitHoverBtn, exitLocation.x, exitLocation.y, 0, 0, exitLocation.w, exitLocation.h, 2
			.ENDIF

		.ENDIF

		INVOKE  BitBlt, hdc, 0, 0, 1024, 768, hMemDC, 0, 0, SRCCOPY		; ��ͼ
			
		; �ͷ��ڴ�
		INVOKE	GdipDeleteGraphics, graphics
		INVOKE	DeleteObject, pbitmap
		INVOKE  DeleteDC, hMemDC
		INVOKE  EndPaint, hWnd, ADDR ps

	.ELSEIF uMsg == WM_MOUSEMOVE
		.IF interfaceID == 0

			; ��ȡ��ǰ�������
			mov eax, lParam
			and eax, 0000FFFFh	; x����
			mov ebx, lParam
			shr ebx, 16			; y����
			
			; �жϳ�ʼ����İ�ť״̬
			mov openStatus, 0
			.IF eax > openLocation.x
				mov ecx, openLocation.x
				add ecx, openLocation.w
				.IF eax < ecx
					.IF ebx > openLocation.y
						mov ecx, openLocation.y
						add ecx, openLocation.h
						.IF ebx < ecx
							mov edx, 1
							mov openStatus, edx
							;invoke SendMessage, hWnd, WM_PAINT, NULL, NULL
						.ENDIF
					.ENDIF
				.ENDIF
			.ENDIF

		.ENDIF

	.ELSEIF uMsg == WM_TIMER

		; ���ݶ�ʱ����ʱ���½���
		invoke SendMessage, hWnd, WM_PAINT, NULL, NULL

	.ELSEIF uMsg == WM_DESTROY
	
		INVOKE  PostQuitMessage, NULL
		
	.ELSE
	
		INVOKE  DefWindowProc, hWnd, uMsg, wParam, lParam		
		ret

	.ENDIF
	
	xor  eax, eax
	ret
WndProc	ENDP

;-----------------------------------------------------
LoadImageFromFile	PROC FileName:PTR BYTE, Bitmap:DWORD
; ���ļ��ж�ȡͼƬת����Bitmap������Bitmap
;-----------------------------------------------------
	mov     eax, OFFSET StartupInfo
	mov     GdiplusStartupInput.GdiplusVersion[eax], 1

	INVOKE  GdiplusStartup, ADDR token, ADDR StartupInfo, 0
	INVOKE  UnicodeStr, FileName, ADDR UnicodeFileName
								
	INVOKE  GdipCreateBitmapFromFile, ADDR UnicodeFileName, Bitmap

	;INVOKE  GdipCreateBitmapFromFile, ADDR UnicodeFileName, ADDR BmpImage
	;INVOKE  GdipCreateHBITMAPFromBitmap, BmpImage, Bitmap, 0
	ret
LoadImageFromFile	ENDP

;-----------------------------------------------------
UnicodeStr	PROC USES esi ebx Source:DWORD, Dest:DWORD
; ���ڽ�ͼƬ����ת����Unicode�ַ���
;-----------------------------------------------------
	mov     ebx, 1
	mov     esi, Source
	mov     edx, Dest
	xor     eax, eax
	sub     eax, ebx
@@:
	add     eax, ebx
	movzx   ecx, BYTE PTR [esi + eax]
	mov     WORD PTR [edx + eax * 2], cx
	test    ecx, ecx
	jnz     @b
	ret
UnicodeStr	ENDP

END START