# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(malloc-null) begin
(malloc-null) end
malloc-null: exit(0)
EOF
pass;
