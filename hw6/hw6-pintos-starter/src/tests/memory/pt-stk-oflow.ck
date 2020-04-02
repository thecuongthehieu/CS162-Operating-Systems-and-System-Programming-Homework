# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected (IGNORE_USER_FAULTS => 1, [<<'EOF']);
(pt-stk-oflow) begin
pt-stk-oflow: exit(-1)
EOF
pass;
