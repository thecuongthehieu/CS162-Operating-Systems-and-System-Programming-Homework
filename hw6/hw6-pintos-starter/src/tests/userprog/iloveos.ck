# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected (IGNORE_USER_FAULTS => 1, [<<'EOF']);
(iloveos) begin
I love CS162
(iloveos) end
iloveos: exit(0)
EOF
pass;
