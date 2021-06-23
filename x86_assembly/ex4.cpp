#include <windows.h>
/* wdm.h
The KeInitializeDpc routine initializes a DPC object, and 
registers a CustomDpc routine for that object.
*/

/*
  ASSEMBLY KeInitializeDpc
  .text:0040E982                 mov     edi, edi
  .text:0040E984                 push    ebp
  .text:0040E985                 mov     ebp, esp
  .text:0040E987                 mov     eax, [ebp+Dpc] 
  .text:0040E98A                 mov     ecx, [ebp+DeferredRoutine]
  .text:0040E98D                 and     dword ptr [eax+1Ch], 0 ; Dpc + 28 bytes = Lock
  .text:0040E991                 mov     [eax+0Ch], ecx ; Dpc->DeferredRoutine = DeferredRoutine
  .text:0040E994                 mov     ecx, [ebp+DeferredContext]
  .text:0040E997                 mov     word ptr [eax], 13h ; Type = 13
  .text:0040E99C                 mov     byte ptr [eax+2], 0 ; Number = 0 
  .text:0040E9A0                 mov     byte ptr [eax+3], 1 ; Importance = 1
  .text:0040E9A4                 mov     [eax+10h], ecx ; Dpc->DeferredContext = DeferredContext
  .text:0040E9A7                 pop     ebp
  .text:0040E9A8                 retn    0Ch
*/

/*
  kd> dt nt!_KDPC
   +0x000 Type             : Int2B
   +0x002 Number           : UChar
   +0x003 Importance       : UChar
   +0x004 DpcListEntry     : _LIST_ENTRY
   +0x00c DeferredRoutine  : Ptr32 Void
   +0x010 DeferredContext  : Ptr32 Void
   +0x014 SystemArgument1  : Ptr32 Void
   +0x018 SystemArgument2  : Ptr32 Void
   +0x01c Lock             : Ptr32 Uint4B
 */

void KeInitializeDpc(
  __drv_aliasesMem PRKDPC Dpc,
  PKDEFERRED_ROUTINE      DeferredRoutine,
  __drv_aliasesMem PVOID  DeferredContext
) {
    Dpc->Lock = NULL;
    Dpc->DeferredRoutine = DeferredRoutine;
    Dpc->Type = 0x13;
    Dpc->Number = 0x0;
    Dpc->Importance = 0x1;
    Dpc->DeferredContext = DeferredContext;
}

/* --------------------------------------*/ 
/*
  ASSEMBLY KeInitializeApc
  .text:0040E1F7                 mov     edi, edi
  .text:0040E1F9                 push    ebp
  .text:0040E1FA                 mov     ebp, esp
  .text:0040E1FC                 mov     eax, [ebp+arg_0] 
  .text:0040E1FF                 mov     edx, [ebp+arg_8]
  .text:0040E202                 cmp     edx, 2 
  .text:0040E205                 mov     ecx, [ebp+arg_4]
  .text:0040E208                 mov     word ptr [eax], 12h ; Apc->Type = 0x12
  .text:0040E20D                 mov     word ptr [eax+2], 30h; Apc->Size = 0x30
  .text:0040E213                 jz      loc_40DDB4 ; ????
  .text:0040E219
  .text:0040E219 loc_40E219:
  .text:0040E219                 mov     [eax+8], ecx ; Apc->Thread = Thread 
  .text:0040E21C                 mov     ecx, [ebp+arg_C] 
  .text:0040E21F                 mov     [eax+14h], ecx ; Apc->KernelRoutine = KernelRoutine
  .text:0040E222                 mov     ecx, [ebp+arg_10]
  .text:0040E225                 mov     [eax+2Ch], dl   ; Apc->ApcStateIndex = Environment or ???
  .text:0040E228                 mov     [eax+18h], ecx  ; Apc->RundownRoutine = RundownRoutine
  .text:0040E22B                 mov     ecx, [ebp+arg_14] ; NormalRoutine 
  .text:0040E22E                 xor     edx, edx ; edx = NULL
  .text:0040E230                 cmp     ecx, edx ; ??
  .text:0040E232                 mov     [eax+1Ch], ecx ; Apc->NormalRoutine
  .text:0040E235                 jnz     loc_40E3D5  ; ???? if NormalRoutine != NULL jmp 
  .text:0040E23B                 mov     [eax+2Dh], dl ; Apc->ApcMode = NULL
  .text:0040E23E                 mov     [eax+20h], edx ; Apc->NormalContext = NULL
  .text:0040E241
  .text:0040E241 loc_40E241:
  .text:0040E241                 mov     [eax+2Eh], dl ; Inserted = 0
  .text:0040E244                 pop     ebp
  .text:0040E245                 retn    20h
 */
/*
  lkd> dt nt!_KAPC
   +0x000 Type             : Int2B
   +0x002 Size             : Int2B
   +0x004 Spare0           : Uint4B
   +0x008 Thread           : Ptr32 _KTHREAD
   +0x00c ApcListEntry     : _LIST_ENTRY
   +0x014 KernelRoutine    : Ptr32     void
   +0x018 RundownRoutine   : Ptr32     void
   +0x01c NormalRoutine    : Ptr32     void
   +0x020 NormalContext    : Ptr32 Void
   +0x024 SystemArgument1  : Ptr32 Void
   +0x028 SystemArgument2  : Ptr32 Void
   +0x02c ApcStateIndex    : Char
   +0x02d ApcMode          : Char
   +0x02e Inserted         : UChar
*/

VOID
KeInitializeApc(
  IN  PRKAPC Apc,
  IN  PRKTHREAD Thread,
  IN  KAPC_ENVIRONMENT Environment,
  IN  PKKERNEL_ROUTINE KernelRoutine,
  IN  PKRUNDOWN_ROUTINE RundownRoutine OPTIONAL,
  IN  PKNORMAL_ROUTINE NormalRoutine OPTIONAL,
  IN  KPROCESSOR_MODE ApcMode OPTIONAL,
  IN  PVOID NormalContext OPTIONAL
  ) {
    Apc->Type = 0x12; 
    Apc->Size = 0x30; 
    // 
    if (Environment == 0x2) { 
      // loc_40DDB4 ????
      Apc->ApcStateIndex = Thread->ApcStateIndex;
    } else {
      Apc->ApcStateIndex = (CHAR)Environment;
    }
    Apc->Thread = Thread;
    Apc->KernelRoutine = KernelRoutine; 
    Apc->RundownRoutine = RundownRoutine;
    Apc->NormalRoutine = NormalRoutine;
    if (NormalRoutine) {
      // loc_40E3D5
      // ????
      Apc->Inserted = NULL;  // not sure
    } else {
      Apc->ApcMode = NULL;
      Apc->NormalContext = NULL;
    }
  }

/* ObjFastDereferenceObject:
  PAGE:00494E6D                 mov     edi, edi
  PAGE:00494E6F                 push    ebp
  PAGE:00494E70                 mov     ebp, esp
  PAGE:00494E72                 sub     esp, 0Ch   ; 12 bytes local vars 
  PAGE:00494E75                 push    ebx        
  PAGE:00494E76                 push    esi
  PAGE:00494E77                 push    edi
  PAGE:00494E78                 mov     edi, ecx        ; save FastRef into edi
  PAGE:00494E7A                 mov     ebx, edx        ; save Object into ebx
  PAGE:00494E7C                 mov     [ebp+local_var2], edi ; save edi (FastRef pointer) into the 2nd stack var
  PAGE:00494E7F
  PAGE:00494E7F loc_494E7F:                             
  PAGE:00494E7F                 mov     esi, [edi]      ; take the first word of FastRef (Object/Kref) and put it into esi
  PAGE:00494E81                 mov     eax, esi        ; save previously saved Fastref (Object/Kref) word into eax
  PAGE:00494E83                 xor     eax, ebx        ; xor previously saved FastRef (Object/Kref) with Object
  PAGE:00494E85                 cmp     eax, 7          ; compare the xoring with 7
  PAGE:00494E88                 mov     [ebp+local_var3], esi ; save Fastref (Object/Kref) into local_var3
  PAGE:00494E8B                 jnb     short loc_494EA8 ; if kref >= 7 or if Fastref->Object != Object
  PAGE:00494E8B                                         ; jump to loc_494EA8
  PAGE:00494E8D                 lea     eax, [esi+1]    ; Fasteref->Kref++ goes in eax
  PAGE:00494E90                 mov     [ebp+local_var1], eax ; save eax (Fastref->Kref+1) into local_var1
  PAGE:00494E93                 mov     eax, [ebp+local_var3] ; save local_var3 (Fastref->Kref) into eax
  PAGE:00494E96                 mov     ecx, [ebp+local_var2] ; save local_var2 (FastRef PTR) into ecx
  PAGE:00494E99                 mov     edx, [ebp+local_var1] ; save local_var1 (Fastref->Kfref+1 into) edx
  PAGE:00494E9C                 cmpxchg [ecx], edx      ; if Fastref->Kref == eax move edx in [ecx] else move edx in eax
  PAGE:00494E9F                 cmp     eax, esi        ; check if eax was changed by the previous instruction
  PAGE:00494EA1                 jnz     short loc_494E7F ; if the cmpxchg operation does not succedeed jump to loc_49E7F 
  PAGE:00494EA3                                          ; else exit
  PAGE:00494EA3 loc_494EA3:
  PAGE:00494EA3                 pop     edi
  PAGE:00494EA4                 pop     esi
  PAGE:00494EA5                 pop     ebx
  PAGE:00494EA6                 leave
  PAGE:00494EA7                 retn
  PAGE:00494EA8 ; ---------------------------------------------------------------------------
  PAGE:00494EA8
  PAGE:00494EA8 loc_494EA8:
  PAGE:00494EA8                 mov     ecx, ebx
  PAGE:00494EAA                 call    @ObfDereferenceObject@4 ; ObfDereferenceObject(x)
  PAGE:00494EAF                 jmp     short loc_494EA3
  PAGE:00494EAF @ObFastDereferenceObject@8 endp
*/
// 
typedef struct _EX_FAST_REF {
  union {
    PVOID Object;
    ULONG_PTR RefCnt : 3;
    ULONG_PTR Value;
  }
} EX_FAST_REF, *PEX_FAST_REF;

// __fastcall: func name decoration prefixed with @ and ending with @numberOfBytesParams 
// arguments through ecx and edx. 
VOID ObFastDereferenceObject(PEX_FAST_REF fast_ref, PVOID object) {
  PEX_FAST_REF local2 = fast_ref;  
  PVOID eax, esi;
  do {
    if ((*fast_ref ^ object) >= 7) {  // check if fast_ref->Object != object || fast_ref->RefCnt >= 7
      ObjDereferenceObject(object);   // arg2 must be a PTR to Obj 
      return; 
    }
    // This part seems to be dealing with interleavings that change *local2
    // PVOID local3 = *local2; // can be simplified 
    // local2, or fast_ref, seems to be a struct PTR
    PVOID local1 = *local2 + 1; 
    eax = *local2; 
    esi = *local2;
    // PVOID ecx = local2;
    if (eax == *local2) {  // if reference is same as the just dereferenced value, atomic 
      *local2 = local1;    // if same then *local2++
    } else {               // not atomic, *local2 has been changed 
      eax = *local2;       // then update 
    }
  } while (eax != esi); // if eax did not update then break, eg. increment is atomic 
}

/*
  .text:00429C2A ; Attributes: bp-based frame
  .text:00429C2A
  .text:00429C2A                 public KeInitializeQueue
  .text:00429C2A KeInitializeQueue proc near
  .text:00429C2A
  .text:00429C2A
  .text:00429C2A arg_0           = dword ptr  8
  .text:00429C2A arg_4           = dword ptr  0Ch
  .text:00429C2A
  1 .text:00429C2A                 mov     edi, edi
  2 .text:00429C2C                 push    ebp
  3 .text:00429C2D                 mov     ebp, esp
  4 .text:00429C2F                 mov     eax, [ebp+arg_0]
  5 .text:00429C32                 and     dword ptr [eax+4], 0
  6 .text:00429C36                 mov     byte ptr [eax], 4
  7 .text:00429C39                 mov     byte ptr [eax+2], 0Ah
  8 .text:00429C3D                 lea     ecx, [eax+8]   ; queue + 8 is a PTR
  9 .text:00429C40                 mov     [ecx+4], ecx   ; pointer content to pointer itself 
  10 .text:00429C43                 mov     [ecx], ecx 
  11 .text:00429C45                 lea     ecx, [eax+10h] ; queue + 16 is a PTR, EntryListHead
  12 .text:00429C48                 mov     [ecx+4], ecx
  13 .text:00429C4B                 mov     [ecx], ecx
  14 .text:00429C4D                 lea     ecx, [eax+20h] ; queue + 32 is a PTR, 
  15 .text:00429C50                 mov     [ecx+4], ecx
  16 .text:00429C53                 mov     [ecx], ecx
  17 .text:00429C55                 mov     ecx, [ebp+arg_4]
  18 .text:00429C58                 and     dword ptr [eax+18h], 0  ; eax+24
  19 .text:00429C5C                 test    ecx, ecx
  20 .text:00429C5E                 jz      short loc_429C67 ; if Count == 0, ecx = KeNumberProcessors 
  21 .text:00429C60
  22 .text:00429C60 loc_429C60:
  23 .text:00429C60                 mov     [eax+1Ch], ecx  ; eax+28
  24 .text:00429C63                 pop     ebp
  25 .text:00429C64                 retn    8
  26 .text:00429C67 ; --------------------------------------------------------------------------
  27 .text:00429C67                 movsx   ecx, ds:KeNumberProcessors
  28 .text:00429C6E                 jmp     short loc_429C60
*/
typedef struct _DISPATCHER_HEADER
{
     union
     {
          struct
          {
               UCHAR Type;  // 2 bytes 
               union 
               {
                    UCHAR Abandoned;
                    UCHAR Absolute;
                    UCHAR NpxIrql;
                    UCHAR Signalling;
               }; // 2 bytes
               union
               {
                    UCHAR Size;
                    UCHAR Hand;
               }; // 2 bytes 
               union
               {
                    UCHAR Inserted;
                    UCHAR DebugActive;
                    UCHAR DpcActive;
               }; // 2 bytes 
          };
          LONG Lock; // 4 bytes 
     }; // 8 bytes 
     LONG SignalState;         // 4 bytes 
     LIST_ENTRY WaitListHead;  // 4 bytes 
} DISPATCHER_HEADER, *PDISPATCHER_HEADER; // 16 bytes 

typedef struct _KQUEUE {  // 36 bytes ?????
    DISPATCHER_HEADER Header;  // 16 bytes, 0 - 16
    LIST_ENTRY EntryListHead;  // 8 bytes, 16 - 24
    ULONG CurrentCount;        // 4 bytes, 24 - 28
    ULONG MaximumCount;        // 4 bytes, 28 - 32
    LIST_ENTRY ThreadListHead; // 4 bytes, 32 - 36 
} KQUEUE, *PKQUEUE, *RESTRICTED_POINTER PRKQUEUE;

VOID KeInitializeQueue(PRKQUEUE queue, ULONG Count) {
  queue->Header.SignalState = NULL;   // +4
  queue->Header.Type = (char)0x4;  // +0
  // queue->Header.f2 = (char)0x10; // +2  ?????
  queue->Header.WaitListHead->Blink = queue->Header.WaitListHead;
  queue->Header.WaitListHead->Flink = queue->Header.WaitListHead; 
  queue->EntryListHead->Blink = queue->EntryListHead; 
  queue->EntryListHead->Flink = queue->EntryListHead; 
  queue->ThreadListHead->Blink = queue->ThreadListHead; 
  queue->ThreadListHead->Flink = queue->ThreadListHead;

  if (Count == 0) {
    queue->MaximumCount = KeNumberProcessors; 
  } else {
    queue->MaximumCount = Count; 
  }
}

// Couldn't find assembly of KxWaitForLockChainValid

typedef struct _KSPIN_LOCK_QUEUE {
    struct _KSPIN_LOCK_QUEUE * volatile Next;
    PKSPIN_LOCK volatile Lock;
} KSPIN_LOCK_QUEUE, *PKSPIN_LOCK_QUEUE;


PKSPIN_LOCK_QUEUE
KxWaitForLockChainValid (
			 __inout PKSPIN_LOCK_QUEUE LockQueue
			 )
{
  PKSPIN_LOCK_QUEUE AQueue;
  
  do {
    SomeYieldingStuff();
  } while ((AQueue = LockQueue->Next) == NULL); 
}

/*
  Disassembly of KiInitializeTSS (it should start the TSS segment for the current processor)
  mov     edi, edi
  push    ebp
  mov     ebp, esp
  mov     eax, [ebp+arg_0] 
  and     word ptr [eax+64h], 0
  and     word ptr [eax+60h], 0
  mov     word ptr [eax+66h], 20ACh
  mov     word ptr [eax+8], 10h
  pop     ebp
  retn    4
*/
/*
  KTSS Struct
  +0x000 Backlink         : Uint2B
  +0x002 Reserved0        : Uint2B
  +0x004 Esp0             : Uint4B
  +0x008 Ss0              : Uint2B
  +0x00a Reserved1        : Uint2B
  +0x00c NotUsed1         : [4] Uint4B
  +0x01c CR3              : Uint4B
  +0x020 Eip              : Uint4B
  +0x024 EFlags           : Uint4B
  +0x028 Eax              : Uint4B
  +0x02c Ecx              : Uint4B
  +0x030 Edx              : Uint4B
  +0x034 Ebx              : Uint4B
  +0x038 Esp              : Uint4B
  +0x03c Ebp              : Uint4B
  +0x040 Esi              : Uint4B
  +0x044 Edi              : Uint4B
  +0x048 Es               : Uint2B
  +0x04a Reserved2        : Uint2B
  +0x04c Cs               : Uint2B
  +0x04e Reserved3        : Uint2B
  +0x050 Ss               : Uint2B
  +0x052 Reserved4        : Uint2B
  +0x054 Ds               : Uint2B
  +0x056 Reserved5        : Uint2B
  +0x058 Fs               : Uint2B
  +0x05a Reserved6        : Uint2B
  +0x05c Gs               : Uint2B
  +0x05e Reserved7        : Uint2B
  +0x060 LDT              : Uint2B
  +0x062 Reserved8        : Uint2B
  +0x064 Flags            : Uint2B
  +0x066 IoMapBase        : Uint2B
  +0x068 IoMaps           : [1] _KiIoAccessMap
  +0x208c IntDirectionMap  : [32] UChar
  
 */
VOID kiInitializeTSS(PKTSS ktss) {
  ktss->Flags = NULL;
  ktss->LDT = NULL; 
  ktss->IoMapBase = 0x20ac; 
  ktss->Ss0 = 0x10;
}

/*
  RtlValidateUnicodeString disassembly
  
  mov     edi, edi
  push    ebp
  mov     ebp, esp
  cmp     [ebp+arg_0], 0
  jz      short loc_467E21
  mov     eax, 0C000000Dh  ; 
  jmp     short loc_467E6D
  
  loc_467E21:                          
  mov     ecx, [ebp+arg_4]
  test    ecx, ecx        
  push    esi
  push    edi
  jz      short loc_467E69 ; if arg_4 is 0, return 0 
  mov     di, [ecx] 
  movzx   eax, di         ; *arg2
  push    2
  cdq                     ; expand eax to edx 
  pop     esi
  idiv    esi             
  test    edx, edx        
  jnz     short loc_467E62   ; if *arg2 mod 2 is not 0, return STATUS_INVALID_PARAMETER 
  mov     si, [ecx+2]
  push    ebx
  movzx   eax, si         ; *(arg2 + 2)
  push    2
  cdq                     
  pop     ebx
  idiv    ebx             
  pop     ebx
  test    edx, edx        
  jnz     short loc_467E62  ; if *(arg2 + 2) mod 2 is not 0, return STATUS_INVALID_PARAMETER
  cmp     di, si          
  ja      short loc_467E62  ; if *arg2 > *(arg2 + 2), return STATUS_INVALID_PARAMETER
  test    di, di          
  jnz     short loc_467E5C  ; if *arg2 != 0, goto 
  test    si, si          
  jz      short loc_467E69  ; if *(arg2 + 2) == 0, return 0
  
  loc_467E5C:              
  cmp     dword ptr [ecx+4], 0 
  jnz     short loc_467E69   ; if *(arg2 + 4) != 0, return 0
  
  loc_467E62:    // return STATUS_INVALID_PARAMETER 
  
  mov     eax, 0C000000Dh
  jmp     short loc_467E6B ; Jump
    
  loc_467E69: // return 0
  xor     eax, eax
  
  loc_467E6B:                             
  pop     esi
  
  loc_467E6D:                             
  pop     ebp
  retn    8               
*/

/* UNICODE_STRING
  +0x000 Length           : Uint2B
  +0x002 MaximumLength    : Uint2B
  +0x004 Buffer           : Ptr32 Uint2B
*/
typedef struct _UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

int RtlValidateUnicodeString(ULONG Flags, const UNICODE_STRING* string) {
  if (Flags != 0) {
    return 0xC000000D;  // STATUS_INVALID_PARAMETER
  }
  if (string == 0) return 0; 
  if (string->Length % 2 != 0) return STATUS_INVALID_PARAMETER;
  if (string->MaximumLength % 2 != 0) return STATUS_INVALID_PARAMETER; 
  if (string->Length > string->MaximumLength) return STATUS_INVALID_PARAMETER; 
  if (string->Length != 0) {
    if (string->Buffer != 0) return 0; 
    else return STATUS_INVALID_PARAMETER;
  }
  if (string->MaximumLength == 0) return 0;
  return STATUS_INVALID_PARAMETER;
}