# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(realloc-3) begin
(realloc-3) end
realloc-3: exit(0)
EOF
pass;
