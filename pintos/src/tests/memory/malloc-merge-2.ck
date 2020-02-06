# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(malloc-merge-2) begin
(malloc-merge-2) end
malloc-merge-2: exit(0)
EOF
pass;
