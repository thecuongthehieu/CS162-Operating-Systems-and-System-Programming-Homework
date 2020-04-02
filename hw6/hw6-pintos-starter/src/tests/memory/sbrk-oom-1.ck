# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-oom-1) begin
(sbrk-oom-1) Completed all 25 iterations
(sbrk-oom-1) Now checking that memory is actually unmapped
sbrk-oom-1: exit(-1)
EOF
pass;
