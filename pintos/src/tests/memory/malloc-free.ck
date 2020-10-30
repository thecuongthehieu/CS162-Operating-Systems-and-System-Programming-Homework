# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(malloc-free) begin
(malloc-free) end
malloc-free: exit(0)
EOF
pass;
