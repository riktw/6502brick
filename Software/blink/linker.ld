.file [name="blink.bin", type="bin", segments="Program"]
.segmentdef Program [segments="Reset, Data, Code, IRQ"]

.segmentdef Reset [start=$FFFC]	
.segmentdef Data [start=$D000]
.segmentdef Code [start=$C000]
.segmentdef IRQ [start=$F000]

.segment Reset		//0xC000 for Code, 0xF000 for IRQ
.fill 1, $00
.fill 1, $C0
.fill 1, $00
.fill 1, $F0

.encoding "ascii"  //Default is petscii
.segment Data
.segment Code 
.segment IRQ
