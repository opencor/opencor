001     a=root(b, 2{dimensionless});
002     a=root(-b, 2{dimensionless});
003     a=-root(b, 2{dimensionless});

004     a=root(b, 3{dimensionless});
005     a=root(-b, 3{dimensionless});
006     a=-root(b, 3{dimensionless});

007     a=root(b, c);                       // NOK
008     a=root(-b, c);                      // NOK
009     a=-root(b, c);                      // NOK

010     a=root(b, c+d);                     // NOK
011     a=root(-b, c+d);                    // NOK
012     a=-root(b, c+d);                    // NOK
