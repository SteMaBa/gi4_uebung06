SECTION .data

wert DD 123456789       ; Parameter 
hexb DD 16              ; Basis fuer Hexadezimalzahlen
decb DD 10              ; Basis fuer Dezimalzahlen

struc Darstellung       ; Struct, Gesamtgroesse 20 bytes
       hex: resb 9      ; 9 Bytes
       dezimal: resb 11 ; 11 Bytes
endstruc

SECTION .text

global main
extern malloc


main:

    push ebp		
    mov ebp, esp

    mov edi, [wert]     ; Parameter in Register edi zur Übergabe kopieren
    jmp convert         ; in Funktion springen
    
    convert:
    mov eax, dword 20   ; Speicher mit Groesse des structs ...
    call malloc         ; ... allokieren        
    mov ebx, eax        ; Zeiger von malloc in ebx 
    mov ecx, dword 8    ; Zaehler fuer Hex-Konvertierung (2^32 = 16^8)
    mov eax, dword 0    ; eax zu 0 setzen
    mov edx, edi        ; Zu konvertierende Zahl in edx kopieren       
    jmp loophex         ; Hexadezimal-Darstellung berechnen
    mov ecx, dword 10   ; Zaehler fuer Dez-Konvertierung 
    mov eax, dword 0    ; eax zu 0 setzen
    mov edx, edi        ; Zu konvertierende Tahl in edx kopieren
    jmp loopdec         ; Dezimal-Darstellung berechnen
    mov eax, ebx        ; Zeiger auf das Struct in eax als Rueckgabewert
    jmp end             ; Zum Ende
    
    loophex: 
    cmp edx, dword 0    ; Zu teilende Zahl mit 0 vergleichen
    jne dodivisionhex   ; Falls die Zahl != 0, dann Division ausfuehren        
    cmp edx, dword [9]  ; Rest in edx, pruefe ob <= 9 oder > 9
    ja bigrest          ; Falls Rest > 9, springe in bigrest
    add edx, dword 48   ; Rest in edx, Offset der ASCII-Tabelle addieren (0x30 = 48 dez.)
    mov [ebx + ecx], dl ; Ziffer in Hex (edx) in Struct kopieren, Little Endian 
    mov edx, eax        ; Ergebnis der Division in edx kopieren
    mov eax, dword 0    ; eax zuruecksetzen 
    dec ecx             ; Zaehler verringern 
    jnb loophex         ; Solange Zaehler >= 0, wiederholen
    ret                 ; Zurueck zu convert   
    
    dodivisionhex:
    div dword [hexb]    ; Zahl (edx:eax) durch Hex-Basis teilen
    ret                 ; Zurueck zu loophex
    
    bigrest:
    add edx, dword 17   ; Offset der ASCII-Tabelle ab A addieren (0x41 = 65 dez. = 17 + 48)
    ret                 ; Zurueck zu loophex
             
             
    loopdec:
    cmp edx, dword 0    ; Zu teilende Zahl mit 0 vergleichen
    jne dodivisiondec   ; Falls die Zahl != 0, dann Division ausfuehren
    add edx, dword 48   ; Rest in edx, ASCII-Offset addieren
    mov [ebx + ecx + 9], dl; Dezimalstelle in Struct kopieren, Little Endian  
    mov edx, eax        ; Ergebnis der Division in edx kopieren
    mov eax, dword 0    ; eax zuruecksetzen
    dec ecx             ; Zaehler verringern
    jnb loopdec         ; Solange Zaehler >= 0, wiederholen
    ret                 ; Zurueck zu convert  
    
    
    dodivisiondec:
    div dword [decb]    ; Zahl (edx:eax) durch Dez-Basis teilen
    ret                 ; Zurueck zu loopdec
    
    
    
    
    end:
    
    mov esp, ebp       
    pop ebp
    mov ebx, 0
    mov eax, 1 
    int 0x80



