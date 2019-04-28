    .export _startirq
    .export _stopirq

_startirq:
    sei
    lda #>main_irq
    sta $0315
    sta $ffff
    lda #<main_irq
    sta $0314
    sta $fffe
    lda #$7f
    sta $dc0d
    lda #$01
    sta $d01a
    lda #$1b
    sta $d011
    lda #$30
    sta $d012
    lda #$00
    sta irq_flag
    cli
    rts

_stopirq:

    sei
    lda #$ea
    sta $0315
    lda #$31
    sta $0314
    inc $d019
    lda #$81
    sta $dc0d
    lda #$00
    sta $d01a
    cli
    rts
    
irq_flag:
    .byte $00
    
main_irq:
    lda $d012
raster_w1:
    cmp $d012
    beq raster_w1
    inc $d020
    inc $d021
    lda $d012
raster_w2:
    cmp $d012
    beq raster_w2
    dec $d020
    dec $d021
    asl $d019
    jmp $ea81
    
