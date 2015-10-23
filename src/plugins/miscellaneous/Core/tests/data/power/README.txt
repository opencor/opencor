a=pow(b, 3{dimensionless});                     // 001
a=pow(-b, 3{dimensionless});                    // 002
a=-pow(b, 3{dimensionless});                    // 003  NOK

a=pow(b, c);                                    // 004
a=pow(-b, c);                                   // 005
a=-pow(b, c);                                   // 006  NOK

a=pow(b, c+d);                                  // 007
a=pow(-b, c+d);                                 // 008
a=-pow(b, c+d);                                 // 009  NOK
