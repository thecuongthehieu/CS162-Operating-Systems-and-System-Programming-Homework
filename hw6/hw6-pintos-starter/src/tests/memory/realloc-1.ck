# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(realloc-1) begin
(realloc-1) end
realloc-1: exit(0)
EOF
pass;
