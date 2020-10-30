# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-fail-2) begin
sbrk-fail-2: exit(-1)
EOF
pass;
