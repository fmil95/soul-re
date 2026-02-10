.include "gte_macros.s"

.macro glabel label
    .global \label
    \label:
.endm

.macro jlabel label, visibility=local
    .\visibility \label
    \label:
.endm

.macro move a, b
    addu \a, \b, $zero
.endm
