# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(realloc-null) begin
(realloc-null) end
realloc-null: exit(0)
EOF
pass;
