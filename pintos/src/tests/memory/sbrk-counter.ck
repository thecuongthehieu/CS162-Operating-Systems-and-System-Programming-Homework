# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-counter) begin
(sbrk-counter) Done counting; about to access memory out of bounds
sbrk-counter: exit(-1)
EOF
pass;
