;--------------------------------------
; C64 sprites multiplexer
; Based on 32 sprites multiplexer
; by Lasse Oorni (Cadaver)
;-------------------
    .IF INCFILE=1                       ; Include file for multi platform compilation
    .INCLUDE "c64.inc"
    .ELSEIF INCFILE=2
    .INCLUDE "c128.inc"
    .ELSEIF INCFILE=3
    .INCLUDE "cbm510.inc"
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
   .IFDEF MULTICOLOR
   .EXPORT _SPRM
   .ENDIF
;-------------------
;DEBUG = $00                             ; Set to != $00 to show rastertime usage.
;USE_KERNAL = $01                        ; Set to != $00 to enable normal kernal usage
;MULTICOLOR = $01                        ; Set to != $00 to enable multicolor sprites flag
;-------------------
SCREEN_RAM = $0400                      ; Screen ram start address
;-------------------
IRQ1LINE = $FC                          ; This is the place on screen where the sorting IRQ happens
IRQ2LINE = $2A                          ; This is where sprite displaying begins...
;MAXSPR = 16                             ; Maximum number of sprites
;-------------------
_MULTIPLEX_DONE = $FA                   ; "Job done" flag.
_NUMSPRITES = $FB                       ; Number of sprites that the main program wants to pass to the sprite sorter
_SPRUPDATEFLAG = $FC                    ; Main program must write a nonzero value here when it wants new sprites to be displayed
SORTEDSPRITES = $FD                     ; Number of sorted sprites for the raster interrupt
TEMPVARIABLE = $FE                      ; Just a temp variable used by the raster interrupt
SPRIRQCOUNTER = $FF                     ; Sprite counter used by the interrupt
SORTORDER = $50                         ; Order-table for sorting. Needs as many bytes
SORTORDERLAST = SORTORDER+MAXSPR-$01    ; as there are sprites.
;---------------------------------------
; Routine to init the
; raster interrupt system
;-------------------
_INITRASTER:
    SEI
    .IF INCFILE=1                       ; C64 RAM setup
    .IFDEF USE_KERNAL
    LDA #$36                            ; Switch kernal ON
    .ELSE
    LDA #$35                            ; Switch kernal OFF
    .ENDIF
    STA $01
    .ENDIF
;-------------------
    LDA #<IRQ1                          ; Setup IRQ vector
    .IFDEF USE_KERNAL
    STA IRQVec                          ; Kernal ON vector
    .ELSE
    STA $FFFE                           ; Kernal OFF vector
    .ENDIF
    LDA #>IRQ1
    .IFDEF USE_KERNAL
    STA IRQVec+$0001                    ; Kernal ON vector
    .ELSE
    STA $FFFF                           ; Kernal OFF vector
    .ENDIF
    LDA #<IRQ_RTI                       ; Avoid problems if user
    STA NMIVec                          ; press RESTORE key during
;    STA $FFFA                           ; program execution...
    LDA #>IRQ_RTI                       ; ... which mean:
    STA NMIVec+$0001                    ; disable "RESTORE"
;    STA $FFFB                           ; key.
    LDA #$7F
    STA CIA1_ICR                        ; CIA interrupt off
    LDA #$01
    STA VIC_IMR                         ; Raster interrupt on
    LDA #$1B                            ; High bit of interrupt position = 0
    STA VIC_CTRL1
    LDA #IRQ1LINE                       ; Line where next IRQ happens
    STA VIC_HLINE
    LDA CIA1_ICR                        ; Acknowledge IRQ (to be sure)
    CLI
    RTS
;---------------------------------------
; Routine to init the
; sprite multiplexing system
;-------------------
_INITSPRITES:
    LDX #$00
    STX SORTEDSPRITES                   ; Reset...
    STX _SPRUPDATEFLAG                  ; three...
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
    STA STORE_A                         ; Fast way to store/restore
    STX STORE_X                         ; CPU regs after an IRQ
    STY STORE_Y                         ; for kernal OFF only
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
    STA SORTEDSPRITES                   ; If it's zero, don't need to
    BNE IRQ1_BEGINSORT                  ; sort
IRQ1_NONEWSPRITES:
    LDX SORTEDSPRITES
    CPX #$09
    BCC IRQ1_NOTMORETHAN8
    LDX #$08
IRQ1_NOTMORETHAN8:
    LDA D015TBL,X                       ; Now put the right value to
    STA VIC_SPR_ENA                     ; VIC_SPR_ENA, based on number of
    BEQ IRQ1_NOSPRITESATALL             ; sprites. Now init the sprite-counter
    LDA #$00                            ; for the actual sprite display
    STA SPRIRQCOUNTER                   ; routine
    LDA #<IRQ2                          ; Set up the sprite display
    .IFDEF USE_KERNAL                   ; preparing vector for next IRQ (IRQ2).
    STA IRQVec                          ; Vector for kernal ON
    .ELSE
    STA $FFFE                           ; Vector for kernal OFF
    .ENDIF
    LDA #>IRQ2
    .IFDEF USE_KERNAL
    STA IRQVec+$0001
    .ELSE
    STA $FFFF
    .ENDIF
    JMP IRQ2_DIRECT                     ; Go directly; we might be late
IRQ1_NOSPRITESATALL:
    JMP EXIT_IRQ
;-------------------
IRQ1_BEGINSORT:
    .IFDEF DEBUG 
    INC VIC_BORDERCOLOR                 ; Show rastertime usage for debug.
    .ENDIF
    LDX #MAXSPR                         ; We needo to sort
    DEX                                 ; this sprite?
    CPX SORTEDSPRITES
    BCC IRQ1_CLEARDONE                  ; Yes -> go sorting.
    LDA #$FF                            ; No -> mark unused sprites with the
IRQ1_CLEARLOOP:
    STA SPRY,X                          ; lowest Y-coordinate ($ff)
    DEX                                 ; these will "fall" to the
    CPX SORTEDSPRITES                   ; bottom of the sorted table
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
    LDX SORTEDSPRITES
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
    .IFDEF MULTICOLOR
    LDA SPRM,Y
    STA SORTSPRM,X
    .ENDIF
    INX
    CPX SORTEDSPRITES
    BCC IRQ1_SORTLOOP3
    .IFDEF DEBUG
    DEC VIC_BORDERCOLOR                 ; Show rastertime usage for debug.
    .ENDIF
    INC _MULTIPLEX_DONE
    JMP IRQ1_NONEWSPRITES
;---------------------------------------
; Raster interrupt 2.
; This is where sprite displaying happens
;-------------------
IRQ2:
    .IFNDEF USE_KERNAL
    STA STORE_A                         ; Fast way to store/restore
    STX STORE_X                         ; CPU regs after an IRQ
    STY STORE_Y                         ; for kernal OFF only
    .ENDIF
IRQ2_DIRECT:
    LDY SPRIRQCOUNTER                   ; Take next sorted sprite number
    LDA SORTSPRY,Y                      ; Take Y-coord of first new sprite
    CLC
    ADC #$18                            ; 16 lines down from there is
    BCC IRQ2_NOTOVER                    ; the endpoint for this IRQ
    LDA #$FF                            ; Endpoint can't be more than $ff
IRQ2_NOTOVER:
    STA TEMPVARIABLE
IRQ2_SPRITELOOP:
    LDA SORTSPRY,Y
    CMP TEMPVARIABLE                    ; End of this IRQ?
    BCS IRQ2_ENDSPR
    LDX PHYSICALSPRTBL2,Y               ; Physical sprite number x 2
    STA VIC_SPR0_Y,X                         ; for X & Y coordinate
    LDA SORTSPRX,Y                      ; Load sorted sprite X coordinate
    ASL                                 ; multiply by 2
    STA VIC_SPR0_X,X                    ; store into sprite X coord register
    BCC IRQ2_LOWMSB                     ; if < 255 clear sprite MSB in VIC_SPR_HI_X
    LDA VIC_SPR_HI_X                    ; Otherwise set the MSB...
    ORA ORTBL,X                         ; ( set )
    STA VIC_SPR_HI_X
    JMP IRQ2_MSBOK
IRQ2_LOWMSB:
    LDA VIC_SPR_HI_X
    AND ANDTBL,X                        ; ( clear )
    STA VIC_SPR_HI_X
IRQ2_MSBOK:
    .IFDEF MULTICOLOR
    LDA SORTSPRM,Y                      ; Multicolor setup
    BEQ IRQ2_NO_MULTI
    LDA VIC_SPR_MCOLOR
    ORA ORTBL,X
    STA VIC_SPR_MCOLOR
    JMP IRQ2_MULTI
IRQ2_NO_MULTI:
    LDA VIC_SPR_MCOLOR
    AND ANDTBL,X
    STA VIC_SPR_MCOLOR
IRQ2_MULTI:
    .ENDIF
    LDX PHYSICALSPRTBL1,Y               ; Physical sprite number x 1
    LDA SORTSPRF,Y
    STA SCREEN_RAM+$03F8,X              ; frame
    LDA SORTSPRC,Y
    STA VIC_SPR0_COLOR,X                ; color
    INY
    BNE IRQ2_SPRITELOOP
IRQ2_ENDSPR:
    CMP #$FF                            ; Was it the endmark?
    BEQ IRQ2_LASTSPR
    STY SPRIRQCOUNTER
    SEC                                 ; That coordinate - $07 is the
    SBC #$07                            ; position for next interrupt
    CMP VIC_HLINE                           ; Already late from that?
    BCC IRQ2_DIRECT                     ; Then go directly to next IRQ
    STA VIC_HLINE
    JMP EXIT_IRQ
IRQ2_LASTSPR:
    LDA #<IRQ1                          ; Was the last sprite,
    .IFDEF USE_KERNAL                   ; go back to irq1 (sorting interrupt)
    STA IRQVec                          ; vector for kernal ON
    .ELSE
    STA $FFFE                           ; vector for kernal OFF
    .ENDIF
    LDA #>IRQ1
    .IFDEF USE_KERNAL
    STA IRQVec+$0001                    ; vector for kernal ON
    .ELSE
    STA $FFFF                           ; vector for kernal OFF
    .ENDIF
    LDA #IRQ1LINE
    STA VIC_HLINE
;-------------------
EXIT_IRQ:                               ; Exit IRQ code.
    LSR VIC_IRR                         ; Acknowledge raster IRQ
    .IF INCFILE=1
    .IFNDEF USE_KERNAL
STORE_A = *+$0001                       ; Restore original registers value
    LDA #$00
STORE_X = *+$0001                       ; at the original values they have before
    LDX #$00
STORE_Y = *+$0001                       ; IRQ call
    LDY #$00
    .ELSE
    JMP $EA81
    .ENDIF
    .ELSEIF INCFILE=2
    JMP $FA65
    .ENDIF
IRQ_RTI:
    RTI                                 ; ReTurn from Interrupt 
;---------------------------------------
_SPRX:
SPRX:
    .RES MAXSPR, $00                    ; Unsorted sprite table
_SPRY:
SPRY:
    .RES MAXSPR, $00
_SPRC:
SPRC:
    .RES MAXSPR, $00
_SPRF:
SPRF:
    .RES MAXSPR, $00
    .IFDEF MULTICOLOR
_SPRM:
SPRM:
    .RES MAXSPR, $00
    .ENDIF
;-------------------
SORTSPRX:
    .RES MAXSPR, $00                    ; Sorted sprite table
SORTSPRY:
    .RES MAXSPR+$01, $00                ; Must be one byte extra for the $ff endmark
SORTSPRC:
    .RES MAXSPR, $00
SORTSPRF:
    .RES MAXSPR, $00
    .IFDEF MULTICOLOR
SORTSPRM:
    .RES MAXSPR, $00
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
;-------------------
PHYSICALSPRTBL1:
    .BYTE $00, $01,$02, $03, $04, $05, $06, $07        ; Indexes to frame & color
    .BYTE $00, $01,$02, $03, $04, $05, $06, $07        ; registers
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
;-------------------
ANDTBL:
    .BYTE $FF-$01
ORTBL:
    .BYTE $01
    .BYTE $FF-$02
    .BYTE $02
    .BYTE $FF-$04
    .BYTE $04
    .BYTE $FF-$08
    .BYTE $08
    .BYTE $FF-$10
    .BYTE $10
    .BYTE $FF-$20
    .BYTE $20
    .BYTE $FF-$40
    .BYTE $40
    .BYTE $FF-$80
    .BYTE $80
;-------------------------------------------------------------------------------
