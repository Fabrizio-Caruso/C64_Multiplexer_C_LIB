;--------------------------------------
; Irq test-case
;-------------------
    .INCLUDE "zeropage.inc"
    .IF .DEFINED(__C64__)                       ; Include file for multi platform compilation
        .INCLUDE "c64.inc"
    .ELSEIF .DEFINED(__C128__)
        .INCLUDE "c128.inc"
    .ELSEIF .DEFINED(__CBM510__)
        .INCLUDE "cbm510.inc"
    .ENDIF 
;-------------------
   .EXPORT _INITRASTER
;-------------------
IRQ1LINE = $FC                          ; This is the place on screen where the sorting IRQ happens
;---------------------------------------
; Routine to init the
; raster interrupt system
;-------------------
_INITRASTER:
    LDA #$7F
    STA CIA1_ICR                        ; CIA interrupt off
    SEI
    .IFDEF __C64__                       ; C64 RAM setup
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
    LDA #$01
    STA VIC_IMR                         ; Raster interrupt on
    LDA #$1B                            ; High bit of interrupt position = 0
    STA VIC_CTRL1
    LDA #IRQ1LINE                       ; Line where next IRQ happens
    STA VIC_HLINE
    LDA CIA1_ICR                        ; Acknowledge IRQ (to be sure)
    LDA #$0E
    STA $FF00
    CLI
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
    INC $D020
    INC $D021
    LDA $D012
IRQ1_WR:
    CMP $D012
    BEQ IRQ1_WR
    DEC $D020
    DEC $D021
;-------------------
EXIT_IRQ:                               ; Exit IRQ code.
    LSR VIC_IRR                         ; Acknowledge raster IRQ
    .IF .DEFINED(__C64__)
        .IF .NOT .DEFINED(USE_KERNAL)
            STORE_A = *+$0001                       ; Restore original registers value
            LDA #$00
            STORE_X = *+$0001                       ; at the original values they have before
            LDX #$00
            STORE_Y = *+$0001                       ; IRQ call
            LDY #$00         
        .ELSE
            JMP $EA81
        .ENDIF
    .ELSEIF .DEFINED(__C128__) 
        PLA
        STA $FF00
        PLA
        TAY
        PLA
        TAX
        PLA
    .ENDIF
IRQ_RTI:
    RTI                                 ; ReTurn from Interrupt 
;-------------------------------------------------------------------------------
