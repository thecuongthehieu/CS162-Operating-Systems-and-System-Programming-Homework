# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(stack-align) begin
(stack-align) last nybble of candidate is 0x0
(stack-align) end
stack-align-2: exit(0)
EOF
pass;
