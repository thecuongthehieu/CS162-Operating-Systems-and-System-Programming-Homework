# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-rv) begin
(sbrk-rv) end
sbrk-rv: exit(0)
EOF
pass;
