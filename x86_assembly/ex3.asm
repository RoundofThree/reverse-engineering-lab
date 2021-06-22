        section .text
        global _strlen,_strchr,_memcpy,_memset,_strcmp,_strset

        ;; size_t strlen(const char *s)
_strlen:
        ; no local variables 
        mov rcx, -1  ; rcx is not callee-save, so don't save it on stack 
        xor rax, rax 
        repne scasb  ; repeat until rcx == 0 or al == rdi, decrement rcx 
        not rcx 
        dec rcx
        mov rax, rcx 
        ret 

        ;; char *strchr(const char *s, int c), rdi = s, sil = c
_strchr:
        xor eax, eax
__strchr_loop:
        cmp BYTE [rdi + rax], 0
        jz __strchr_end
        cmp BYTE [rdi + rax], sil
        jz __strchr_good
        inc rax
        jmp __strchr_loop
__strchr_good:
        lea rax, [rdi + rax]
        ret
__strchr_end:
        xor eax, eax
        ret

;         ;; void *memcpy(void *dest, const void *src, size_t n)
_memcpy:
        mov rcx, rdx  ; size_t n 
        mov rdx, rdi  ; save dest
__memcpy_loop:
        lodsb ; mov *rsi to al
        stosb ; mov al to *rdi 
        loop __memcpy_loop  ; loop rcx times
__memcpy_end:
        mov rax, rdx 
        ret 

;         ;; void *memset(void *s, int c, size_t n)
_memset: 
        mov rcx, rdx 
        mov eax, esi 
        mov rsi, rdi 
        rep stosb ; mov eax to *rdi 
        mov rax, rsi 
        ret 
        
;         ;; int strcmp(const char *s1, const char *s2)
_strcmp:
__strcmp_start:
        test rdi, rdi 
        jz __strcmp_end ; if rdi == NULL
        test rsi, rsi 
        jz __strchr_end ; if rsi == NULL
        mov rdx, [rdi]
        mov rcx, [rsi] 
        cmp rdx, rcx
        inc rdi 
        inc rsi 
        je __strcmp_start
__strcmp_end:
        mov rdx, [rdi] 
        mov rcx, [rsi] 
        cmp rdx, rcx
        je __strcmp_true
__strcmp_false:
        mov rax, 0
        ret 
__strcmp_true:
        mov rax, 1 
        ret

;         ;; char *strset(const char *str, char c)
_strset: 
        mov rax, rdi 
__strset_loop:
        cmp BYTE [rdi], 0 
        jz __strset_end 
        mov BYTE [rdi], sil 
        inc rdi 
        jmp __strset_loop 
__strset_end: 
        ret 
        