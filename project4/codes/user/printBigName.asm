org 0e000h
printBigName:
    push ax
    push bx
    push cx
    push dx
    push es
    push bp
    
    push cs
    pop ds
    mov al,1
    mov bh,0
    mov bl,5FH
    mov cx,2000
    mov dx,0000h
    push cs
    pop es
    mov bp,named

    mov ah,13h
    int 10h
    

    mov ah, 00h
    int 16h    

    

    pop bp
    pop es
    pop dx
    pop cx
    pop bx
    pop ax

 
    retf 

named db "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"
db "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$=$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ $$$$$$$$$$$$"
db "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  ~$$$$!.$$$$$$  $$$$$$$$$$$$$$$ ,$$ $$$$$$$$$$$"
db "$$$$$$$$$$$$$$$$$$$$$$$ $$$$$$$$  $$$$$! $$$$$$ .$$$$$$$. $$$$$= !$$  $$$$$$$$$$"
db "$$$$$$                   $$$$$$$ $$$ $$* $$$$$$ ,$$$$$$$$  $$$$  $$$* $$$$$$$$$$"
db "$$$$$$$$$$$$$$  $$$$$$$$$$$$$$$       $* ****** ,$$$$$$$$* $=$$ $$$$$$$$$  $$$$$"
db "$$$$$$$$$$$$$$  $$$$$$$$$$$$$$$ $$$$$$$* $$$$$$ ,$$$$$$$$$$$ $~ $$$$ ~$$$$$$$$$$"
db "$$$$$$$$$$$$$$  $$$$$$$$$$$$$$: $$$$$$$! $$$$$$ .$$$$$ $$$$$=$  $$$$ ~$$$$$$$$$$"
db "$$$$$$$$$$$$$$  $$$$$$$$$$$$$$ $$$$  $$! ------ .$$$$$! *$$ $.~ $$$$ ~$$$$$$$$$$"
db "$$$$$$$$$$$$$$  $$$$$$$$$$$$$,=$; $$$$$; .!$$$$ ;$$$$$$  $$ $.$ $$$$ ~$$  $$$$$$"
db "$$$$$$$$$$$$$$  $$$$$$$$$$$$$!$$; $$$$$$$ -$$$$$$$$$$$$$ $,$~$$ $$$$ ~$$$$$$$$$$"
db "$$$$$$$=======  =====  $$$$$$$$$; $$$$$$  ------, -$$$$$$$ $$$$ $$$$ ~$$$$$$$$$$"
db "$$$$$$$$$$$$$$  $$$$$$$$$$$$$$$$; $, $$ .$: $$ $! $$$$$$$;~$$$$ $$$$ ~$$$$$$$$$$"
db "$$$$$$$$$$$$$$  $$$$$$$$$$$$$$$$; $$$$.*$$ =$. $~ $$$$$$$ $$$$$ $$$$ ~$$. $$$$$$"
db "$$$$$$$$$$$$$$  $$$$$$$$$$$$$$$$; $$$ $$$ -$$ $$. $$$$$$; $$$$$ ;;;; ,;;;;;$$$$$"
db "$$$$$$$$$$$$$$  $$$$$$$$$$$$$$$$; $$$$$$ ~$$ .$$  $$$:!; =$$$$$ $$$$ ~$$$$$$$$$$"
db "$$$$$$$$$$$$$$  $$$$$$$$$$$$$$$$; $$.$$ $$$  $$$  $$$$$  $$$$$$ $$$$ ~$$$$$$$$$$"
db "$$$$$$$$$$$$$$  $$$$$$$$$$$$$$$$; $ $ =$$$- $$$$ ~$$$$$! $$$$$$ $$$$ ~$$$$;$$$$$"
db "$$$$$$$$$$$$$$  $$$$$$$$-$$$$$$$;  $$$$$$. $$$$$ $$$$$$  $$$$$$ ---- .---  -$$$$"
db "$$$$;;;;;;;;;;  ;;;;;;;   $$$$$$  $$$$$$ =$$. .  $$$$$$  $$$$$$ $$$$$$$$$$$$$$$$"
db "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$=$$$$ =$$$$$=  $$$$$$$  $$$$$$ $$$$$$$$$$$$$$$$"
db "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"
db "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"
db "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"
db "                         press any key to exit                                  "

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; modules ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;  
delayModule:
    delay equ 20000
    ddelay equ 2000
    mov word[dcount],delay
    mov word[ddcount],ddelay
    delayModule_loop1:
        dec word [dcount]                ; 递减计数变量
            jnz delayModule_loop1         ; >0：跳转;

        mov word[dcount],delay
        dec word[ddcount]
            jnz delayModule_loop1
    ret
    dcount dw delay
    ddcount dw ddelay

