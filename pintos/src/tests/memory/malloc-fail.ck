# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(malloc-fail) begin
(malloc-fail) end
malloc-fail: exit(0)
EOF
pass;
