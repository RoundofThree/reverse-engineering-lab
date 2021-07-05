/*
    Sample L. sub_1000CEA0. 
*/

/*
                             **************************************************************
                             * Library Function - Single Match                            *
                             *  _strrchr                                                  *
                             *                                                            *
                             * Library: Visual Studio                                     *
                             **************************************************************
                             char * __cdecl _strrchr(char * _Str, int _Ch)
             char *            EAX:4          <RETURN>
             char *            Stack[0x4]:4   _Str                                    XREF[1]:     1000cea4(R)  
             int               Stack[0x8]:4   _Ch                                     XREF[1]:     1000ceb6(R)  
                             _strrchr                                        XREF[5]:     FUN_10007a4b:10007c22(c), 
                                                                                          FUN_1000ad4d:1000ad87(c), 
                                                                                          FUN_1000ad4d:1000ad95(c), 
                                                                                          FUN_1000ad4d:1000ada3(c), 
                                                                                          FUN_1000b014:1000b135(c)  
        1000cea0 55              PUSH       EBP
        1000cea1 8b ec           MOV        EBP,ESP
        1000cea3 57              PUSH       EDI
        1000cea4 8b 7d 08        MOV        EDI,dword ptr [EBP + _Str]
        1000cea7 33 c0           XOR        EAX,EAX
        1000cea9 83 c9 ff        OR         ECX,0xffffffff
        1000ceac f2 ae           SCASB.RE   ES:EDI
        1000ceae 83 c1 01        ADD        ECX,0x1
        1000ceb1 f7 d9           NEG        ECX
        1000ceb3 83 ef 01        SUB        EDI,0x1
        1000ceb6 8a 45 0c        MOV        AL,byte ptr [EBP + _Ch]
        1000ceb9 fd              STD
        1000ceba f2 ae           SCASB.RE   ES:EDI
        1000cebc 83 c7 01        ADD        EDI,0x1
        1000cebf 38 07           CMP        byte ptr [EDI],AL
        1000cec1 74 04           JZ         LAB_1000cec7
        1000cec3 33 c0           XOR        EAX,EAX
        1000cec5 eb 02           JMP        LAB_1000cec9
                             LAB_1000cec7                                    XREF[1]:     1000cec1(j)  
        1000cec7 8b c7           MOV        EAX,EDI
                             LAB_1000cec9                                    XREF[1]:     1000cec5(j)  
        1000cec9 fc              CLD
        1000ceca 5f              POP        EDI
        1000cecb c9              LEAVE
        1000cecc c3              RET


 */

char* __cdecl _strrchr(char* _Str, char _Ch) 
{
  char cVar1;
  int iVar2 = 0xffffffff;  // ecx 
  char *pcVar3;
  char *pcVar4;            // edi = _str 
  
  iVar2 = -1;
  do {  // scasb.re
    pcVar4 = _Str;
    if (iVar2 == 0) break;
    iVar2 = iVar2 + -1;
    pcVar4 = _Str + 1;
    cVar1 = *_Str;
    _Str = pcVar4;
  } while (cVar1 != '\0');
  iVar2 = -(iVar2 + 1);     // get length from 2 complement 
  pcVar4 = pcVar4 + -1;
  do {
    pcVar3 = pcVar4;
    if (iVar2 == 0) break;
    iVar2 = iVar2 + -1;
    pcVar3 = pcVar4 + -1;
    cVar1 = *pcVar4;
    pcVar4 = pcVar3;
  } while ((char)_Ch != cVar1);
  pcVar3 = pcVar3 + 1;
  if (*pcVar3 != (char)_Ch) {
    pcVar3 = (char *)0x0;
  }
  return pcVar3;  // last matching character pointer, NULL if no one 
}