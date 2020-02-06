# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-zero) begin
(sbrk-zero) end
sbrk-zero: exit(0)
EOF
pass;
