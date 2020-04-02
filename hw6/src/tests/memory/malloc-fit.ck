# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(malloc-fit) begin
(malloc-fit) end
malloc-fit: exit(0)
EOF
pass;
