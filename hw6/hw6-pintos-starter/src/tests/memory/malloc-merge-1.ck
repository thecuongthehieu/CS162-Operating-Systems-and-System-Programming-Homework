# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(malloc-merge-1) begin
(malloc-merge-1) end
malloc-merge-1: exit(0)
EOF
pass;
