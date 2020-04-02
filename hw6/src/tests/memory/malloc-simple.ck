# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
(malloc-simple) begin
(malloc-simple) David Culler is the instructor for CS 162
(malloc-simple) end
malloc-simple: exit(0)
EOF
pass;
