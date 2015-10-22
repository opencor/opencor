001     a=pow(b, 3{dimensionless});
002     a=pow(-b, 3{dimensionless});        // NOK
003     a=-pow(b, 3{dimensionless});        // NOK

004     a=pow(b, c);
005     a=pow(-b, c);                       // NOK
006     a=-pow(b, c);                       // NOK

007     a=pow(b, c+d);
008     a=pow(-b, c+d);                     // NOK
009     a=-pow(b, c+d);                     // NOK
