a=log(b);                                       // 001
a=log(-b);                                      // 002  NOK
a=-log(b);                                      // 003

a=log(b, 3{dimensionless});                     // 001
a=log(-b, 3{dimensionless});                    // 002  NOK
a=-log(b, 3{dimensionless});                    // 003

a=log(b, c);                                    // 004  NOK
a=log(-b, c);                                   // 005  NOK
a=-log(b, c);                                   // 006  NOK

a=log(b, c+d);                                  // 007  NOK
a=log(-b, c+d);                                 // 008  NOK
a=-log(b, c+d);                                 // 009  NOK
