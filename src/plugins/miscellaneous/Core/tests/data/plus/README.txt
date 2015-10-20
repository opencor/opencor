001     a=b+c;
002     a=-b+c;

003     a=b+c+d;
004     a=-b+c+d;

005     a=b+c+d+e;
006     a=-b+c+d+e;
007     a=(b+c)+d+e;
008     a=-(b+c)+d+e;       // NOK
009     a=b+c+(d+e);
010     a=(b+c)+(d+e);
011     a=-(b+c)+(d+e);     // NOK

012     a=b-c+d-e;
013     a=-b-c+d-e;
014     a=(b-c)+d-e;
015     a=-(b-c)+d-e;       // NOK
016     a=b-c+(d-e);        // NOK
017     a=(b-c)+(d-e);      // NOK
018     a=-(b-c)+(d-e);     // NOK

019     a=b*c+d*e;
020     a=-b*c+d*e;         // Kind of NOK
022     a=-(b*c)+d*e;

026     a=b/c+d/e;
027     a=-b/c+d/e;
029     a=-(b/c)+d/e;
