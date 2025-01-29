#include <stdio.h>
#include "mpg2km.h"

// Function to convert miles per gallon (MPG) to kilometers per liter (KML)
double mpg2kml(double mpg) {
    if (mpg <= 0) {
        printf("Error: MPG must be positive.\n");
        return -1;
    }

    return mpg * 1.609344 / 3.785411784;
}

// Function to convert miles per gallon (MPG) to liters per 100 kilometers (LPH)
double mpg2lphm(double mpg) {
    if (mpg <= 0) {
        printf("Error: MPG must be positive.\n");
        return -1; // Indicate an error
    }
    double kml = mpg2kml(mpg); // Convert MPG to KML
    return (kml > 0) ? 100.0 / kml : -1; // Avoid division by zero
}
// Function to convert liters per 100 kilometers (LPH) to miles per gallon (MPG)
double lph2mpg(double lph) {
    if (lph <= 0) {
        printf("Error: LPH must be positive.\n");
        return -1;
    }

    return 235.214583 / lph;
}
