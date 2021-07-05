/* Task: This has a loop searching for something. Recover the function prototype and infer the types based on 
the context. 
  sub_10BB2 SAMPLE_H
  
  10bb2:	8b 44 24 04          	mov    eax,DWORD PTR [esp+0x4]  ; move to eax the begin of a PE
  10bb6:	53                   	push   ebx
  10bb7:	56                   	push   esi
  10bb8:	8b 70 3c             	mov    esi,DWORD PTR [eax+0x3c] ; put into ESI the value of the offset to PE signature
  10bbb:	03 f0                	add    esi,eax                  ; adjust ESI to be the address of the starting _IMAGE_NT_HEADERS
  10bbd:	0f b7 46 14          	movzx  eax,WORD PTR [esi+0x14]  ; copy into EAX the value of _IMAGE_NT_HEADERS.FileHeader.SizeOfOptionalHeader
  10bc1:	33 db                	xor    ebx,ebx                  ; zeroes EBX
  10bc3:	66 39 5e 06          	cmp    WORD PTR [esi+0x6],bx    ; check if _IMAGE_FILE_HEADER.FileHeader.NumberOfSections is zero
  10bc7:	57                   	push   edi
  10bc8:	8d 7c 30 18          	lea    edi,[eax+esi*1+0x18]     ; eax = SizeOfOptionalHeader, esi = _IMAGE_NT_HEADERS
                                                                        ; so esi + eax + 0x18 (0x4 bytes of Signature + 0x14 bytes of _IMAGE_FILE_HEADER) = _IMAGE_SECTION_HEADER
  10bcc:	76 1d                	jbe    0x10beb                  ; if no sections, jump to 0x10beb and return 0
  10bce:	ff 74 24 14          	push   DWORD PTR [esp+0x14]     ; get the instruction pointer value from the _IMAGE_DOS_HEADER and push it
  10bd2:	57                   	push   edi                      ; push the section header
  10bd3:	ff 15 a4 69 01 00    	call   DWORD PTR ds:0x169a4     ; arguments to this function are call to "PVOID something(_IMAGE_SECTION_HEADER *ptr, _IMAGE_DOS_HEADER.e_ip)"
  10bd9:	85 c0                	test   eax,eax
  10bdb:	59                   	pop    ecx
  10bdc:	59                   	pop    ecx
  10bdd:	74 14                	je     0x10bf3                  ; if result was zero jump to 0x10bed and return the section (maybe modified ?)
  10bdf:	0f b7 46 06          	movzx  eax,WORD PTR [esi+0x6]   ; ...else put the number of sections into eax
  10be3:	83 c7 28             	add    edi,0x28                 ; edi = current section + sizeof(_IMAGE_SECTION_HEADER) = next section header
  10be6:	43                   	inc    ebx                      ; increment ebx
  10be7:	3b d8                	cmp    ebx,eax                  ; compare ebx with eax (iterate over sections)
  10be9:	72 e3                	jb     0x10bce
  10beb:	33 c0                	xor    eax,eax
  10bed:	5f                   	pop    edi                      ; restore registers
  10bee:	5e                   	pop    esi
  10bef:	5b                   	pop    ebx
  10bf0:	c2 08 00             	ret    0x8
  10bf3:	8b c7                	mov    eax,edi                   ; put the address of section header into eax
  10bf5:	eb f6                	jmp    0x10bed                   ; jump
*/

PVOID sub_10BB2(PVOID ptr)
{
  PVOID ptr1 = ptr;
  PVOID ptr2 = ptr1 + (int *)(ptr1 + 0x3c); /* offset where the PE _IMAGE_NT_HEADERS starts */
  PVOID ptr3;
  PVOID ret;

  *ptr1 = (int *)(ptr2 + 0x14); /* _IMAGE_NT_HEADERS.FileHeader.SizeOfOptionalHeader */

  if(*ptr1 == 0)
    return 0;

  ptr3 = *ptr1 + ptr2 + 0x18; /* Point to the first _IMAGE_SECTION_HEADER */

  while(*ptr1){
    ret = call_some_stuff(ptr3, (int *)(ptr + 0x14)); /* First argument is an _IMAGE_SECTION_HEADER, second arg
						   is _IMAGE_DOS_HEADER.e_ip (instruction pointer)" */
    if(ret == NULL)
      return ret;

    (*ptr1)--;

    ptr3 += 0x28; /* Advance pointer to next _IMAGE_SECTION_HEADER, each section is 0x28 bytes */
  }

  return 0;	   
}
