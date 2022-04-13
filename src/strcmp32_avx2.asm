	;------------------------------------------------
; Goryainov SI
; 09/04/2022
;------------------------------------------------

global strcmp32_avx2

section .text

;------------------------------------------------
; strcmp designed specially for 32-byte-long
; strings using avx2
;
; Entry: ymm0 - first  string
;		 ymm1 - second string
; Exit:  eax  - compraison result
; Note:  -
; Destr: -
;------------------------------------------------
strcmp32_avx2:
		vpcmpeqb ymm2, ymm0, ymm1
		vpmovmskb eax, ymm2
		inc eax
		ret
