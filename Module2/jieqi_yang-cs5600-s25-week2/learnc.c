/*
 * learnc.c / Week 2: Assignment
 *
 * Jieqi Yang / CS5600 / Northeastern University
 * Spring 2025 / Jan 24, 2025
 *
 */
#include "mpg2km.h"
#include <stdio.h>

int main()
{
    double mpg, lph;
    printf("-----Valid test cases-----\n");
    // Test mpg2kml
    mpg = 25.0;
    printf("%.2f MPG is %.2f KML\n", mpg, mpg2kml(mpg));

    // Test mpg2lphm
    printf("%.2f MPG is %.2f L/100KM\n", mpg, mpg2lphm(mpg));
    lph = 9.0;
    printf("%.2f L/100KM is %.2f MPG\n", lph, lph2mpg(lph));

    printf("-----Invalid test cases-----\n");
    
    // Negative MPG    
    mpg = -25.0;
    printf("Testing with %.2f MPG:\n", mpg);
    printf("%.2f KML\n", mpg2kml(mpg));
    printf("%.2f L/100KM\n", mpg2lphm(mpg));

    // Zero MPG
    mpg = 0.0;
    printf("\nTesting with %.2f MPG:\n", mpg);
    printf("%.2f KML\n", mpg2kml(mpg));
    printf("%.2f L/100KM\n", mpg2lphm(mpg));

    // Negative LPH
    lph = -10.0;
    printf("\nTesting with %.2f L/100KM:\n", lph);
    printf("%.2f MPG\n", lph2mpg(lph));

    // Zero LPH
    lph = 0.0;
    printf("\nTesting with %.2f L/100KM:\n", lph);
    printf("%.2f MPG\n", lph2mpg(lph));	

    // Extremely large values
    mpg = 1e6; // 1,000,000 MPG
    printf("\nTesting with %.2e MPG:\n", mpg);
    printf("%.2f KML\n", mpg2kml(mpg));
    printf("%.2f L/100KM\n", mpg2lphm(mpg));

    lph = 1e6; // 1,000,000 L/100KM
    printf("\nTesting with %.2e L/100KM:\n", lph);
    printf("%.2f MPG\n", lph2mpg(lph));
    
    return 0;
}
