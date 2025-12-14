include /masm64/include64/masm64rt.inc
;______________________Прототипы__________________________
WinMain proto :HINSTANCE,:HINSTANCE,:LPSTR,:QWORD

myPaint proto :HWND ;Обработка сообщения WM_PAINT
myDraw proto nSegment:DWORD ;принимаю параметр с колличеством сегментов (в HIWORD) и номером данного сегмента (в LOWORD) 
;_______________________________________________________
.data?
hInstance HINSTANCE ?   ; Хэндл нашей пpогpаммы
CommandLine LPSTR ?
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


;_______________________________________________________
.data
szClassName db "CocaClass",0 ; Имя нашего класса окна
AppName db "Cocaine - no sleeping on the job!",0 ; Имя нашего окна
ELAPSE_UPDTIMER EQU 20
IDT_UPDTIMER    EQU 111
wc  WNDCLASSEX <sizeof WNDCLASSEX,CS_HREDRAW or CS_VREDRAW,WndProc,0,0,?,?,?,COLOR_WINDOW+1,NULL,szClassName,?>
bmi BITMAPINFO <>
rc RECT <>



