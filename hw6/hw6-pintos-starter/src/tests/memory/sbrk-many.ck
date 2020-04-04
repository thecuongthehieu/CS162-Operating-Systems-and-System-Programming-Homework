# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-many) begin
(sbrk-many) end
sbrk-many: exit(0)
EOF
pass;
