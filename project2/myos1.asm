;����Դ���루myos1.asm��
org  7c00h		; BIOS���������������ص�0:7C00h��������ʼִ��
OffSetOfUserPrg1 equ 8100h
Start:
	mov	ax, cs	       ; �������μĴ���ֵ��CS��ͬ
	mov	ds, ax	       ; ���ݶ�
	mov	bp, Message		 ; BP=��ǰ����ƫ�Ƶ�ַ
	mov	ax, ds		 ; ES:BP = ����ַ
	mov	es, ax		 ; ��ES=DS
	mov	cx, MessageLength  ; CX = ������=9��
	mov	ax, 1301h		 ; AH = 13h�����ܺţ���AL = 01h��������ڴ�β��
	mov	bx, 0007h		 ; ҳ��Ϊ0(BH = 0) �ڵװ���(BL = 07h)
      mov dh, 0		       ; �к�=0
	mov	dl, 0			 ; �к�=0
	int	10h			 ; BIOS��10h���ܣ���ʾһ���ַ�
LoadnEx:
     ;�����̻�Ӳ���ϵ����������������ڴ��ES:BX����
      mov ax,cs                ;�ε�ַ ; ������ݵ��ڴ����ַ
      mov es,ax                ;���öε�ַ������ֱ��mov es,�ε�ַ��
      mov bx, OffSetOfUserPrg1  ;ƫ�Ƶ�ַ; ������ݵ��ڴ�ƫ�Ƶ�ַ
      mov ah,2                 ; ���ܺ�
      mov al,1                 ;������
      mov dl,0                 ;�������� ; ����Ϊ0��Ӳ�̺�U��Ϊ80H
      mov dh,0                 ;��ͷ�� ; ��ʼ���Ϊ0
      mov ch,0                 ;����� ; ��ʼ���Ϊ0
      mov cl,2                 ;��ʼ������ ; ��ʼ���Ϊ1
      int 13H ;                ���ö�����BIOS��13h����
      ; �û�����a.com�Ѽ��ص�ָ���ڴ�������
      jmp OffSetOfUserPrg1
AfterRun:
      jmp $                      ;����ѭ��
Message:
      db 'Hello, MyOs is loading user program A.COM��'
MessageLength  equ ($-Message)
      times 510-($-$$) db 0
      db 0x55,0xaa

org 8100h 
    mov ax, cs  
    mov ds, ax  
; load_os:  
;     mov ah,02h                            ;����������  
;     mov al,06h                            ;��ȡ1������  
;     mov ch,00h                            ;��ʼ�ŵ�  
;     mov cl,02h                            ;��ʼ����  
;     mov dh,00h                            ;��ͷ��  
;     mov dl,00h                            ;��������  
;     mov bx,os                             ;�洢������  
;     int 13h  
;     ret  

                             ;����������ʶ,�����ļ���СΪ512byte  
  
    Dn_Rt equ 1                  ;D-Down,U-Up,R-right,L-Left
    Up_Rt equ 2                  ;
    Up_Lt equ 3                  ;
    Dn_Lt equ 4                  ;       

    delay equ 5000
    ddelay equ 200

    
    x dw 5
    y dw 0
    dcount dw delay
    ddcount dw ddelay
    color db 5Fh 
    count db 0 
    ten db 10
    rdul db Dn_Rt
    
start:
    mov ax,cs
    mov ds,ax         ; DS = CS       
    mov ax,0B800h       ; �ı������Դ���ʼ��ַ
    mov es,ax         ; ES = B800h


loop1: 
  mov ah,01h
  int 16h
  jz loop2
  mov ah,00H
  int 16h
  cmp al, 'c'
  jnz loop2

  call clearprint

  loop2: 
    dec word [dcount]                ; �ݼ���������
        jnz loop1         ; >0����ת;

    mov word[dcount],delay
    dec word[ddcount]
        jnz loop1
    mov word[dcount],delay
    mov word[ddcount],ddelay

    mov al,1
    cmp al,byte[rdul]
        jz DnRt
    mov al,2
    cmp al,byte[rdul]
        jz UpRt
    mov al,3
    cmp al,byte[rdul]
        jz UpLt    
    mov al,4
    cmp al,byte[rdul]
        jz DnLt
        jmp $

DnRt:
  inc word [x]
  inc word [y]
  mov bx,word [x]
  mov ax,25
  sub ax,bx
        jz  dr2ur
  mov bx,word [y]
  mov ax,80
  sub ax,bx
        jz  dr2dl
  jmp show
  
dr2ur:
    mov word [x],23
    mov byte[rdul],Up_Rt  
    jmp show
dr2dl:         
    mov word [y],78
    mov byte[rdul],Dn_Lt  
    jmp show

UpRt:
  dec word [x]
  inc word [y]
  mov bx,word [y]
  mov ax,80
  sub ax,bx
        jz ur2ul
  mov bx,word [x]
  mov ax,0
  sub ax,bx
        jz ur2dr
  jmp show
ur2ul:
    mov word [y],78
    mov byte[rdul],Up_Lt  
    jmp show
ur2dr:
    mov word [x],1
    mov byte[rdul],Dn_Rt  
    jmp show

  
  
UpLt:
  dec word [x]
  dec word [y]
  mov bx,word [x]
  mov ax,0
  sub ax,bx
        jz ul2dl
  mov bx,word [y]
  mov ax,-1
  sub ax,bx
        jz ul2ur
  jmp show

ul2dl:
    mov word [x],1
    mov byte[rdul],Dn_Lt  
    jmp show
ul2ur:
    mov word [y],1
    mov byte[rdul],Up_Rt  
    jmp show

  
  
DnLt:
  inc word [x]
  dec word [y]
  mov bx,word [y]
  mov ax,-1
  sub ax,bx
        jz dl2dr
  mov bx,word [x]
  mov ax,25
  sub ax,bx
        jz dl2ul
  jmp show

dl2dr:
    mov word [y],1
    mov byte[rdul],Dn_Rt  
    jmp show
  
dl2ul:        
    mov word [x],23
    mov byte[rdul],Up_Lt  
    jmp show



  
show:
      call clearprint  
      call printnames
      inc byte [color]
      and byte [color],0Fh
      or byte [color],50h
      xor ax,ax                 ; �����Դ��ַ
      mov ax,word [x]
      mov bx, 80
      mul bx
      add ax,word [y]
      mov bx,2
      mul bx
      mov bx,ax
      mov ah,byte [color]        ;  0000���ڵס�1111�������֣�Ĭ��ֵΪ07h��
      mov al,'O'      ;  al = ��ʾ�ַ�ֵ��Ĭ��ֵΪ20h=�ո����
      mov [es:bx],ax      ;  ��ʾ�ַ���ASCII��ֵ

     
      jmp loop1

  
clearprint: 
         push bx
         push cx
         push es
         mov bx,0b800h  
         mov es,bx
         mov bx,0
         mov cx,2000
    clear_print_real: 
           mov word [es:bx],5520h
           add bx,2
           loop clear_print_real
         pop es
         pop cx
         pop bx
         ret

printnames: 
            push ax
            push bx
            push cx
            push dx
            push es
            push bp
 
            mov al,1
            mov bh,0
            mov bl,5FH
            mov cx,50
            mov dx,0005h
            push cs
            pop es
            mov bp,me
            mov ah,13h
            int 10h
          

            pop bp
            pop es
            pop dx
            pop cx
            pop bx
            pop ax
             
            ret
    me db "WANG XIHUAI 16337236 Press 'c' to clean the screen"


times 512-($-$$) db 0                     ;�����510byte  