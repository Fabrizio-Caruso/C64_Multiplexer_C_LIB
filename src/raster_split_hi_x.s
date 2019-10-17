.IF .DEFINED(__C128__)
    .SEGMENT "CODE2"
.ENDIF
;--------------------------------------
; C64 sprites multiplexer
; Based on 32 sprites multiplexer
; by Lasse Oorni (Cadaver)
;-------------------
    .INCLUDE "zeropage.inc"
    .IF .DEFINED(__C64__)
        .INCLUDE "c64.inc"              ; Include file for C64 specific compilation
    .ELSEIF .DEFINED(__C128__)
        .INCLUDE "c128.inc"             ; Include file for C128 specific compilation
    .ENDIF 
;-------------------
   .EXPORTZP _SPRUPDATEFLAG             ; Export zeropage
   .EXPORTZP _NUMSPRITES
   .EXPORTZP _MULTIPLEX_DONE
;-------------------
   .EXPORT _INITSPRITES                 ; Export program variables
   .EXPORT _INITRASTER
   .EXPORT _SPRX
   .EXPORT _SPRY
   .EXPORT _SPRC
   .EXPORT _SPRF
   .IFDEF MULTICOLOR                    ; If MULTICOLOR flag is set, then
        .EXPORT _SPRM                   ; export _SPRM SPRites Multicolor array of flags
   .ENDIF
   .IFDEF EXPANDX                       ; If EXPANDX flag is set, then
        .EXPORT _SPREX                  ; export _SPRM SPRites EXPANDX array of flags
   .ENDIF
   .IFDEF EXPANDY                       ; If EXPANDY flag is set, then
        .EXPORT _SPREY                  ; export _SPRM SPRites EXPANDY array of flags
   .ENDIF  
   .IF .DEFINED(__C64__)
       .IFDEF STANDARD_IRQ
            KERNAL_IRQ=$EA31
       .ELSE
            KERNAL_IRQ=$EA81
       .ENDIF
   .ENDIF
   .IF .DEFINED(__C128__)
       .IFDEF STANDARD_IRQ
            KERNAL_IRQ=$FA65
       .ELSE
            KERNAL_IRQ=$FF33
       .ENDIF
   .ENDIF  
;-------------------
;DEBUG = $00                             ; Set to != $00 to show rastertime usage.
;USE_KERNAL = $01                        ; Set to != $00 to enable normal kernal usage
;MULTICOLOR = $01                        ; Set to != $00 to enable multicolor sprites flag
;EXPANDX= $01                            ; Set to != $00 to enable expand_x sprites flag
;EXPANDY= $01                            ; Set to != $00 to enable expand_y sprites flag
;-------------------
SCREEN_RAM = $0400                      ; Screen ram start address
;-------------------
IRQTOPLINE = $23                          ; Sprites display IRQ at rasterline $023.
IRQBOTTOMLINE = $90                          ; Sorting code IRQ at rasterline $0FC

;-------------------
;MUSIC_CODE = $01                       ; Set to $01 to enable music routines
.IF .DEFINED(__C64__)
    MUSIC_INIT = $1000                  ; Music init address for C64 specific compilation
    MUSIC_PLAY = $1003                  ; Music play address for C64 specific compilation
.ENDIF
.IF .DEFINED(__C128__)
    MUSIC_INIT = $2400                  ; Music init address for C128 specific compilation
    MUSIC_PLAY = $2403                  ; Music play address for C128 specific compilation
.ENDIF    
;MAXSPR = 16                            ; Maximum number of sprites
;-------------------
_MULTIPLEX_DONE = $FA                   ; "Job done" flag.
_NUMSPRITES = $FB                       ; Number of sprites that the main program wants to pass to the sprite sorter
_SPRUPDATEFLAG = $FC                    ; Main program must write a nonzero value here when it wants new sprites to be displayed
TEMPVARIABLE = $FE                      ; Just a temp variable used by the raster interrupt

;---------------------------------------
; Routine to init the
; raster interrupt system
;-------------------
_INITRASTER:
    LDA #$7F
    STA CIA1_ICR                        ; CIA interrupt off
    SEI
    .IFDEF __C64__                      ; C64 RAM setup
        .IFDEF USE_KERNAL
            LDA #$36                    ; Switch kernal ON
        .ELSE
            LDA #$35                    ; Switch kernal OFF
        .ENDIF
        STA $01
    .ENDIF
;-------------------
    LDA #<IRQTOP                          ; Load low byte of Setup IRQ vector
    .IFDEF USE_KERNAL
        STA IRQVec                      ; Store it into vector used if Kernal is ON,
    .ELSE
        STA $FFFE                       ; otherwise store it into vector used if Kernal is OFF
    .ENDIF
    LDA #>IRQTOP                          ; Load hi byte of Setup IRQ vector
    .IFDEF USE_KERNAL
        STA IRQVec+$0001                ; Store it into vector used if Kernal is ON,
    .ELSE
        STA $FFFF                       ; otherwise store it into vector used if Kernal is OFF
    .ENDIF
    LDA #<IRQ_RTI                       ; Load low byte of unarmed RTI (disable RESTORE key)
    .IFDEF USE_KERNAL
        STA NMIVec                      ; Store it into vector used if Kernal is ON,
    .ELSE
        STA $FFFA                       ; otherwise store it into vector used if Kernal is OFF
    .ENDIF
    LDA #>IRQ_RTI                       ; Load hi byte of unarmed RTI (disable RESTORE key)
    .IFDEF USE_KERNAL
        STA NMIVec+$0001                ; Store it into vector used if Kernal is ON,
    .ELSE
        STA $FFFB                       ; otherwise store it into vector used if Kernal is OFF
    .ENDIF
    LDA #$01
    STA VIC_IMR                         ; Enable raster IRQs.
    LDA #$1B
    STA VIC_CTRL1                       ; Set high bit of interrupt position = $0xx
    LDA #IRQTOPLINE
    STA VIC_HLINE                       ; Set position where first IRQ happens.
    LDA CIA1_ICR                        ; Acknowledge IRQ (to be sure)
    
    CLI                                 ; Let IRQs happen.
    RTS                                 ; Back to where he came from.
;---------------------------------------
; Routine to init the
; sprite multiplexing system
;-------------------
_INITSPRITES:
    LDX #$00
    STX _SPRUPDATEFLAG                  ; three...
    INX                                 ; internal ...
    STX _MULTIPLEX_DONE                 ; flags.
    LDA #$FF
    STA VIC_SPR_ENA
    RTS
;---------------------------------------
; Raster interrupt 1.
; This is where sorting happens.
;-------------------
IRQTOP:
    .IFNDEF USE_KERNAL
        STA STORE_A                     ; Fast way to store/restore
        STX STORE_X                     ; CPU regs after an IRQ
        STY STORE_Y                     ; for kernal OFF only
    .ENDIF

    LDA SPRY
    STA VIC_SPR0_Y                     ; to the bottom to prevent
    LDA SPRY+$01
    STA VIC_SPR1_Y                      ; weird effects when sprite
    LDA SPRY+$02
    STA VIC_SPR2_Y                      ; moves lower than what it
    LDA SPRY+$03
    STA VIC_SPR3_Y                      ; previously was
    LDA SPRY+$04
    STA VIC_SPR4_Y
    LDA SPRY+$05
    STA VIC_SPR5_Y
    LDA SPRY+$06
    STA VIC_SPR6_Y
    LDA SPRY+$07
    STA VIC_SPR7_Y   
    
    LDA SPRX
    ASL                                 ; multiply by 2
    STA VIC_SPR0_X                      ; weird effects when sprite    
    BCS _HIMSB_0                     ; if < 255 clear sprite MSB in VIC_SPR_HI_X
    LDA VIC_SPR_HI_X                    ; Load actual VIC_SPR_HI_X value
    AND ANDTBL                        ; Do a logic AND operation to set actual sprite bit OFF
    JMP _SET_SPR_HI_0
_HIMSB_0:
    LDA VIC_SPR_HI_X                    ; Load actual VIC_SPR_HI_X value
    ORA ORTBL                         ; Do a logic OR operation to set actual sprite bit ON
_SET_SPR_HI_0:
    STA VIC_SPR_HI_X                    ; and store new value back to the VIC_SPR_HI_X register.


    LDA SPRX+$01
    STA VIC_SPR1_X
    LDA SPRX+$02
    STA VIC_SPR2_X                      ; moves lower than what it
    LDA SPRX+$03
    STA VIC_SPR3_X                      ; previously was
    LDA SPRX+$04
    STA VIC_SPR4_X
    LDA SPRX+$05
    STA VIC_SPR5_X
    LDA SPRX+$06
    STA VIC_SPR6_X
    LDA SPRX+$07
    STA VIC_SPR7_X        

    LDX SPRF                         ; Physical sprite number x 1
    STX SCREEN_RAM+$03F8              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$01
    STX SCREEN_RAM+$03F9              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$02
    STX SCREEN_RAM+$03FA              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$03
    STX SCREEN_RAM+$03FB              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$04
    STX SCREEN_RAM+$03FC              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$05
    STX SCREEN_RAM+$03FD              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$06
    STX SCREEN_RAM+$03FE              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$07
    STX SCREEN_RAM+$03FF              ; Set sprite frame pointer to actual sprite register

    LDA SPRC
    STA VIC_SPR0_COLOR                ; Set sprite color to actual sprite color register
    LDA SPRC+$01    
    STA VIC_SPR1_COLOR
    LDA SPRC+$02
    STA VIC_SPR2_COLOR                ; Set sprite color to actual sprite color register
    LDA SPRC+$03    
    STA VIC_SPR3_COLOR
    LDA SPRC+$04
    STA VIC_SPR4_COLOR                ; Set sprite color to actual sprite color register
    LDA SPRC+$05
    STA VIC_SPR5_COLOR
    LDA SPRC+$06
    STA VIC_SPR6_COLOR                ; Set sprite color to actual sprite color register
    LDA SPRC+$07
    STA VIC_SPR7_COLOR 
    
    LDA #<IRQBOTTOM                          ; If we just displayed last sprite, load low byte of sort IRQ vector
    .IFDEF USE_KERNAL
        STA IRQVec                      ; Store it into vector used if Kernal is ON,
    .ELSE
        STA $FFFE                       ; otherwise store it into vector used if Kernal is OFF
    .ENDIF
    LDA #>IRQBOTTOM                          ; Load hi byte of sort IRQ vector
    .IFDEF USE_KERNAL
        STA IRQVec+$0001                ; Store it into vector used if Kernal is ON
    .ELSE
        STA $FFFF                       ; otherwise store it into vector used if Kernal is OFF
    .ENDIF
    LDA #IRQBOTTOMLINE                       ; Load position where sort IRQ happens,
    STA VIC_HLINE                       ; and set it.    
    
    JMP EXIT_IRQ


;---------------------------------------
; Raster interrupt 3.
; This is where sprite displaying happens
;-------------------
IRQBOTTOM:
    .IFNDEF USE_KERNAL
        STA STORE_A                     ; Fast way to store/restore
        STX STORE_X                     ; CPU regs after an IRQ
        STY STORE_Y                     ; for kernal OFF only
    .ENDIF
    

    LDA SPRY+$08
    STA VIC_SPR0_Y                      ; weird effects when sprite
    LDA SPRY+$09
    STA VIC_SPR1_Y                      ; moves lower than what it
    LDA SPRY+$0A
    STA VIC_SPR2_Y                      ; previously was
    LDA SPRY+$0B
    STA VIC_SPR3_Y
    LDA SPRY+$0C
    STA VIC_SPR4_Y
    LDA SPRY+$0D
    STA VIC_SPR5_Y
    LDA SPRY+$0E
    STA VIC_SPR6_Y   
    LDA SPRY+$0F
    STA VIC_SPR7_Y                     ; to the bottom to prevent
    
    LDA SPRX+$08
    ASL                                 ; multiply by 2
    STA VIC_SPR0_X                      ; weird effects when sprite    
    BCS _HIMSB_9                     ; if < 255 clear sprite MSB in VIC_SPR_HI_X
    LDA VIC_SPR_HI_X                    ; Load actual VIC_SPR_HI_X value
    AND ANDTBL                        ; Do a logic AND operation to set actual sprite bit OFF
    JMP _SET_SPR_HI_8
_HIMSB_9:
    LDA VIC_SPR_HI_X                    ; Load actual VIC_SPR_HI_X value
    ORA ORTBL                         ; Do a logic OR operation to set actual sprite bit ON
_SET_SPR_HI_8:
    STA VIC_SPR_HI_X                    ; and store new value back to the VIC_SPR_HI_X register.


    LDA SPRX+$09
    STA VIC_SPR1_X                      ; weird effects when sprite
    LDA SPRX+$0A
    STA VIC_SPR2_X                      ; moves lower than what it
    LDA SPRX+$0B
    STA VIC_SPR3_X                      ; previously was
    LDA SPRX+$0C
    STA VIC_SPR4_X
    LDA SPRX+$0D
    STA VIC_SPR5_X
    LDA SPRX+$0E
    STA VIC_SPR6_X
    LDA SPRX+$0F
    STA VIC_SPR7_X    


    LDX SPRF+$08                         ; Physical sprite number x 1
    STX SCREEN_RAM+$03F8              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$09
    STX SCREEN_RAM+$03F9              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$0A
    STX SCREEN_RAM+$03FA              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$0B
    STX SCREEN_RAM+$03FB              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$0C
    STX SCREEN_RAM+$03FC              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$0D
    STX SCREEN_RAM+$03FD              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$0E
    STX SCREEN_RAM+$03FE              ; Set sprite frame pointer to actual sprite register
    LDX SPRF+$0F
    STX SCREEN_RAM+$03FF              ; Set sprite frame pointer to actual sprite register
    
    
    LDA SPRC+$08 
    STA VIC_SPR0_COLOR                ; Set sprite color to actual sprite color register
    LDA SPRC+$09    
    STA VIC_SPR1_COLOR
    LDA SPRC+$0A
    STA VIC_SPR2_COLOR                ; Set sprite color to actual sprite color register
    LDA SPRC+$0B    
    STA VIC_SPR3_COLOR
    LDA SPRC+$0C
    STA VIC_SPR4_COLOR                ; Set sprite color to actual sprite color register
    LDA SPRC+$0D
    STA VIC_SPR5_COLOR
    LDA SPRC+$0E
    STA VIC_SPR6_COLOR                ; Set sprite color to actual sprite color register
    LDA SPRC+$0F
    STA VIC_SPR7_COLOR        
    
    LDA #<IRQTOP                          ; If we just displayed last sprite, load low byte of sort IRQ vector
    .IFDEF USE_KERNAL
        STA IRQVec                      ; Store it into vector used if Kernal is ON,
    .ELSE
        STA $FFFE                       ; otherwise store it into vector used if Kernal is OFF
    .ENDIF
    LDA #>IRQTOP                          ; Load hi byte of sort IRQ vector
    .IFDEF USE_KERNAL
        STA IRQVec+$0001                ; Store it into vector used if Kernal is ON
    .ELSE
        STA $FFFF                       ; otherwise store it into vector used if Kernal is OFF
    .ENDIF
    LDA #IRQTOPLINE                       ; Load position where sort IRQ happens,
    STA VIC_HLINE                       ; and set it.

    JMP EXIT_IRQ
;-------------------
EXIT_IRQ:                               ; Exit IRQ code.
    LSR VIC_IRR                         ; Acknowledge raster IRQ
    .IF .DEFINED(__C64__)
        .IF .NOT .DEFINED(USE_KERNAL)
            STORE_A = *+$0001           ; Restore original registers value
            LDA #$00
            STORE_X = *+$0001           ; at the original values they have before
            LDX #$00
            STORE_Y = *+$0001           ; IRQ call
            LDY #$00         
        .ELSE ; JMP $EA31/$EA81
            JMP KERNAL_IRQ              ; Use normal Kernal C64 IRQ exit code if Kernal is ON 
        .ENDIF
    .ELSEIF .DEFINED(__C128__) 
            JMP KERNAL_IRQ                   ; Use normal Kernal C128 IRQ exit code if Kernal is ON 
    .ENDIF
IRQ_RTI:
    RTI                                 ; ReTurn from Interrupt 
;---------------------------------------
_SPRX:
SPRX:
    .RES MAXSPR, $00                    ; Unsorted sprites X coords array
_SPRY:
SPRY:
    .RES MAXSPR, $00                    ; Unsorted sprites Y coords array
_SPRC:
SPRC:
    .RES MAXSPR, $00                    ; Unsorted sprites colors array
_SPRF:
SPRF:
    .RES MAXSPR, $00                    ; Unsorted sprites frame pointer array
    .IFDEF MULTICOLOR
    _SPRM:
    SPRM:
        .RES MAXSPR, $00                ; Unsorted sprites multicolor flags array
    .ENDIF
    .IFDEF EXPANDX
    _SPREX:
    SPREX:
        .RES MAXSPR, $00                ; Unsorted sprites EXPANDX flags array
    .ENDIF
    .IFDEF EXPANDY
    _SPREY:
    SPREY:
        .RES MAXSPR, $00                ; Unsorted sprites EXPANDY flags array
    .ENDIF    
;-------------------
SORTSPRX:
    .RES MAXSPR, $00                    ; Sorted sprites X coords array
SORTSPRY:
    .RES MAXSPR+$01, $00                ; Sorted sprites Y coords array, one byte extra for the $FF endmark
SORTSPRC:
    .RES MAXSPR, $00                    ; Sorted sprites colors array
SORTSPRF:
    .RES MAXSPR, $00                    ; Sorted sprites frame pointer array
    .IFDEF MULTICOLOR
    SORTSPRM:
        .RES MAXSPR, $00                ; Sorted sprites multicolor flags array
    .ENDIF
    .IFDEF EXPANDX
    SORTSPREX:
        .RES MAXSPR, $00                ; Sorted sprites EXPANDX flags array
    .ENDIF
    .IFDEF EXPANDY
    SORTSPREY:
        .RES MAXSPR, $00                ; Sorted sprites EXPANDY flags array
    .ENDIF    
;-------------------
D015TBL:
    .BYTE %00000000                     ; Table of sprites that are "on"
    .BYTE %00000001                     ; for VIC_SPR_ENA
    .BYTE %00000011
    .BYTE %00000111
    .BYTE %00001111
    .BYTE %00011111
    .BYTE %00111111
    .BYTE %01111111
    .BYTE %11111111
;---------------------------------------
; Indexes to frame & color registers
;-------------------
PHYSICALSPRTBL1:
    .BYTE $00, $01,$02, $03, $04, $05, $06, $07
    .BYTE $00, $01,$02, $03, $04, $05, $06, $07
    .BYTE $00, $01,$02, $03, $04, $05, $06, $07
    .BYTE $00, $01,$02, $03, $04, $05, $06, $07
    .BYTE $00, $01,$02, $03, $04, $05, $06, $07
    .BYTE $00, $01,$02, $03, $04, $05, $06, $07
    .BYTE $00, $01,$02, $03, $04, $05, $06, $07
    .BYTE $00, $01,$02, $03, $04, $05, $06, $07
PHYSICALSPRTBL2:
    .BYTE $00, $02, $04, $06, $08, $0A, $0C, $0E
    .BYTE $00, $02, $04, $06, $08, $0A, $0C, $0E
    .BYTE $00, $02, $04, $06, $08, $0A, $0C, $0E
    .BYTE $00, $02, $04, $06, $08, $0A, $0C, $0E
    .BYTE $00, $02, $04, $06, $08, $0A, $0C, $0E
    .BYTE $00, $02, $04, $06, $08, $0A, $0C, $0E
    .BYTE $00, $02, $04, $06, $08, $0A, $0C, $0E
    .BYTE $00, $02, $04, $06, $08, $0A, $0C, $0E
;---------------------------------------
; AND/OR tables for various register
; fast setup.
;-------------------
ANDTBL:
    .BYTE %11111110
ORTBL:
    .BYTE %00000001
    .BYTE %11111101
    .BYTE %00000010
    .BYTE %11111011
    .BYTE %00000100
    .BYTE %11110111
    .BYTE %00001000
    .BYTE %11101111
    .BYTE %00010000
    .BYTE %11011111
    .BYTE %00100000
    .BYTE %10111111
    .BYTE %01000000
    .BYTE %01111111
    .BYTE %10000000
;-------------------------------------------------------------------------------
