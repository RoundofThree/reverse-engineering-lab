/*
    Sample H. Decompile sub_11732 (actual sub_1172e) and explain the most likely programming construct 
    used in the original code. 
*/

/*
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined __fastcall FUN_0001172e(uint * param_1, int * 
             undefined         AL:1           <RETURN>
             uint *            ECX:4          param_1
             int *             EDX:4          param_2
             int               Stack[0x4]:4   param_3                                 XREF[1]:     0001172f(R)  
                             FUN_0001172e                                    XREF[2]:     FUN_00011798:000118ad(c), 
                                                                                          FUN_00011798:000118ef(c)  
        0001172e 56              PUSH       ESI
        0001172f 8b 74 24 08     MOV        ESI,dword ptr [ESP + param_3]
        00011733 4e              DEC        ESI
        00011734 74 29           JZ         LAB_0001175f      ; if param3-1 is 0, jmp 
        00011736 4e              DEC        ESI
        00011737 74 1c           JZ         LAB_00011755      ; if param3-2 is 0, jmp
        00011739 4e              DEC        ESI
        0001173a 74 0f           JZ         LAB_0001174b      ; if param3-3 is 0, jmp
        0001173c 83 ee 09        SUB        ESI,0x9   
        0001173f 75 2a           JNZ        LAB_0001176b      ; if param3-12 is not zero, return 
        00011741 8b 70 08        MOV        ESI,dword ptr [EAX + 0x8]
        00011744 d1 ee           SHR        ESI,1
        00011746 83 c0 0c        ADD        EAX,0xc
        00011749 eb 1c           JMP        LAB_00011767
                             LAB_0001174b                                    XREF[1]:     0001173a(j)  
        0001174b 8b 70 3c        MOV        ESI,dword ptr [EAX + 0x3c]
        0001174e d1 ee           SHR        ESI,1
        00011750 83 c0 5e        ADD        EAX,0x5e
        00011753 eb 12           JMP        LAB_00011767
                             LAB_00011755                                    XREF[1]:     00011737(j)  
        00011755 8b 70 3c        MOV        ESI,dword ptr [EAX + 0x3c]
        00011758 d1 ee           SHR        ESI,1
        0001175a 83 c0 44        ADD        EAX,0x44
        0001175d eb 08           JMP        LAB_00011767
                             LAB_0001175f                                    XREF[1]:     00011734(j)  
        0001175f 8b 70 3c        MOV        ESI,dword ptr [EAX + 0x3c]
        00011762 d1 ee           SHR        ESI,1
        00011764 83 c0 40        ADD        EAX,0x40
                             LAB_00011767                                    XREF[3]:     00011749(j), 00011753(j), 
                                                                                          0001175d(j)  
        00011767 89 31           MOV        dword ptr [param_1],ESI
        00011769 89 02           MOV        dword ptr [param_2],EAX
                             LAB_0001176b                                    XREF[1]:     0001173f(j)  
        0001176b 5e              POP        ESI
        0001176c c2 04 00        RET        0x4
        0001176f cc              ??         CCh

*/

// Most likely it used a switch. 
void __fastcall FUN_0001172e(uint* param1, uint* param2, int param3) {
    int inEAX;  // from previous function 
    int inESI = param3; 
    if (param3 == 1) {
        inESI = *(inEAX + 0x3c) >> 1;
        inEAX += 0x40;
    } else if (param3 == 2) {
        inESI = *(inEAX + 0x3c) >> 1; 
        inEAX += 0x44; 
    } else if (param3 == 3) {
        inESI = *(inEAX + 0x3c) >> 1; 
        inEAX += 0x5e; 
    } else {
        if (param3 != 12) return; 
        inESI = *(inEAX + 0x8) >> 1; 
        inEAX += 0xc;
    } 
    *param1 = inESI; 
    *param2 = inEAX; 
    return;
}