# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-mebi) begin
(sbrk-mebi) end
sbrk-mebi: exit(0)
EOF
pass;
