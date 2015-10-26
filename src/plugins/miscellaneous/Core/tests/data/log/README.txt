a=log(b);                                       // 001
a=log(-b);                                      // 002  NOK
a=-log(b);                                      // 003

a=log(b, 3{dimensionless});                     // 004
a=log(-b, 3{dimensionless});                    // 005  NOK
a=-log(b, 3{dimensionless});                    // 006

a=log(b, c);                                    // 007  NOK
a=log(-b, c);                                   // 008  NOK
a=-log(b, c);                                   // 009  NOK

a=log(b, c+d);                                  // 010  NOK
a=log(-b, c+d);                                 // 011  NOK
a=-log(b, c+d);                                 // 012  NOK
