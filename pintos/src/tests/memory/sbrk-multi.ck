# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-multi) begin
(sbrk-multi) Fall 2019 is the best semester to take CS 162
(sbrk-multi) end
sbrk-multi: exit(0)
EOF
pass;
