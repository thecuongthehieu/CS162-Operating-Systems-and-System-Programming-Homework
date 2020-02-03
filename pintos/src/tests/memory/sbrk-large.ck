# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-large) begin
(sbrk-large) end
sbrk-large: exit(0)
EOF
pass;
