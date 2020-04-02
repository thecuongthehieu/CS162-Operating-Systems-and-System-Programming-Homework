# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-dealloc) begin
(sbrk-dealloc) Allocated 13000 bytes
(sbrk-dealloc) Accessed 13000 bytes
(sbrk-dealloc) Deallocated 712 bytes
(sbrk-dealloc) Accessed 12288 bytes
sbrk-dealloc: exit(-1)
EOF
pass;
