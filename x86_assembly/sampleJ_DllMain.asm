01:    ; BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason,
       ; LPVOID lpvReserved)
       ; local variables (total 304 bytes):
       ; [ebp-8] 6 bytes truncated to 8 bytes 
       ; [ebp-12Ch] (-300), likely takes 300-8 = 292 bytes 
       ; [ebp-130h], 4 bytes 
02:                _DllMain@12 proc near   ; stdcall with 12 bytes arguments (3 * 4 bytes)
03: 55               push    ebp
04: 8B EC            mov     ebp, esp
05: 81 EC 30 01 00+  sub     esp, 130h     ; 304 bytes local variables 
06: 57               push    edi           ; edi is ? argument?
07: 0F 01 4D F8      sidt    fword ptr [ebp-8] ; __sidt(&idtr); get the interrupt table descriptor register
08: 8B 45 FA         mov     eax, [ebp-6]  ; idtr.base (since idtr.limit is a SHORT)
09: 3D 00 F4 03 80   cmp     eax, 8003F400h  ; if idtr.base <= 8003F400 or idtr.base >= 80047400, goto line 18 
10: 76 10            jbe     short loc_10001C88 ; (line 18)
11: 3D 00 74 04 80   cmp     eax, 80047400h
12: 73 09            jnb     short loc_10001C88 ; (line 18)
13: 33 C0            xor     eax, eax  ; return 0 
14: 5F               pop     edi
15: 8B E5            mov     esp, ebp
16: 5D               pop     ebp
17: C2 0C 00         retn    0Ch  ; because its stdcall, the callee clears the 12 bytes
18:                loc_10001C88:
19: 33 C0            xor     eax, eax  ; ret = 0
20: B9 49 00 00 00   mov     ecx, 49h  ; ecx = 73
21: 8D BD D4 FE FF+  lea     edi, [ebp-12Ch] ; edi = *local var2
22: C7 85 D0 FE FF+  mov     dword ptr [ebp-130h], 0 ; set local var3 to 0 
23: 50               push    eax ; 0x0, argument 2 to CreateToolhelp32Snapshot
24: 6A 02            push    2  ; 0x2, argument 1 
25: F3 AB            rep stosd  ; eax = nil, memset local var2 to 0, 73 * 4 = 292 bytes
26: E8 2D 2F 00 00   call    CreateToolhelp32Snapshot
27: 8B F8            mov     edi, eax ; edi = HANDLE 
28: 83 FF FF         cmp     edi, 0FFFFFFFFh 
29: 75 09            jnz     short loc_10001CB9 ; if edi != -1, goto line 35
30: 33 C0            xor     eax, eax
31: 5F               pop     edi
32: 8B E5            mov     esp, ebp
33: 5D               pop     ebp
34: C2 0C 00         retn    0Ch
35:                loc_10001CB9:
36: 8D 85 D0 FE FF+  lea     eax, [ebp-130h] 
37: 56               push    esi  ; ??
38: 50               push    eax  ; *local var3
39: 57               push    edi  ; HANDLE, return of CreateToolhelp32Snapshot
40: C7 85 D0 FE FF+  mov     dword ptr [ebp-130h], 128h ; local var3 = 296, likely dwSize
41: E8 FF 2E 00 00   call    Process32First ; local var3 (merged with local var2) has type PROCESSENTRY32 (296 bytes)
42: 85 C0            test    eax, eax ; BOOL return of Process32First
43: 74 4F            jz      short loc_10001D24 ; if eax == FALSE go to (line 70)
44: 8B 35 C0 50 00+  mov     esi, ds:_stricmp 
45: 8D 8D F4 FE FF+  lea     ecx, [ebp-10Ch]  ; ebp-268: 296-(268-8) = 36 byte from PROCESSENTRY32, szExeFile
46: 68 50 7C 00 10   push    10007C50h ; some static data location
47: 51               push    ecx
48: FF D6            call    esi ; _stricmp(p_entry.szExeFile, 0x10007C50)
49: 83 C4 08         add     esp, 8 ; _stricmp is cdecl 
50: 85 C0            test    eax, eax
51: 74 26            jz      short loc_10001D16 ; if _stricmp return is 0, goto (line 66)
52:                loc_10001CF0:
53: 8D 95 D0 FE FF+  lea     edx, [ebp-130h]
54: 52               push    edx 
55: 57               push    edi
56: E8 CD 2E 00 00   call    Process32Next ; Process32Next(handle of CreateTool..., local var PROCESSENTRY32)
57: 85 C0            test    eax, eax
58: 74 23            jz      short loc_10001D24 ; if return is 0, goto (line 70)
59: 8D 85 F4 FE FF+  lea     eax, [ebp-10Ch] ; local var3 szExeFile
60: 68 50 7C 00 10   push    10007C50h 
61: 50               push    eax
62: FF D6            call    esi ; _stricmp(p_entry.szExeFile, 0x10007C50)
63: 83 C4 08         add     esp, 8
64: 85 C0            test    eax, eax
65: 75 DA            jnz     short loc_10001CF0 ; if ret != 0, start the routine again (line 52)
66:                loc_10001D16:
67: 8B 85 E8 FE FF+  mov     eax, [ebp-118h] ; ????
68: 8B 8D D8 FE FF+  mov     ecx, [ebp-128h]
69: EB 06            jmp     short loc_10001D2A ; goto (line 73)
70:                loc_10001D24:
71: 8B 45 0C         mov     eax, [ebp+0Ch] ; argument (12 - 4) / 4 = 2, fdwReason????? or lpvReserved?
72: 8B 4D 0C         mov     ecx, [ebp+0Ch]
73:                loc_10001D2A:
74: 3B C1            cmp     eax, ecx
75: 5E               pop     esi  ; argument ? fwdReason
76: 75 09            jnz     short loc_10001D38 ; if fwdReason != 0 goto (line 82)
77: 33 C0            xor     eax, eax ; else return false
78: 5F               pop     edi
79: 8B E5            mov     esp, ebp
80: 5D               pop     ebp
81: C2 0C 00         retn    0Ch
82:                loc_10001D38:
83: 8B 45 0C         mov     eax, [ebp+0Ch]
84: 48               dec     eax
85: 75 15            jnz     short loc_10001D53 ; if fwdReason-1 != 0 goto (line 93)
86: 6A 00            push    0
87: 6A 00            push    0
88: 6A 00            push    0
89: 68 D0 32 00 10   push    100032D0h ; LPTHREAD_START_ROUTINE
90: 6A 00            push    0
91: 6A 00            push    0
92: FF 15 20 50 00+  call    ds:CreateThread ; CreateThread(0, 0, 100032D0, 0, 0, 0)
93:                loc_10001D53:
94: B8 01 00 00 00   mov     eax, 1 ; return true 
95: 5F               pop     edi
96: 8B E5            mov     esp, ebp
97: 5D               pop     ebp
98: C2 0C 00         retn    0Ch
99:                _DllMain@12 endp