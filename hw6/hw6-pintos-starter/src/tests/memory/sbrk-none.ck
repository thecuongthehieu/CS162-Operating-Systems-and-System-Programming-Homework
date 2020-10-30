# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-none) begin
sbrk-none: exit(-1)
EOF
pass;
