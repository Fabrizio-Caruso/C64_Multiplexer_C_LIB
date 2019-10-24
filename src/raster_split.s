.IF .DEFINED(__C128__)
    .SEGMENT "CODE2"
.ENDIF

    .IF .DEFINED(BASIC)
        USE_KERNAL=1
        .BYTE $00, $C0    
        .ORG $C000         
    .ENDIF
;--------------------------------------
; C64 sprites multiplexer
; Based on 32 sprites multiplexer
; by Lasse Oorni (Cadaver)
;-------------------
    .INCLUDE "zeropage.inc"
    .IF .DEFINED(__C64__) .OR .DEFINED(BASIC)
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
        FULL_STANDARD_KERNAL=$EA31
        LIGHT_STANDARD_KERNAL=$EA81
   .ELSEIF .DEFINED(__C128__)
        FULL_STANDARD_KERNAL=$FA65
        LIGHT_STANDARD_KERNAL=$FF33
   .ENDIF   
   
   .IFDEF STANDARD_IRQ
        KERNAL_IRQ=FULL_STANDARD_KERNAL
   .ELSE
        KERNAL_IRQ=LIGHT_STANDARD_KERNAL
   .ENDIF
 
   TOP_KERNAL_IRQ=LIGHT_STANDARD_KERNAL
   BOTTOM_KERNAL_IRQ=FULL_STANDARD_KERNAL

   
   .MACRO handle_x spr_number 
        LDA SPRX+spr_number
        ASL                                 ; multiply by 2
        STA VIC_SPR0_X+(spr_number & 7)*2   ; weird effects when sprite
        LDA VIC_SPR_HI_X                    ; Load actual VIC_SPR_HI_X value        
        BCS :+                              ; if > 255 set sprite MSB in VIC_SPR_HI_X
        AND #<~(%1 << (spr_number & 7))     ; Do a logic AND operation to set actual sprite bit OFF
        JMP :++
    :
        ORA #%1 << (spr_number & 7)         ; Do a logic OR operation to set actual sprite bit ON
    :
        STA VIC_SPR_HI_X                    ; and store new value back to the VIC_SPR_HI_X register.
   .ENDMACRO
;-------------------
;DEBUG = $00                             ; Set to != $00 to show rastertime usage.
;USE_KERNAL = $01                        ; Set to != $00 to enable normal kernal usage
;MULTICOLOR = $01                        ; Set to != $00 to enable multicolor sprites flag
;EXPANDX= $01                            ; Set to != $00 to enable expand_x sprites flag
;EXPANDY= $01                            ; Set to != $00 to enable expand_y sprites flag
;-------------------
SCREEN_RAM = $0400                      ; Screen ram start address
;-------------------
IRQTOPLINE = $23                        ; Sprites display IRQ at rasterline $023.
IRQBOTTOMLINE = $90                     ; Sorting code IRQ at rasterline $0FC

;-------------------
;MUSIC_CODE = $01                       ; Set to $01 to enable music routines

 .IF .NOT .DEFINED(FREE_SPRITES)
     FREE_SPRITES=0
 .ENDIF

.IF .DEFINED(__C64__)
    MUSIC_INIT = $1000                  ; Music init address for C64 specific compilation
    MUSIC_PLAY = $1003                  ; Music play address for C64 specific compilation
.ENDIF
.IF .DEFINED(__C128__)
    MUSIC_INIT = $2400                  ; Music init address for C128 specific compilation
    MUSIC_PLAY = $2403                  ; Music play address for C128 specific compilation
.ENDIF    

.IF .NOT .DEFINED(MAXSPR)
    MAXSPR = 16                            ; Maximum number of sprites
.ENDIF
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
    .IF .NOT .DEFINED(BASIC)
    LDA #$7F
    STA CIA1_ICR                        ; CIA interrupt off
    .ENDIF
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

    .IFDEF __C128__
        LDA #$FF
        STA $D8
        LDA $0A04
        AND #$FE
        STA $0A04
    .ENDIF
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
    .IFDEF DEBUG 
        INC VIC_BORDERCOLOR             ; Show rastertime usage for debug.
    .ENDIF
    .IFNDEF USE_KERNAL
        STA STORE_A                     ; Fast way to store/restore
        STX STORE_X                     ; CPU regs after an IRQ
        STY STORE_Y                     ; for kernal OFF only
    .ENDIF

    LDA SPRY
    STA VIC_SPR0_Y                     ; to the bottom to prevent
    .IF FREE_SPRITES<7
    LDA SPRY+$01
    STA VIC_SPR1_Y                      ; weird effects when sprite
    .ENDIF
    .IF FREE_SPRITES<6    
    LDA SPRY+$02
    .ENDIF
    STA VIC_SPR2_Y                      ; moves lower than what it
    .IF FREE_SPRITES<5    
    LDA SPRY+$03
    STA VIC_SPR3_Y                      ; previously was
    .ENDIF
    .IF FREE_SPRITES<4
    LDA SPRY+$04
    STA VIC_SPR4_Y
    .ENDIF
    .IF FREE_SPRITES<3
    LDA SPRY+$05
    STA VIC_SPR5_Y
    .ENDIF
    .IF FREE_SPRITES<2
    LDA SPRY+$06
    STA VIC_SPR6_Y    
    .ENDIF
    .IF FREE_SPRITES<1
    LDA SPRY+$07
    STA VIC_SPR7_Y   
    .ENDIF
    
    handle_x $0
    .IF FREE_SPRITES<7
    handle_x $1
    .ENDIF    
    .IF FREE_SPRITES<6    
    handle_x $2
    .ENDIF
    .IF FREE_SPRITES<5    
    handle_x $3
    .ENDIF
    .IF FREE_SPRITES<4
    handle_x $4
    .ENDIF
    .IF FREE_SPRITES<3
    handle_x $5
    .ENDIF
    .IF FREE_SPRITES<2
    handle_x $6
    .ENDIF
    .IF FREE_SPRITES<1     
    handle_x $7
    .ENDIF

    LDX SPRF                         ; Physical sprite number x 1
    STX SCREEN_RAM+$03F8              ; Set sprite frame pointer to actual sprite register
    .IF FREE_SPRITES<7    
    LDX SPRF+$01
    STX SCREEN_RAM+$03F9              ; Set sprite frame pointer to actual sprite register
    .ENDIF
    .IF FREE_SPRITES<6    
    LDX SPRF+$02
    STX SCREEN_RAM+$03FA              ; Set sprite frame pointer to actual sprite register
    .ENDIF
    .IF FREE_SPRITES<5
    LDX SPRF+$03
    .ENDIF
    STX SCREEN_RAM+$03FB              ; Set sprite frame pointer to actual sprite register
    .IF FREE_SPRITES<4
    LDX SPRF+$04
    STX SCREEN_RAM+$03FC              ; Set sprite frame pointer to actual sprite register
    .ENDIF
    .IF FREE_SPRITES<3
    LDX SPRF+$05
    STX SCREEN_RAM+$03FD              ; Set sprite frame pointer to actual sprite register
    .ENDIF
    .IF FREE_SPRITES<2
    LDX SPRF+$06
    STX SCREEN_RAM+$03FE              ; Set sprite frame pointer to actual sprite register
    .ENDIF
    .IF FREE_SPRITES<1 
    LDX SPRF+$07
    STX SCREEN_RAM+$03FF              ; Set sprite frame pointer to actual sprite register
    .ENDIF
    
    LDA SPRC
    STA VIC_SPR0_COLOR                ; Set sprite color to actual sprite color register
    .IF FREE_SPRITES<7    
    LDA SPRC+$01    
    STA VIC_SPR1_COLOR
    .ENDIF
    .IF FREE_SPRITES<6
    LDA SPRC+$02
    STA VIC_SPR2_COLOR                ; Set sprite color to actual sprite color register
    .ENDIF
    .IF FREE_SPRITES<5    
    LDA SPRC+$03    
    STA VIC_SPR3_COLOR
    .ENDIF
    .IF FREE_SPRITES<4
    LDA SPRC+$04
    STA VIC_SPR4_COLOR                ; Set sprite color to actual sprite color register
    .ENDIF
    .IF FREE_SPRITES<3
    LDA SPRC+$05
    STA VIC_SPR5_COLOR
    .ENDIF
    .IF FREE_SPRITES<2
    LDA SPRC+$06
    STA VIC_SPR6_COLOR                ; Set sprite color to actual sprite color register    
    .ENDIF
    .IF FREE_SPRITES<1
    LDA SPRC+$07
    STA VIC_SPR7_COLOR
    .ENDIF
    
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

    .IFDEF DEBUG 
        DEC VIC_BORDERCOLOR             ; Show rastertime usage for debug.
    .ENDIF    
.IF .NOT .DEFINED(BASIC)
    JMP EXIT_IRQ
.ELSE
    JMP TOP_KERNAL_IRQ                   ; Use normal Kernal C128 IRQ exit code if Kernal is ON     
.ENDIF
;---------------------------------------
; Raster interrupt 3.
; This is where sprite displaying happens
;-------------------
IRQBOTTOM:
    .IFDEF DEBUG 
        INC VIC_BORDERCOLOR             ; Show rastertime usage for debug.
    .ENDIF   
    .IFNDEF USE_KERNAL
        STA STORE_A                     ; Fast way to store/restore
        STX STORE_X                     ; CPU regs after an IRQ
        STY STORE_Y                     ; for kernal OFF only
    .ENDIF
    

    LDA SPRY+$08
    STA VIC_SPR0_Y                      ; weird effects when sprite
    .IF FREE_SPRITES<7    
    LDA SPRY+$09
    STA VIC_SPR1_Y                      ; moves lower than what it
    .ENDIF
    .IF FREE_SPRITES<6    
    LDA SPRY+$0A
    STA VIC_SPR2_Y                      ; previously was
    .ENDIF
    .IF FREE_SPRITES<5    
    LDA SPRY+$0B
    STA VIC_SPR3_Y
    .ENDIF
    .IF FREE_SPRITES<4
    LDA SPRY+$0C
    STA VIC_SPR4_Y
    .ENDIF
    .IF FREE_SPRITES<3
    LDA SPRY+$0D
    STA VIC_SPR5_Y
    .ENDIF
    .IF FREE_SPRITES<2
    LDA SPRY+$0E
    STA VIC_SPR6_Y
    .ENDIF 
    .IF FREE_SPRITES<1     
    LDA SPRY+$0F
    STA VIC_SPR7_Y                     ; to the bottom to prevent
    .ENDIF
    
    handle_x $8
    .IF FREE_SPRITES<7    
    handle_x $9
    .ENDIF
    .IF FREE_SPRITES<6    
    handle_x $A
    .ENDIF
    .IF FREE_SPRITES<5    
    handle_x $B
    .ENDIF
    .IF FREE_SPRITES<4
    handle_x $C
    .ENDIF
    .IF FREE_SPRITES<3
    handle_x $D
    .ENDIF
    .IF FREE_SPRITES<2
    handle_x $E
    .ENDIF
    .IF FREE_SPRITES<1     
    handle_x $F
    .ENDIF

    LDX SPRF+$08                         ; Physical sprite number x 1
    STX SCREEN_RAM+$03F8              ; Set sprite frame pointer to actual sprite register
    .IF FREE_SPRITES<7    
    LDX SPRF+$09
    STX SCREEN_RAM+$03F9              ; Set sprite frame pointer to actual sprite register
    .ENDIF
    .IF FREE_SPRITES<6    
    LDX SPRF+$0A
    STX SCREEN_RAM+$03FA              ; Set sprite frame pointer to actual sprite register
    .ENDIF
    .IF FREE_SPRITES<5    
    LDX SPRF+$0B
    STX SCREEN_RAM+$03FB              ; Set sprite frame pointer to actual sprite register
    .ENDIF
    .IF FREE_SPRITES<4
    LDX SPRF+$0C
    STX SCREEN_RAM+$03FC              ; Set sprite frame pointer to actual sprite register
    .ENDIF
    .IF FREE_SPRITES<3
    LDX SPRF+$0D
    STX SCREEN_RAM+$03FD              ; Set sprite frame pointer to actual sprite register
    .ENDIF
    .IF FREE_SPRITES<2
    LDX SPRF+$0E
    STX SCREEN_RAM+$03FE              ; Set sprite frame pointer to actual sprite register
    .ENDIF
    .IF FREE_SPRITES<1     
    LDX SPRF+$0F
    STX SCREEN_RAM+$03FF              ; Set sprite frame pointer to actual sprite register
    .ENDIF
    
    LDA SPRC+$08 
    STA VIC_SPR0_COLOR                ; Set sprite color to actual sprite color register
    .IF FREE_SPRITES<7    
    LDA SPRC+$09    
    STA VIC_SPR1_COLOR
    .ENDIF
    .IF FREE_SPRITES<6    
    LDA SPRC+$0A
    STA VIC_SPR2_COLOR                ; Set sprite color to actual sprite color register
    .ENDIF
    .IF FREE_SPRITES<5    
    LDA SPRC+$0B    
    STA VIC_SPR3_COLOR
    .ENDIF
    .IF FREE_SPRITES<4
    LDA SPRC+$0C
    STA VIC_SPR4_COLOR                ; Set sprite color to actual sprite color register
    .ENDIF
    .IF FREE_SPRITES<3
    LDA SPRC+$0D
    STA VIC_SPR5_COLOR
    .ENDIF
    .IF FREE_SPRITES<2
    LDA SPRC+$0E
    STA VIC_SPR6_COLOR                ; Set sprite color to actual sprite color register    
    .ENDIF
    .IF FREE_SPRITES<1     
    LDA SPRC+$0F
    STA VIC_SPR7_COLOR        
    .ENDIF
    
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

    .IFDEF DEBUG 
        DEC VIC_BORDERCOLOR             ; Show rastertime usage for debug.
    .ENDIF    
    ;JMP EXIT_IRQ
;-------------------
EXIT_IRQ:                               ; Exit IRQ code.
    LSR VIC_IRR                         ; Acknowledge raster IRQ
    .IF .DEFINED(__C64__)
        .IF .NOT .DEFINED(USE_KERNAL) .OR .DEFINED(BASIC)
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
    .ELSEIF .DEFINED(BASIC)
            JMP BOTTOM_KERNAL_IRQ                   ; Use normal Kernal C128 IRQ exit code if Kernal is ON     
    .ENDIF
IRQ_RTI:
    RTI                                 ; ReTurn from Interrupt 
;---------------------------------------
_SPRX:
SPRX:
    .RES MAXSPR, $40                    ; Unsorted sprites X coords array
_SPRY:
SPRY:
    .RES MAXSPR, $80                    ; Unsorted sprites Y coords array
_SPRC:
SPRC:
    .RES MAXSPR, $00                    ; Unsorted sprites colors array
_SPRF:
SPRF:
    .RES MAXSPR, $80                    ; Unsorted sprites frame pointer array
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

