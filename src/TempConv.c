#include <stdio.h>
#include <string.h>

static float fahrenheitToCelcius(int fahr);

int temperatureConvert(char *buf, size_t size, int lower, int upper, int step)
{
    size_t len = snprintf(buf, size, "F   C\n");

    int fahr;
    int end;

    if(step > 0)
    {
        fahr = lower;
        end = upper;
    } 
    else if (step < 0)
    {
        fahr = upper;
        end = lower;
    }
    else
    {
        printf("Illigeal step size (0)\n");
        return 1;
    }

    for (; (step > 0) ? (fahr <= end) : (fahr >= end); fahr += step)
    {
        len += snprintf(buf + len, size - len, "%3d %6.1f\n", fahr, fahrenheitToCelcius(fahr));
    }
    
    return 0;
}

static float fahrenheitToCelcius(int fahr)
{
    return (5.0/9.0)*(fahr-32);
}
