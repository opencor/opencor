::

    a=rem(b, 3{dimensionless});                     // 001
    a=rem(-b, 3{dimensionless});                    // 002
    a=-rem(b, 3{dimensionless});                    // 003

    a=rem(b, c);                                    // 004
    a=rem(-b, c);                                   // 005
    a=-rem(b, c);                                   // 006

    a=rem(b, c+d);                                  // 007
    a=rem(-b, c+d);                                 // 008
    a=-rem(b, c+d);                                 // 009
