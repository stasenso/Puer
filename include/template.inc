__UNICODE__ equ 1
include /masm64/include64/masm64rt.inc
include /masm64/include64/user32.inc
include /masm64/include64/shell32.inc
includelib /masm64/lib64/user32.lib
includelib /masm64/lib64/shell32.lib
;______________________Prototypes__________________________
WinMain proto :HINSTANCE,:HINSTANCE,:LPSTR,:QWORD

myPaint proto :HWND ; WM_PAINT handler
myDraw proto nSegment:DWORD ; segment draw helper

WM_SHELLNOTIFY equ WM_USER+5
IFNDEF NIM_SETVERSION
NIM_SETVERSION equ 4
ENDIF
IFNDEF NOTIFYICON_VERSION_4
NOTIFYICON_VERSION_4 equ 4
ENDIF
IFNDEF NIF_SHOWTIP
NIF_SHOWTIP equ 80h
ENDIF
;_______________________________________________________
.data?
hInstance HINSTANCE ?   ; app instance handle
CommandLine LPSTR ?
hIcon dq ?
sleepBlockEnabled dq ?

NOTIFYICONDATAW STRUCT
  cbSize            DWORD           ?
  _pad0             DWORD           ?
  hWnd              QWORD           ?
  uID               DWORD           ?
  uFlags            DWORD           ?
  uCallbackMessage  DWORD           ?
  _pad1             DWORD           ?
  hIcon             QWORD           ?
  szTip             WORD 128 dup(?)
  dwState           DWORD           ?
  dwStateMask       DWORD           ?
  szInfo            WORD 256 dup(?)
  uTimeoutOrVersion DWORD           ?
  szInfoTitle       WORD 64 dup(?)
  dwInfoFlags       DWORD           ?
  guidItem          BYTE 16 dup(?)
  hBalloonIcon      QWORD           ?
NOTIFYICONDATAW ENDS

BITMAPINFOHEADER STRUCT
    biSize DWORD ?;
    biWidth LONG ?;
    biHeight LONG ?;
    biPlanes WORD  ?;
    biBitCount WORD  ?;
    biCompression DWORD ?;
    biSizeImage DWORD ?;
    biXPelsPerMeter LONG ?;
    biYPelsPerMeter LONG ?;
    biClrUsed DWORD ?;
    biClrImportant DWORD ?;
BITMAPINFOHEADER ENDS
RGBQUAD STRUCT
    rgbBlue       BYTE      ?
    rgbGreen      BYTE      ?
    rgbRed        BYTE      ?
    rgbReserved   BYTE      ?
RGBQUAD ENDS
BITMAPINFO STRUCT
  bmiHeader  BITMAPINFOHEADER <>
  bmiColors  RGBQUAD <>
BITMAPINFO ENDS
hPopupMenu dq ?

;_______________________________________________________
.data
szClassName dw "C","o","c","a","C","l","a","s","s",0 ; window class name
AppName dw 0050h,0075h,0027h,0065h,0072h,0020h,0074h,0065h,0061h,003ah,0020h,041dh,0430h,0020h,0440h,0430h,0431h,043eh,0442h,0435h,0020h,043dh,0435h,0020h,0441h,043fh,0438h,043ch,0021h,0D83Dh,0DE00h,0; window title / tray tip
ELAPSE_UPDTIMER EQU 20
IDT_UPDTIMER    EQU 111
wc  WNDCLASSEX <sizeof WNDCLASSEX,CS_HREDRAW or CS_VREDRAW,WndProc,0,0,?,?,?,COLOR_WINDOW+1,NULL,szClassName,?>
bmi BITMAPINFO <>
rc RECT <>
RestoreString dw "&","R","e","s","t","o","r","e",0
ExitString   dw 0412h,0026h,044bh,0445h,043eh,0434h,0
EnableSleep dw 0420h,0430h,0437h,0440h,0435h,0448h,0438h,0442h,044ch,0020h,0441h,043eh,043dh,0
DisableSleep dw 0417h,0430h,043fh,0440h,0435h,0442h,0438h,0442h,044ch,0020h,0441h,043eh,043dh,0
note NOTIFYICONDATAW <>
IDI_TRAY equ 1
IDM_RESTORE equ 1000
IDM_ENABLESLEEP equ 1100
IDM_DISABLESLEEP equ 1110

IDM_EXIT equ 1010

IDI_ICON equ 1020
IDI_ICON2 equ 1030
