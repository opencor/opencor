a=pow(b, 3{dimensionless});                     // 001
a=pow(-b, 3{dimensionless});                    // 002
a=-pow(b, 3{dimensionless});                    // 003

a=pow(b, c);                                    // 004
a=pow(-b, c);                                   // 005
a=-pow(b, c);                                   // 006

a=pow(b, c+d);                                  // 007
a=pow(-b, c+d);                                 // 008
a=-pow(b, c+d);                                 // 009

a=pow(b, -3{dimensionless});                    // 010
a=pow(b, -c);                                   // 011

a=pow(b+c, d);                                  // 012
a=pow(-b+c, d);                                 // 013
a=-pow(b+c, d);                                 // 014

a=pow(b-c, d);                                  // 015
a=pow(-b-c, d);                                 // 016
a=-pow(b-c, d);                                 // 017

a=pow(b*c, d);                                  // 018
a=pow(-b*c, d);                                 // 019
a=-pow(b*c, d);                                 // 020

a=pow(b/c, d);                                  // 021
a=pow(-b/c, d);                                 // 022
a=-pow(b/c, d);                                 // 023
