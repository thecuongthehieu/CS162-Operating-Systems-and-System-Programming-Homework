# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
do-stack-align: exit(12)
EOF
pass;
