; Interrupt Handler

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
