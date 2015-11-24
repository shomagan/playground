#include <stdio.h>
#include <math.h>
int main(int argc, const char * argv[]) {
    FILE *file = fopen(argv[1], "r");
    char line[1024];
    signed int x1,y1,x2,y2;
    x1 = x2 = y1 = y2 =0;
    while (fgets(line, 1024, file)) {
        // Do something with the line
        int n = sscanf(line, "(%d, %d) (%d, %d)", &x1,&y1,&x2,&y2);
        int distance = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
        printf(" %d %d %d %d ",x1,y1,x2,y2);
        printf("%d",distance);
 //       printf(distance);
    }
    return 0;
}