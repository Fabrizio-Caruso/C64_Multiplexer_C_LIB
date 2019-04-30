;--------------------------------------
; C64 sprites multiplexer
;-------------------
   .EXPORTZP _SPRUPDATEFLAG
   .EXPORTZP _NUMSPRITES
;-------------------
   .EXPORT _INITSPRITES
   .EXPORT _INITRASTER
   .EXPORT _SPRX
   .EXPORT _SPRY
   .EXPORT _SPRC
   .EXPORT _SPRF
   .EXPORT _SPRITE_GFX
;-------------------
DEBUB = $00                             ; Set to != $00 to show rastertime usage.
;-------------------
IRQ1LINE = $FC                          ; This is the place on screen where the sorting IRQ happens
IRQ2LINE = $2A                          ; This is where sprite displaying begins...
MAXSPR = 32                             ; Maximum number of sprites
;-------------------
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
    LDA #<IRQ1
    STA $0314
    LDA #>IRQ1
    STA $0315
    LDA #$7F                            ; CIA interrupt off
    STA $DC0D
    LDA #$01                            ; Raster interrupt on
    STA $D01A
    LDA #27                             ; High bit of interrupt position = 0
    STA $D011
    LDA #IRQ1LINE                       ; Line where next IRQ happens
    STA $D012
    LDA $DC0D                           ; Acknowledge IRQ (to be sure)
    CLI
    RTS
;---------------------------------------
; Routine to init the
; sprite multiplexing system
;-------------------
_INITSPRITES:
    LDA #$00
    STA SORTEDSPRITES
    STA _SPRUPDATEFLAG
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
    DEC $D019                           ; Acknowledge raster interrupt
    LDA #$FF                            ; Move all sprites
    STA $D001                           ; to the bottom to prevent
    STA $D003                           ; weird effects when sprite
    STA $D005                           ; moves lower than what it
    STA $D007                           ; previously was
    STA $D009
    STA $D00B
    STA $D00D
    STA $D00F
    LDA _SPRUPDATEFLAG                  ; New sprites to be sorted?
    BEQ IRQ1_NONEWSPRITES
    LDA #$00
    STA _SPRUPDATEFLAG
    LDA _NUMSPRITES                      ; Take number of sprites given by the main program
    STA SORTEDSPRITES                   ; If it's zero, don't need to
    BNE IRQ1_BEGINSORT                  ; sort
IRQ1_NONEWSPRITES:
    LDX SORTEDSPRITES
    CPX #$09
    BCC IRQ1_NOTMORETHAN8
    LDX #$08
IRQ1_NOTMORETHAN8:
    LDA D015TBL,X                       ; Now put the right value to
    STA $D015                           ; $d015, based on number of
    BEQ IRQ1_NOSPRITESATALL             ; sprites. Now init the sprite-counter
    LDA #$00                            ; for the actual sprite display
    STA SPRIRQCOUNTER                   ; routine
    LDA #<IRQ2                          ; Set up the sprite display IRQ
    STA $0314
    LDA #>IRQ2
    STA $0315
    JMP IRQ2_DIRECT                     ; Go directly; we might be late
IRQ1_NOSPRITESATALL:
    JMP $EA81
;-------------------
IRQ1_BEGINSORT:
    .IFDEF DEBUG 
    INC $D020
    .ENDIF
    LDX #MAXSPR
    DEX
    CPX SORTEDSPRITES
    BCC IRQ1_CLEARDONE
    LDA #$FF                            ; Mark unused sprites with the
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
    INX
    CPX SORTEDSPRITES
    BCC IRQ1_SORTLOOP3
    .IFDEF DEBUG
    DEC $D020
    .ENDIF
    JMP IRQ1_NONEWSPRITES
;---------------------------------------
; Raster interrupt 2.
; This is where sprite displaying happens
;-------------------
IRQ2:
    DEC $D019                           ; Acknowledge raster interrupt
IRQ2_DIRECT:
    LDY SPRIRQCOUNTER                   ; Take next sorted sprite number
    LDA SORTSPRY,Y                      ; Take Y-coord of first new sprite
    CLC
    ADC #$16                            ; 16 lines down from there is
    BCC IRQ2_NOTOVER                    ; the endpoint for this IRQ
    LDA #$FF                            ; Endpoint can't be more than $ff
IRQ2_NOTOVER:
    STA TEMPVARIABLE
IRQ2_SPRITELOOP:
    LDA SORTSPRY,Y
    CMP TEMPVARIABLE                    ; End of this IRQ?
    BCS IRQ2_ENDSPR
    LDX PHYSICALSPRTBL2,Y               ; Physical sprite number x 2
    STA $D001,X                         ; for X & Y coordinate
    LDA SORTSPRX,Y
    ASL
    STA $D000,X
    BCC IRQ2_LOWMSB
    LDA $D010
    ORA ORTBL,X
    STA $D010
    JMP IRQ2_MSBOK
IRQ2_LOWMSB:
    LDA $D010
    AND ANDTBL,X
    STA $D010
IRQ2_MSBOK:
    LDX PHYSICALSPRTBL1,Y               ; Physical sprite number x 1
    LDA SORTSPRF,Y
    STA $07F8,X                         ; for color & frame
    LDA SORTSPRC,Y
    STA $D027,X
    INY
    BNE IRQ2_SPRITELOOP
IRQ2_ENDSPR:
    CMP #$FF                            ; Was it the endmark?
    BEQ IRQ2_LASTSPR
    STY SPRIRQCOUNTER
    SEC                                 ; THAT COORDINATE - $10 IS THE
    SBC #$08                            ; position for next interrupt
    CMP $D012                           ; Already late from that?
    BCC IRQ2_DIRECT                     ; Then go directly to next IRQ
    STA $D012
    JMP $EA81
IRQ2_LASTSPR:
    LDA #<IRQ1                          ; Was the last sprite,
    STA $0314                           ; go back to irq1
    LDA #>IRQ1                          ; (sorting interrupt)
    STA $0315
    LDA #IRQ1LINE
    STA $D012
    JMP $EA81
;---------------------------------------
_SPRX:
SPRX:
    .RES MAXSPR,0                       ; Unsorted sprite table
_SPRY:
SPRY:
    .RES MAXSPR,0
_SPRC:
SPRC:
    .RES MAXSPR,0
_SPRF:
SPRF:
    .RES MAXSPR,0
;-------------------
SORTSPRX:
    .RES MAXSPR,0                       ; Sorted sprite table
SORTSPRY:
    .RES MAXSPR+1,0                     ; Must be one byte extra for the $ff endmark
SORTSPRC:
    .RES MAXSPR,0
SORTSPRF:
    .RES MAXSPR,0
;-------------------
D015TBL:
    .BYTE %00000000                     ; Table of sprites that are "on"
    .BYTE %00000001                     ; for $d015
    .BYTE %00000011
    .BYTE %00000111
    .BYTE %00001111
    .BYTE %00011111
    .BYTE %00111111
    .BYTE %01111111
    .BYTE %11111111
;-------------------
PHYSICALSPRTBL1:
    .BYTE 0, 1, 2, 3, 4, 5, 6, 7        ; Indexes to frame & color
    .BYTE 0, 1, 2, 3, 4, 5, 6, 7        ; registers
    .BYTE 0, 1, 2, 3, 4, 5, 6, 7
    .BYTE 0, 1, 2, 3, 4, 5, 6, 7
    .BYTE 0, 1, 2, 3, 4, 5, 6, 7
    .BYTE 0, 1, 2, 3, 4, 5, 6, 7
    .BYTE 0, 1, 2, 3, 4, 5, 6, 7
    .BYTE 0, 1, 2, 3, 4, 5, 6, 7
PHYSICALSPRTBL2:
    .BYTE 0, 2, 4, 6, 8, 10, 12, 14
    .BYTE 0, 2, 4, 6, 8, 10, 12, 14
    .BYTE 0, 2, 4, 6, 8, 10, 12, 14
    .BYTE 0, 2, 4, 6, 8, 10, 12, 14
    .BYTE 0, 2, 4, 6, 8, 10, 12, 14
    .BYTE 0, 2, 4, 6, 8, 10, 12, 14
    .BYTE 0, 2, 4, 6, 8, 10, 12, 14
    .BYTE 0, 2, 4, 6, 8, 10, 12, 14
;-------------------
ANDTBL:
    .BYTE 255-1
ORTBL:
    .BYTE 1
    .BYTE 255-2
    .BYTE 2
    .BYTE 255-4
    .BYTE 4
    .BYTE 255-8
    .BYTE 8
    .BYTE 255-16
    .BYTE 16
    .BYTE 255-32
    .BYTE 32
    .BYTE 255-64
    .BYTE 64
    .BYTE 255-128
    .BYTE 128
;--------------------------------------
    .ALIGN $40
_SPRITE_GFX:
    .RES 64,255                         ; Our sprite. Really complex design :-)
;-------------------------------------------------------------------------------
