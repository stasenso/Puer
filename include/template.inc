include /masm64/include64/masm64rt.inc
include /masm64/include64/user32.inc
includelib /masm64/lib64/user32.lib
;______________________Прототипы__________________________
WinMain proto :HINSTANCE,:HINSTANCE,:LPSTR,:QWORD

myPaint proto :HWND ;Обработка сообщения WM_PAINT
myDraw proto nSegment:DWORD ;принимаю параметр с колличеством сегментов (в HIWORD) и номером данного сегмента (в LOWORD) 
;_______________________________________________________
.data?
hInstance HINSTANCE ?   ; Хэндл нашей пpогpаммы
CommandLine LPSTR ?
hIcon dq ?    ; icon handle

NOTIFYICONDATAW STRUCT
  cbSize            DWORD      ?
  hwnd              DWORD      ?
  uID               DWORD      ?
  uFlags            DWORD      ?
  uCallbackMessage  DWORD      ?
  hIcon             DWORD      ?
  szTip             WORD 64 dup (?)
                    WORD 64 dup (?)
  dwState           DWORD      ?
  dwStateMask       DWORD      ?
  szInfo            WORD 256 dup(?)
  union DUMMYUNIONNAME
  uTimeout          DWORD      ?
  uVersion          DWORD      ?
  ends
  szInfoTitle       WORD 64 dup(?)
  dwInfoFlags       DWORD      ?
  guidItem          GUID <>
  hBalloonIcon      DWORD      ?
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

note NOTIFYICONDATAW <>
hPopupMenu dq ?

;_______________________________________________________
.data
szClassName db "CocaClass",0 ; Имя нашего класса окна
AppName db "Cocaine - no sleeping on the job!",0 ; Имя нашего окна
ELAPSE_UPDTIMER EQU 20
IDT_UPDTIMER    EQU 111
wc  WNDCLASSEX <sizeof WNDCLASSEX,CS_HREDRAW or CS_VREDRAW,WndProc,0,0,?,?,?,COLOR_WINDOW+1,NULL,szClassName,?>
bmi BITMAPINFO <>
rc RECT <>
RestoreString db "&Restore",0
ExitString   db "E&xit Program",0

IDI_TRAY equ 0
IDM_RESTORE equ 1000

IDM_EXIT equ 1010

IDI_ICON equ 1020