;------- Block 1 ------------
CODESG segment
assume CS:CODESG

;------ Block 2 -------------
MAIN proc
	xor AL,AL
	mov BH,10h
	mov CH,5
	mov CL,10h
	mov DH,10h
	mov DL,3Eh
	mov AH,6
	
	int 10h

	call WINDOW
	call WINDOW
	call WINDOW
	
	int 20h
MAIN endp


;-------- Block 3 -----------
WINDOW proc
	add BH,10h
	add CH,1
	add CL,1
	sub DH,1
	sub DL,1
	int 10h
	ret
WINDOW endp

; ------- Block 4 ------------
CODESG ends

end MAIN