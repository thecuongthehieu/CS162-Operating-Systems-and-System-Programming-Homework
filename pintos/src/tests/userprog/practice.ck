# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected (IGNORE_USER_FAULTS => 1, [<<'EOF']);
(practice) begin
practice
(practice) end
practice: exit(0)
EOF
pass;
