// Ex 6, sample H, sub_13842.
// Task: recover function prototype and reconstruct the struct. 
/*
  sub_13842 SAMPLE_H       
                                         
  mov     eax, [ecx+60h]  ; 
  push    esi
  mov     esi, [edx+8]
  dec     byte ptr [ecx+23h] 
  sub     eax, 24h
  mov     [ecx+60h], eax  ; arg1->f96 -= 36; 
  mov     [eax+14h], edx  ; arg1->f96->f16 = arg2 (a PTR)
  movzx   eax, byte ptr [eax] ; eax = (uint8_t)*(arg1->f96)
  push    ecx
  push    edx
  call    dword ptr [esi+eax*4+38h] ; shellcode at arg2->f8 + 4*arg1->f96->f0 + 56?
  pop     esi
  retn
*/
// eax = *(arg1 + 96)
// esi = *(arg2 + 8)
// Probably fastcall, seems like ecx and edx are the arguments
struct something2 {
    void* f8; 
};

struct something3 {  // >= 24 bytes 
    uint8_t f0;   
    something2* f16;
};
 
struct something1 {  // >= 100 bytes 
    uint8_t f35;  // counter?? Gets decremented 
    something3* f96;  // eax = f96, eax -= 36; 
}; 

