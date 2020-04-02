# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-page) begin
(sbrk-page) Fall 2019 is the best semester to take CS 162
(sbrk-page) end
sbrk-page: exit(0)
EOF
pass;
