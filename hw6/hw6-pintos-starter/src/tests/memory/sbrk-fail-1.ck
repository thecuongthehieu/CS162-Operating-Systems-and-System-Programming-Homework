# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-fail-1) begin
(sbrk-fail-1) end
sbrk-fail-1: exit(0)
EOF
pass;
