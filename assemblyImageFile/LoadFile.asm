.386
.Model Flat, StdCall
Option Casemap :None

INCLUDE		header.inc

.data
;����Ϊ�ⲿ����
EXTERN		StartupInfo:		GdiplusStartupInput
EXTERN		UnicodeFileName:	DWORD
EXTERN		token:				DWORD
EXTERN		ofn:				OPENFILENAME

;�ⲿ�ɵ��õĺ���
PUBLIC		LoadImageFromFile
PUBLIC		GetFileNameFromDialog

.code
;-----------------------------------------------------
UnicodeStr	PROC USES esi Source:DWORD, Dest:DWORD
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
	mov		WORD PTR [edx + eax * 2 + 1], 0
	test    ecx, ecx
	jnz     @b
	ret
UnicodeStr	ENDP

;-----------------------------------------------------
LoadImageFromFile	PROC FileName:PTR BYTE, Bitmap:DWORD
; ���ļ��ж�ȡͼƬת����Bitmap������Bitmap
;-----------------------------------------------------
	;mov     eax, OFFSET StartupInfo
	;mov     GdiplusStartupInput.GdiplusVersion[eax], 1
	;INVOKE  GdiplusStartup, ADDR token, ADDR StartupInfo, 0
	
	INVOKE  UnicodeStr, FileName, ADDR UnicodeFileName
	INVOKE  GdipCreateBitmapFromFile, ADDR UnicodeFileName, Bitmap

	;INVOKE  GdipCreateBitmapFromFile, ADDR UnicodeFileName, ADDR BmpImage
	;INVOKE  GdipCreateHBITMAPFromBitmap, BmpImage, Bitmap, 0
	ret
LoadImageFromFile	ENDP

;-----------------------------------------------------
GetFileNameFromDialog	PROC USES esi filter_string:DWORD, initial_dir:DWORD, filename:DWORD, dialog_title:DWORD
; ��ѡ���ļ��Ի��� 
; https://www.daimajiaoliu.com/daima/37f6f0d89900406/huibianzhongshiyongdakaiduihuakuang
; https://blog.csdn.net/weixin_33835103/article/details/91893316
;-----------------------------------------------------
	INVOKE	RtlZeroMemory, addr ofn, sizeof ofn
	mov ofn.lStructSize, sizeof ofn		;�ṹ�Ĵ�С
	mov esi, filter_string
	mov ofn.lpstrFilter, esi	;�ļ�������
	mov esi, initial_dir
	mov ofn.lpstrInitialDir, esi ; ��ʼĿ¼
	mov esi, filename
	mov ofn.lpstrFile, esi	;�ļ����Ĵ��λ��
	mov ofn.nMaxFile, 256	;�ļ�������󳤶�
	mov ofn.Flags, OFN_FILEMUSTEXIST or OFN_HIDEREADONLY or OFN_LONGNAMES
	mov esi, dialog_title
	mov ofn.lpstrTitle, esi	;���򿪡��Ի���ı���
	INVOKE GetOpenFileName, addr ofn	;��ʾ�򿪶Ի���
	ret
GetFileNameFromDialog	ENDP

END