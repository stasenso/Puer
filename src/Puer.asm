include template.inc
.code
entry_point proc
    invoke GetModuleHandle, NULL    ; Взять хэндл пpогpаммы
    mov hInstance,rax               ; Под Win32, hmodule==hinstance mov hInstance,eax
    
    invoke WinMain, hInstance,NULL,CommandLine, SW_SHOWDEFAULT  ; вызвать основную функцию
    invoke ExitProcess, rax ; Выйти из пpогpаммы.
                            ; Возвpащаемое значение, помещаемое в eax, беpется из WinMain'а.
entry_point endp
;______________________________________________________________________________________________________________________________________________________________
WinMain proc hInst:HINSTANCE,hPrevInst:HINSTANCE,CmdLine:LPSTR,CmdShow:QWORD
    ; создание локальных пеpеменных в стеке      
    LOCAL msg:MSG
    LOCAL hwnd:HWND
    ;регистрация оконного класса
    push  hInstance
    pop   wc.hInstance
    invoke LoadIcon,wc.hInstance,IDI_ICON
    mov   wc.hIcon,rax
    mov   wc.hIconSm,rax
    invoke LoadCursor,NULL,IDC_ARROW
    mov   wc.hCursor,rax
    invoke RegisterClassEx, addr wc  ; pегистpация нашего класса окнаW
    invoke CreateWindowEx,NULL,ADDR szClassName,ADDR AppName,WS_OVERLAPPEDWINDOW,282,0,1354,1017,NULL,NULL,hInst,NULL
    mov   hwnd,rax
    invoke ShowWindow,hwnd,CmdShow ; отобpазить наше окно на десктопе (вместо CmdShow можно указать свойство 3 - во весь экран)
    invoke UpdateWindow, hwnd ; обновить клиентскую область
    .while TRUE   ; Enter message loop
       invoke GetMessage, ADDR msg,NULL,0,0
    .break .if (rax==0)
       invoke TranslateMessage, ADDR msg
       invoke DispatchMessage, ADDR msg
    .endw
     mov     rax,msg.wParam ; сохpанение возвpащаемого значения в eax
     ret
WinMain endp
;______________________________________________________________________________________________________________________________________________________________
WndProc proc hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM
    LOCAL hDC:HDC    
    .if uMsg==WM_DESTROY           ; если пользователь закpывает окно
        invoke Shell_NotifyIconA,NIM_DELETE,addr note
        invoke DestroyMenu,hPopupMenu
        invoke SetThreadExecutionState,ES_CONTINUOUS
        invoke PostQuitMessage,NULL ; выходим из пpогpаммы
    .elseif uMsg==WM_CREATE
        invoke CreatePopupMenu
        mov hPopupMenu,rax  ;Создаём pop-up меню для взаимодействия через трей
           invoke AppendMenuA,hPopupMenu,MF_STRING,IDM_RESTORE,addr RestoreString
           invoke AppendMenuA,hPopupMenu,MF_STRING,IDM_EXIT,addr ExitString
        invoke LoadIcon,hInstance,IDI_ICON
        mov hIcon,rax
        .if rax==0
            invoke LoadIcon,NULL,IDI_APPLICATION
            mov hIcon,rax
        .endif
        mov note.hIcon,rax
        ;Запрещаем переход в сон и отключение дисплея
        invoke SetThreadExecutionState,ES_CONTINUOUS or ES_SYSTEM_REQUIRED or ES_DISPLAY_REQUIRED
        .if rax ==-1
            invoke PostQuitMessage,NULL ; выходим из пpогpаммы
        .else
            mov sleepBlockEnabled,1
            mov rax,0
            ret
        .endif
        
    .elseif uMsg==WM_SIZE
        .if wParam==SIZE_MINIMIZED
            mov note.cbSize,SIZEOF TRAYICONDATAA
            mov rax,hWnd
            mov note.hWnd,rax
            mov note.uID,IDI_TRAY
            mov note.uFlags,NIF_ICON or NIF_MESSAGE or NIF_TIP
            mov note.uCallbackMessage,WM_SHELLNOTIFY
            mov rax,hIcon
            mov note.hIcon,rax
            invoke lstrcpyA,addr note.szTip,addr AppName
            invoke Shell_NotifyIconA,NIM_ADD,addr note
            .if rax==0
            .else
                invoke ShowWindow,hWnd,SW_HIDE
            .endif
        .endif
        mov rax,0
        ret
    .elseif uMsg==WM_SHELLNOTIFY
        .if wParam==IDI_TRAY
            .if lParam==WM_LBUTTONDBLCLK
                .if sleepBlockEnabled==1
                    invoke SetThreadExecutionState,ES_CONTINUOUS
                    invoke LoadIcon,hInstance,IDI_ICON2
                    .if rax==0
                        invoke LoadIcon,NULL,IDI_APPLICATION
                    .endif
                    mov hIcon,rax
                    mov note.hIcon,rax
                    mov note.uFlags,NIF_ICON
                    invoke Shell_NotifyIconA,NIM_MODIFY,addr note
                    mov sleepBlockEnabled,0
                .else
                    invoke SetThreadExecutionState,ES_CONTINUOUS or ES_SYSTEM_REQUIRED or ES_DISPLAY_REQUIRED
                    invoke LoadIcon,hInstance,IDI_ICON
                    .if rax==0
                        invoke LoadIcon,NULL,IDI_APPLICATION
                    .endif
                    mov hIcon,rax
                    mov note.hIcon,rax
                    mov note.uFlags,NIF_ICON
                    invoke Shell_NotifyIconA,NIM_MODIFY,addr note
                    mov sleepBlockEnabled,1
                .endif
            .endif
        .endif
        mov rax,0
        ret
    .elseif uMsg==WM_PAINT
        invoke myPaint,hWnd
    .elseif uMsg==WM_ERASEBKGND
        mov rax,TRUE
        ret
     .else
        invoke DefWindowProc,hWnd,uMsg,wParam,lParam ; Дефолтная функция обpаботки окна
        ret
    .endif
    xor rax,rax

    ret
WndProc endp
;______________________________________________________________________________________________________________________________________________________________

;______________________________________________________________________________________________________________________________________________________________

;______________________________________________________________________________________________________________________________________________________________
myPaint proc hWndPaint:HWND
    LOCAL ps:PAINTSTRUCT
    invoke BeginPaint,hWndPaint,ptr$(ps)            ;тут создаётся контекст устройства окна


    invoke EndPaint,hWndPaint,ptr$(ps)
    ret
myPaint endp

end


