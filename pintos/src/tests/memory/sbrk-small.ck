# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-small) begin
(sbrk-small) Fall 2019 is the best semester to take CS 162
(sbrk-small) end
sbrk-small: exit(0)
EOF
pass;
