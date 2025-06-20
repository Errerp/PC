org 100h
;    mov dx, 0xfffeh    
;    mov al, 80h
;    out dx, al

M:  
    mov bl, 40h
    mov bh, 80h
    mov cx, 1  
    ;mov dx, 0xfffbh
    mov dx,0x0fff9
    in al, dx         
    cmp al, 1
    jz turnOn 
    mov cx, 0
    test al, 1
    jz turnOff
    jmp M
       
turnOn:
    mov dx, 0xfffah
    ;mov dx, 0x0fff8 
    mov al, ah
    out dx, al
    cmp ah, 0x0ff
    je M
    cmp ah, 0x0aa
    jb chet
    jmp nechet
    
turnOff:
    mov dx, 0xfffah
    ;mov dx, 0x0fff8 
    mov al, ah
    out dx, al
    cmp ah, 0
    je M
    cmp ah, 0x055
    jbe nechet
    jmp chet

chet:     
    xor ah, bh
    shr bh, 2
    ;call delay
    cmp cx, 1       
    je turnOn
    jne turnOff 
    
nechet:     
    xor ah, bl
    shr bl, 2
    ;call delay2       
    cmp cx, 1       
    je turnOn
    jne turnOff 
        
proc delay 
    push cx
    mov cx, 100
cyc1:
    mov dx, cx
    ;mov cx, 0x10h
    mov cx, 0x0ffff
cyc2:
    loop cyc2
    mov cx, dx
    loop cyc1
    pop cx
    xor ch, ch
    ret
    delay endp 
     
proc delay2 
    push cx
    mov cx, 200
cyc3:
    mov dx, cx
    ;mov cx, 0x05h
    mov cx, 0x0ffff
cyc4:
    loop cyc4
    mov cx, dx 
    loop cyc3
    pop cx
    xor ch, ch
    ret
    delay2 endp