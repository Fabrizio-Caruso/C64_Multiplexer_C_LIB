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
   .IFDEF MUSIC_SWITCH
        .EXPORT _MUSIC_ON
   .ENDIF
   .IFDEF MULTICOLOR                    ; If MULTICOLOR flag is set, then
        .EXPORT _SPRM                   ; export _SPRM SPRites Multicolor array of flags
   .ENDIF
   .IFDEF EXPANDX                       ; If EXPANDX flag is set, then
        .EXPORT _SPREX                  ; export _SPRM SPRites EXPANDX array of flags
   .ENDIF
   .IFDEF EXPANDY                       ; If EXPANDY flag is set, then
        .EXPORT _SPREY                  ; export _SPRM SPRites EXPANDY array of flags
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
IRQ1LINE = $FC                          ; Sorting code IRQ at rasterline $0FC
IRQ2LINE = $00                          ; Music play IRQ at rasterline $000
IRQ3LINE = $23                          ; Sprites display IRQ at rasterline $023.
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

.IFDEF MUSIC_SWITCH
    _MUSIC_ON = $FA
.ENDIF
_MULTIPLEX_DONE = $FB                   ; "Job done" flag.
_NUMSPRITES = $FC                       ; Number of sprites that the main program wants to pass to the sprite sorter
_SPRUPDATEFLAG = $FD                    ; Main program must write a nonzero value here when it wants new sprites to be displayed
TEMPVARIABLE = $FE                      ; Just a temp variable used by the raster interrupt
SPRIRQCOUNTER = $FF                     ; Sprite counter used by the interrupt
SORTORDER = $40                         ; Order-table for sorting. Needs as many bytes
SORTORDERLAST = SORTORDER+MAXSPR-$01    ; as there are sprites.
;---------------------------------------
.IF .DEFINED(__C64__) 
    FULL_STANDARD_KERNAL=$EA31
    LIGHT_STANDARD_KERNAL=$EA81
.ELSEIF .DEFINED(__C128__)
    FULL_STANDARD_KERNAL=$FA65
    LIGHT_STANDARD_KERNAL=$FF33
.ENDIF   

TOP_KERNAL_IRQ=LIGHT_STANDARD_KERNAL
.IFDEF STANDARD_IRQ
    BOTTOM_KERNAL_IRQ=FULL_STANDARD_KERNAL
.ELSE
    BOTTOM_KERNAL_IRQ=LIGHT_STANDARD_KERNAL        
.ENDIF
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
    LDA #<IRQ1                          ; Load low byte of Setup IRQ vector
    .IFDEF USE_KERNAL
        STA IRQVec                      ; Store it into vector used if Kernal is ON,
    .ELSE
        STA $FFFE                       ; otherwise store it into vector used if Kernal is OFF
    .ENDIF
    LDA #>IRQ1                          ; Load hi byte of Setup IRQ vector
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
    LDA #IRQ1LINE
    STA VIC_HLINE                       ; Set position where first IRQ happens.
    LDA CIA1_ICR                        ; Acknowledge IRQ (to be sure)
    .IF .DEFINED(MUSIC_CODE)            ; Music init code
        LDA #$00                        ; Load Accumulator with sub tune value to initialize
        TAX                             ; then move it to X
        TAY                             ; and move it to Y
        JSR MUSIC_INIT                  ; Call MUSIC_INIT subroutine
    .ENDIF
    CLI                                 ; Let IRQs happen.
    RTS                                 ; Back to where he came from.
;---------------------------------------
; Routine to init the
; sprite multiplexing system
;-------------------
_INITSPRITES:
    LDX #$01                            ; Reset...
    STX _SPRUPDATEFLAG                  ; three...
    STX _NUMSPRITES
    INX                                 ; internal ...
    STX _MULTIPLEX_DONE                 ; flags.
    LDX #MAXSPR-$01                     ; Init the order table with a
IS_ORDERLIST:
    TXA                                 ; 0, 1, 2, 3, 4, 5... order
    STA SORTORDER,X
    DEX
    BPL IS_ORDERLIST
    RTS
;---------------------------------------
; Raster interrupt 1.
; This is where sorting happens.
;-------------------
IRQ1:
    .IFNDEF USE_KERNAL
        STA STORE_A                     ; Fast way to store/restore
        STX STORE_X                     ; CPU regs after an IRQ
        STY STORE_Y                     ; for kernal OFF only
    .ENDIF
    LDA #$FF                            ; Move all sprites
    STA VIC_SPR0_Y                      ; to the bottom to prevent
    STA VIC_SPR1_Y                      ; weird effects when sprite
    STA VIC_SPR2_Y                      ; moves lower than what it
    STA VIC_SPR3_Y                      ; previously was
    STA VIC_SPR4_Y
    STA VIC_SPR5_Y
    STA VIC_SPR6_Y
    STA VIC_SPR7_Y
    LDA _SPRUPDATEFLAG                  ; New sprites to be sorted?
    BEQ IRQ1_NONEWSPRITES
    LDA #$00
    STA _SPRUPDATEFLAG
    LDA _NUMSPRITES                     ; Take number of sprites given by the main program
    BNE IRQ1_BEGINSORT                  ; If it's zero, don't need to sort
IRQ1_NONEWSPRITES:
    LDX _NUMSPRITES
    CPX #$09
    BCC IRQ1_NOTMORETHAN8
    LDX #$08
IRQ1_NOTMORETHAN8:
    LDA D015TBL,X                       ; Now put the right value to
    STA VIC_SPR_ENA                     ; VIC_SPR_ENA, based on number of
    BEQ IRQ1_NOSPRITESATALL             ; sprites. Now init the sprite-counter
    LDA #$00                            ; for the actual sprite display
    STA SPRIRQCOUNTER                   ; routine
    
    .IF .DEFINED(MUSIC_CODE)            ; Choose the next IRQ based on the MUSIC_CODE flag value:
        .IF .DEFINED(MUSIC_SWITCH)
            LDA _MUSIC_ON
            BEQ _NOMUSICBRN0
        .ENDIF
        LDA #<IRQ2                      ; if music is ON then prepare vectors for IRQ2 code,    
        JMP _SETIRQ0
_NOMUSICBRN0:
        LDA #<IRQ3                      ; otherwise go directly to IRQ3 (Sprites display code)
    .ELSE
        LDA #<IRQ3                      ; otherwise go directly to IRQ3 (Sprites display code)
    .ENDIF
_SETIRQ0:
    .IFDEF USE_KERNAL
        STA IRQVec                      ; Store low byte into vector used if Kernal is ON,
    .ELSE
        STA $FFFE                       ; otherwise store it into vector used if Kernal is OFF
    .ENDIF
    
    .IF .DEFINED(MUSIC_CODE)            ; Choose the next IRQ based on the MUSIC_CODE flag value:
        .IF .DEFINED(MUSIC_SWITCH)
            LDA _MUSIC_ON
            BEQ _NOMUSICBRN1
        .ENDIF
        LDA #>IRQ2                      ; if music is ON then prepare vectors for IRQ2 code,
        JMP _SETIRQ1
_NOMUSICBRN1:
        LDA #>IRQ3                      ; otherwise go directly to IRQ3 (Sprites display code)
    .ELSE
        LDA #>IRQ3                      ; otherwise go directly to IRQ3 (Sprites display code)
    .ENDIF
_SETIRQ1:
    .IFDEF USE_KERNAL
        STA IRQVec+$0001                ; Store hi byte into vector used if Kernal is ON,
    .ELSE
        STA $FFFF                       ; otherwise store it into vector used if Kernal is OFF
    .ENDIF
    
    .IF .DEFINED(MUSIC_CODE)            ; If MUSIC_CODE is defined
        .IF .DEFINED(MUSIC_SWITCH)
            LDA _MUSIC_ON
            BEQ _NOMUSICBRN2
        .ENDIF
        LDA #IRQ2LINE                   ; then load raster line where start play routine
        JMP _SETIRQPOS
_NOMUSICBRN2:
        LDA #IRQ3LINE                   ; otherwise load display routine raster position
    .ELSE
        LDA #IRQ3LINE                   ; otherwise load display routine raster position
    .ENDIF
_SETIRQPOS:
    STA VIC_HLINE                       ; set new IRQ starting position
IRQ1_NOSPRITESATALL:
    JMP EXIT_IRQ
;-------------------
IRQ1_BEGINSORT:
    .IFDEF DEBUG 
        INC VIC_BORDERCOLOR             ; Show rastertime usage for debug.
    .ENDIF
    .IFDEF FAST_MODE 
        LDA #$01                        ; Enable "2 mhz mode under the border" if this code is
        STA VIC_CLK_128                 ; executed on C128. On other machines this store is useless.
    .ENDIF    
    LDX #MAXSPR                         ; We needo to sort
    DEX                                 ; this sprite?
    CPX _NUMSPRITES
    BCC IRQ1_CLEARDONE                  ; Yes -> go sorting.
    LDA #$FF                            ; No -> mark unused sprites with the
IRQ1_CLEARLOOP:
    STA SPRY,X                          ; lowest Y-coordinate ($ff)
    DEX                                 ; these will "fall" to the
    CPX _NUMSPRITES                   ; bottom of the sorted table
    BCS IRQ1_CLEARLOOP
IRQ1_CLEARDONE:
    LDX #$00
IRQ1_SORTLOOP:
    LDY SORTORDER+$0001,X               ; Sorting code. Algorithm
    LDA SPRY,Y                          ; ripped from Dragon Breed :-)
    LDY SORTORDER,X
    CMP SPRY,Y
    BCS IRQ1_SORTSKIP
    STX IRQ1_SORTRELOAD+$0001
IRQ1_SORTSWAP:
    LDA SORTORDER+$0001,X
    STA SORTORDER,X
    STY SORTORDER+$0001,X
    CPX #$00
    BEQ IRQ1_SORTRELOAD
    DEX
    LDY SORTORDER+$0001,X
    LDA SPRY,Y
    LDY SORTORDER,X
    CMP SPRY,Y
    BCC IRQ1_SORTSWAP
IRQ1_SORTRELOAD:
    LDX #$00
IRQ1_SORTSKIP:
    INX
    CPX #MAXSPR-$01
    BCC IRQ1_SORTLOOP
    LDX _NUMSPRITES
    LDA #$FF                            ; $ff is the endmark for the
    STA SORTSPRY,X                      ; sprite interrupt routine
    LDX #$00
IRQ1_SORTLOOP3:
    LDY SORTORDER,X                     ; Final loop:
    LDA SPRY,Y                          ; Now copy sprite variables to
    STA SORTSPRY,X                      ; the sorted table
    LDA SPRX,Y
    STA SORTSPRX,X
    LDA SPRF,Y
    STA SORTSPRF,X
    LDA SPRC,Y
    STA SORTSPRC,X
    .IFDEF MULTICOLOR                   ; If sprites MULTICOLOR flag is on then
        LDA SPRM,Y                      ; sort sprites Multicolor flags array
        STA SORTSPRM,X
    .ENDIF
    .IFDEF EXPANDX                      ; If sprites expand_x flag is on then
        LDA SPREX,Y                     ; sort sprite expand_x flags array
        STA SORTSPREX,X
    .ENDIF
    .IFDEF EXPANDY                      ; If sprites expand_y flag is on then
        LDA SPREY,Y                     ; sort sprite expand_y flags array
        STA SORTSPREY,X
    .ENDIF    
    INX
    CPX _NUMSPRITES
    BCC IRQ1_SORTLOOP3
    .IFDEF DEBUG
        DEC VIC_BORDERCOLOR             ; Show rastertime usage for debug.
    .ENDIF
;    .IFDEF FAST_MODE
;        LDA #$00                        ; Disable "2 mhz mode under the border" if this code is
;        STA VIC_CLK_128                 ; executed on C128. On other machines this store is useless.
;    .ENDIF    
    INC _MULTIPLEX_DONE                 ; Set _MULTIPLEX_DONE flag to 1 to keep MAIN code in sync with multiplexer.
    JMP IRQ1_NONEWSPRITES
;---------------------------------------
; Raster Interrupt 2
; Music Play Irq
;-------------------
    .IF .DEFINED(MUSIC_CODE)
    IRQ2:
        .IFNDEF USE_KERNAL
            STA STORE_A                 ; Fast way to store/restore
            STX STORE_X                 ; CPU regs after an IRQ
            STY STORE_Y                 ; for kernal OFF only
        .ENDIF
        
        .IFDEF DEBUG
            INC VIC_BORDERCOLOR         ; Change border color for debug
        .ENDIF
        JSR MUSIC_PLAY                  ; Jump to MUSIC_PLAY subroutine
        .IFDEF DEBUG
            DEC VIC_BORDERCOLOR         ; Change border color for debug
        .ENDIF            
    ;-------------------
        LDA #<IRQ3                      ; Load low byte of sprites display IRQ vector
        .IFDEF USE_KERNAL
            STA IRQVec                  ; Store it into vector used if Kernal is ON,
        .ELSE
            STA $FFFE                   ; otherwise store it into vector used if Kernal is OFF
        .ENDIF
        LDA #>IRQ3                      ; Load hi byte of sprites display IRQ vector
        .IFDEF USE_KERNAL
            STA IRQVec+$0001            ; Store it into vector used if Kernal is ON
        .ELSE
            STA $FFFF                   ; otherwise store it into vector used if Kernal is OFF
        .ENDIF
        LDA #IRQ3LINE                   ; Load position where display IRQ happens,
        STA VIC_HLINE                   ; and set it.
        JMP EXIT_IRQ                    ; Exit from IRQ.
    .ENDIF
;---------------------------------------
; Raster interrupt 3.
; This is where sprite displaying happens
;-------------------
IRQ3:
    .IFNDEF USE_KERNAL
        STA STORE_A                     ; Fast way to store/restore
        STX STORE_X                     ; CPU regs after an IRQ
        STY STORE_Y                     ; for kernal OFF only
    .ENDIF
IRQ3_DIRECT:
    LDY SPRIRQCOUNTER                   ; Take next sorted sprite number
    LDA SORTSPRY,Y                      ; Take Y-coord of first new sprite
    CLC
    ADC #$18                            ; $18 lines down from there is
    BCC IRQ3_NOTOVER                    ; the endpoint for this IRQ
    LDA #$FF                            ; Endpoint can't be more than $ff
IRQ3_NOTOVER:
    STA TEMPVARIABLE
    .IFDEF FAST_MODE
        LDA #$00                        ; Disable "2 mhz mode under the border" if this code is
        STA VIC_CLK_128                 ; executed on C128. On other machines this store is useless.
    .ENDIF        
IRQ3_SPRITELOOP:
    LDA SORTSPRY,Y
    CMP TEMPVARIABLE                    ; End of this IRQ?
    BCS IRQ3_ENDSPR
    LDX PHYSICALSPRTBL2,Y               ; Physical sprite number x 2
    STA VIC_SPR0_Y,X                    ; for X & Y coordinate
    LDA SORTSPRX,Y                      ; Load sorted sprite X coordinate
    ASL                                 ; multiply by 2
    STA VIC_SPR0_X,X                    ; store into sprite X coord register
    BCC IRQ3_LOWMSB                     ; if < 255 clear sprite MSB in VIC_SPR_HI_X
    LDA VIC_SPR_HI_X                    ; Load actual VIC_SPR_HI_X value
    ORA ORTBL,X                         ; Do a logic OR operation to set actual sprite bit ON
    JMP IRQ3_MSBOK
IRQ3_LOWMSB:
    LDA VIC_SPR_HI_X                    ; Load actual VIC_SPR_HI_X value
    AND ANDTBL,X                        ; Do a logic AND operation to set actual sprite bit OFF
IRQ3_MSBOK:
    STA VIC_SPR_HI_X                    ; and store new value back to the VIC_SPR_HI_X register.    
    .IFDEF MULTICOLOR                   ; If multicolor mode flag is ON
        LDA SORTSPRM,Y                  ; check if actual sprite is MULTI or SINGLE color mode
        BEQ IRQ3_NO_MULTI               ; No -> clear sprite related bit into VIC_SPR_MCOLOR
        LDA VIC_SPR_MCOLOR              ; Load actual VIC_SPR_MCOLOR status
        ORA ORTBL,X                     ; Do a logic OR operation to set actual sprite bit ON
        STA VIC_SPR_MCOLOR              ; and store new value back to the VIC_SPR_MCOLOR register.
        JMP IRQ3_MULTI
    IRQ3_NO_MULTI:
        LDA VIC_SPR_MCOLOR              ; Load actual VIC_SPR_MCOLOR status
        AND ANDTBL,X                    ; Do a logic AND operation to set actual sprite bit OFF
        STA VIC_SPR_MCOLOR              ; and store new value back to the VIC_SPR_MCOLOR register.
    IRQ3_MULTI:
    .ENDIF
    .IFDEF EXPANDX                      ; If EXPANDX mode flag is ON
        LDA SORTSPREX,Y                 ; check if actual sprite is x-expanded 
        BEQ IRQ3_NO_EXPANDX             ; No -> clear sprite related bit into VIC_SPR_EXP_X
        LDA VIC_SPR_EXP_X               ; Load actual VIC_SPR_EXP_X status
        ORA ORTBL,X                     ; Do a logic OR operation to set actual sprite bit ON
        STA VIC_SPR_EXP_X               ; and store new value back to the VIC_SPR_EXP_X register.
        JMP IRQ3_EXPANDX
    IRQ3_NO_EXPANDX:
        LDA VIC_SPR_EXP_X               ; Load actual VIC_SPR_EXP_X status
        AND ANDTBL,X                    ; Do a logic AND operation to set actual sprite bit OFF
        STA VIC_SPR_EXP_X               ; and store new value back to the VIC_SPR_EXP_X register.
    IRQ3_EXPANDX:
    .ENDIF
    .IFDEF EXPANDY                      ; If EXPANDY mode flag is ON
        LDA SORTSPREY,Y                 ; check if actual sprite is y-expanded
        BEQ IRQ3_NO_EXPANDY             ; No -> clear sprite related bit into VIC_SPR_EXP_Y
        LDA VIC_SPR_EXP_Y               ; Load actual VIC_SPR_EXP_Y status
        ORA ORTBL,X                     ; Do a logic OR operation to set actual sprite bit ON
        STA VIC_SPR_EXP_Y               ; and store new value back to the VIC_SPR_EXP_Y register.
        JMP IRQ3_EXPANDY
    IRQ3_NO_EXPANDY:
        LDA VIC_SPR_EXP_Y               ; Load actual VIC_SPR_EXP_Y status
        AND ANDTBL,X                    ; Do a logic AND operation to set actual sprite bit OFF
        STA VIC_SPR_EXP_Y               ; and store new value back to the VIC_SPR_EXP_Y register.
    IRQ3_EXPANDY:
    .ENDIF  
    LDX PHYSICALSPRTBL1,Y               ; Physical sprite number x 1
    LDA SORTSPRF,Y
    STA SCREEN_RAM+$03F8,X              ; Set sprite frame pointer to actual sprite register
    LDA SORTSPRC,Y
    STA VIC_SPR0_COLOR,X                ; Set sprite color to actual sprite color register
    INY
    BNE IRQ3_SPRITELOOP
IRQ3_ENDSPR:
    CMP #$FF                            ; Was it the endmark?
    BEQ IRQ3_LASTSPR
    STY SPRIRQCOUNTER
    SEC                                 ; That coordinate - $07 is the
    SBC #$07                            ; position for next interrupt
    CMP VIC_HLINE                       ; Already late from that?

    .IF (IRQ3_DIRECT-*)<-128
        BCS NO_IRQ3_DIRECT
        JMP IRQ3_DIRECT
    NO_IRQ3_DIRECT:
    .ELSE
        BCC IRQ3_DIRECT                 ; Then go directly to next IRQ directly
    .ENDIF    
    
    STA VIC_HLINE                       ; otherwise set new IRQ position
    JMP EXIT_IRQ                        ; and normally exit this IRQ.
IRQ3_LASTSPR:
    LDA #<IRQ1                          ; If we just displayed last sprite, load low byte of sort IRQ vector
    .IFDEF USE_KERNAL
        STA IRQVec                      ; Store it into vector used if Kernal is ON,
    .ELSE
        STA $FFFE                       ; otherwise store it into vector used if Kernal is OFF
    .ENDIF
    LDA #>IRQ1                          ; Load hi byte of sort IRQ vector
    .IFDEF USE_KERNAL
        STA IRQVec+$0001                ; Store it into vector used if Kernal is ON
    .ELSE
        STA $FFFF                       ; otherwise store it into vector used if Kernal is OFF
    .ENDIF
    LDA #IRQ1LINE                       ; Load position where sort IRQ happens,
    STA VIC_HLINE                       ; and set it.
;-------------------
EXIT_IRQ:                               ; Exit IRQ code.
    LSR VIC_IRR                         ; Acknowledge raster IRQ
    .IF .NOT .DEFINED(USE_KERNAL)
        STORE_A = *+$0001           ; Restore original registers value
        LDA #$00
        STORE_X = *+$0001           ; at the original values they have before
        LDX #$00
        STORE_Y = *+$0001           ; IRQ call
        LDY #$00  
    .ELSE 
        JMP LIGHT_STANDARD_KERNAL       ; Use normal Kernal C128 IRQ exit code if Kernal is ON 
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
    .REPEAT (MAXSPR/$08)+$01
    .BYTE $00, $01, $02, $03, $04, $05, $06, $07
    .ENDREP
PHYSICALSPRTBL2:
    .REPEAT (MAXSPR/$08)+$01
    .BYTE $00, $02, $04, $06, $08, $0A, $0C, $0E
    .ENDREP
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
