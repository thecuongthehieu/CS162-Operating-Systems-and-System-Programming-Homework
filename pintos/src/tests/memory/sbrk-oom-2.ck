# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(sbrk-oom-2) begin
(sbrk-oom-2) Completed all 1000 iterations
(sbrk-oom-2) Now checking that memory is actually unmapped
sbrk-oom-2: exit(-1)
EOF
pass;
