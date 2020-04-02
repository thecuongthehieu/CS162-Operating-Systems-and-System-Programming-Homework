# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(realloc-2) begin
(realloc-2) end
realloc-2: exit(0)
EOF
pass;
