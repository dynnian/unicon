/* 
 * unicon.c
 *
 * Copyright 2024 Clay Gomera
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-only
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define VERSION 0.1

// Enumeration for each unit type
typedef enum {
    TEMPERATURE,
    LENGTH,
    TIME,
    MASS,
    DIGITAL
} UnitType;

// Enumeration for each unit
typedef enum {
    // Temperature units
    CELSIUS, FAHRENHEIT, KELVIN,
    // Length units
    METERS, CENTIMETERS, DECIMETERS, DECAMETERS, HECTOMETERS, KILOMETERS, MILLIMETERS, MILE, INCHES, FEET,
    // Time units
    SECONDS, MILLISECONDS, MINUTES, HOURS, DAYS, MONTHS, YEARS,
    // Mass units
    GRAMS, CENTIGRAMS, DECIGRAMS, DECAGRAMS, HECTOGRAMS, MILLIGRAMS, KILOGRAMS, POUNDS, OUNCES,
    // Digital storage units
    BYTES, KILOBYTES, MEGABYTES, GIGABYTES, TERABYTES, PETABYTES, EXABYTES
} Unit;

// Struct for the units table
typedef struct _UnitTable {
    UnitType type;
    Unit unit;
    const char *name;
    double conversion_factor;
} UnitTable;

// Units table with conversion factors
UnitTable unit_table[] = {
    // Temperature units
    {TEMPERATURE, CELSIUS, "celsius", 0},
    {TEMPERATURE, FAHRENHEIT, "fahrenheit", 0},
    {TEMPERATURE, KELVIN, "kelvin", 0},
    // Length units
    {LENGTH, METERS, "meters", 1.0},
    {LENGTH, CENTIMETERS, "centimeters", 100.0},
    {LENGTH, DECIMETERS, "decimeters", 10.0},
    {LENGTH, DECAMETERS, "decameters", 0.1},
    {LENGTH, HECTOMETERS, "hectometers", 0.01},
    {LENGTH, KILOMETERS, "kilometers", 0.001},
    {LENGTH, MILLIMETERS, "millimeters", 1000.0},
    {LENGTH, MILE, "miles", 0.000621371},
    {LENGTH, INCHES, "inches", 39.3701},
    {LENGTH, FEET, "feet", 3.28084},
    // Time units
    {TIME, SECONDS, "seconds", 1.0},
    {TIME, MILLISECONDS, "milliseconds", 1000.0},
    {TIME, MINUTES, "minutes", 1.0 / 60.0},
    {TIME, HOURS, "hours", 1.0 / 3600.0},
    {TIME, DAYS, "days", 1.0 / 86400.0},
    {TIME, MONTHS, "months", 1.0 / 2592000.0},
    {TIME, YEARS, "years", 1.0 / 31536000.0},
    // Mass units
    {MASS, GRAMS, "grams", 1.0},
    {MASS, CENTIGRAMS, "centigrams", 100.0},
    {MASS, DECIGRAMS, "decigrams", 10.0},
    {MASS, DECAGRAMS, "decagrams", 0.1},
    {MASS, HECTOGRAMS, "hectograms", 0.01},
    {MASS, MILLIGRAMS, "milligrams", 1000.0},
    {MASS, KILOGRAMS, "kilograms", 0.001},
    {MASS, POUNDS, "pounds", 0.00220462},
    {MASS, OUNCES, "ounces", 0.03527396},
    // Digital storage units
    {DIGITAL, BYTES, "bytes", 1.0},
    {DIGITAL, KILOBYTES, "kilobytes", 1.0 / 1024.0},
    {DIGITAL, MEGABYTES, "megabytes", 1.0 / 1048576.0},
    {DIGITAL, GIGABYTES, "gigabytes", 1.0 / 1073741824.0},
    {DIGITAL, TERABYTES, "terabytes", 1.0 / 1099511627776.0},
    {DIGITAL, PETABYTES, "petabytes", 1.0 / 1125899906842624.0},
    {DIGITAL, EXABYTES, "exabytes", 1.0 / 1152921504606846976.0},
};

// Function prototypes
bool isNumeric(const char *str);
double convertUnit(double value, Unit from, Unit to);
Unit matchArgument(const char *arg);
void displayHelp();
void displayVersion();
void displayUnits();

int main(int argc, char **argv) {
    int opt;
    int round_places = -1; // Default value for rounding places
    
    // Check if there are no command-line arguments
    if (argc == 1) {
        displayHelp();
        return 0;
    }

    static const char* const short_options = "r:shv";
    static struct option long_options[] = {
        {"round", required_argument, 0, 'r'},
        {"show", no_argument, 0, 's'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {NULL, 0, NULL, 0}
    };

    // Command line option parsing
    while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (opt) {
            case 'r':
                round_places = atoi(optarg);
                break;
            case 'h':
                displayHelp();
                return 0;
            case 'v':
                displayVersion();
                return 0;
            case 's':
                displayUnits();
                return 0;
            default:
                fprintf(stderr, "Use '-h, --help' for help.\n");
                return 1;
        }
    }

    // Check if there are enough arguments
    if (optind + 5 != argc) {
        printf("Invalid command format. Please provide the correct number of arguments.\n");
        displayHelp();
        return 1;
    }

    // Extract the value to convert
    if (!isNumeric(argv[optind])) {
        printf("Invalid value provided. Please provide a valid numeric value.\n");
        return 1;
    }

    double value = atof(argv[optind]);

    // Find the positions of "from" and "to" keywords
    int fromPos = -1;
    int toPos = -1;
    for (int i = optind + 1; i < argc - 1; i++) {
        if (strcasecmp(argv[i], "from") == 0) {
            fromPos = i + 1;
        } else if (strcasecmp(argv[i], "to") == 0) {
            toPos = i + 1;
        }
    }

    // Check if both "from" and "to" keywords were found
    if (fromPos == -1 || toPos == -1) {
        printf("Invalid command format. Please provide both 'from' and 'to' units.\n");
        displayHelp();
        return 1;
    }

    // Find the matching units
    Unit from = matchArgument(argv[fromPos]);
    Unit to = matchArgument(argv[toPos]);

    // Check if both units are valid
    if (from == -1 || to == -1) {
        printf("Invalid units provided. Please provide valid units.\n");
        displayHelp();
        return 1;
    }

    // Convert the value
    double result;
    if (from < to) {
        result = convertUnit(value, from, to);
    } else {
        result = convertUnit(value, to, from);
    }
    
    // Round the result if round_places is set
    if (round_places >= 0) {
        result = round(result * pow(10, round_places)) / pow(10, round_places);
    }
    
    // Determine the number of decimal places for formatting
    int decimal_places = (round_places >= 0) ? round_places : 2;
    
    // Display the result with the appropriate decimal places
    printf("%.*f %s = %.*f %s\n", decimal_places, value, unit_table[from].name, decimal_places, result, unit_table[to].name);
    
    return 0;
}

// Function to check if a string is a valid number
bool isNumeric(const char *str) {
    if (str == NULL || str[0] == '\0') {
        return false;
    }
    int i = 0;
    if (str[0] == '-' || str[0] == '+') {
        i++;
    }
    int dotCount = 0;
    int len = strlen(str);
    for (; i < len; i++) {
        if (!isdigit(str[i])) {
            if (str[i] == '.' && dotCount == 0) {
                dotCount++;
            } else {
                return false;
            }
        }
    }
    return true;
}

// Function to convert a value from one unit to another
double convertUnit(double value, Unit from, Unit to) {
    if (from == to) {
        return value;
    }

    // Temperatures need to be handled by formula
    switch (from) {
        case CELSIUS:
            switch (to) {
                case FAHRENHEIT:
                    return (value * 9/5) + 32;
                case KELVIN:
                    return value + 273.15;
                default:
                    break;
            }
        case FAHRENHEIT:
            switch (to) {
                case CELSIUS:
                    return (value - 32) * 5/9;
                case KELVIN:
                    return (value - 32) * 5/9 + 273.15;
                default:
                    break;
            }
        case KELVIN:
            switch (to) {
                case CELSIUS:
                    return value - 273.15;
                case FAHRENHEIT:
                    return (value - 273.15) * 9/5 + 32;
                default:
                    break;
            }
        default:
            if (unit_table[from].type != unit_table[to].type) {
                printf("Cannot convert between different unit types.\n");
                exit(1);
            }
            double factor = unit_table[from].conversion_factor / unit_table[to].conversion_factor;
            return value * factor;
    }
}

// Function to match an argument to a unit
Unit matchArgument(const char *arg) {
    for (int i = 0; i < sizeof(unit_table) / sizeof(unit_table[0]); i++) {
        if (strcasecmp(arg, unit_table[i].name) == 0) {
            return unit_table[i].unit;
        }
    }
    return -1;
}

// Function to display the help message
void displayHelp() {
    printf("Usage: unicon [OPTIONS] VALUE from <UNIT> to <UNIT>\n");
    printf("Convert between various units.\n\n");
    printf("Options:\n");
    printf("\t-r, --round=PLACES   Round the result to the specified number of decimal places.\n");
    printf("\t-s, --show           Show the full table of supported units.\n");
    printf("\t-h, --help           Display this help message and exit.\n");
    printf("\t-v, --version        Display version information and exit.\n");
}

void displayUnits() {
    printf("Supported units:\n");
    printf("TEMPERATURE:\n");
    printf("\t- Celsius\n");
    printf("\t- Fahrenheit\n");
    printf("\t- Kelvin\n");
    printf("LENGTH:\n");
    printf("\t- Millimeters\n");
    printf("\t- Centimeters\n");
    printf("\t- Decimeters\n");
    printf("\t- Meters\n");
    printf("\t- Decameters\n");
    printf("\t- Hectometers\n");
    printf("\t- Kilometers\n");
    printf("\t- Miles\n");
    printf("\t- Inches\n");
    printf("\t- Feet\n");
    printf("TIME:\n");
    printf("\t- Milliseconds\n");
    printf("\t- Seconds\n");
    printf("\t- Minutes\n");
    printf("\t- Hours\n");
    printf("\t- Days\n");
    printf("\t- Months\n");
    printf("\t- Years\n");
    printf("MASS:\n");
    printf("\t- Milligrams\n");
    printf("\t- Centigrams\n");
    printf("\t- Decigrams\n");
    printf("\t- Grams\n");
    printf("\t- Decagrams\n");
    printf("\t- Hectograms\n");
    printf("\t- Kilograms\n");
    printf("\t- Pounds\n");
    printf("\t- Ounces\n");
    printf("DIGITAL STORAGE:\n");
    printf("\t- Bytes\n");
    printf("\t- Kilobytes\n");
    printf("\t- Megabytes\n");
    printf("\t- Gigabytes\n");
    printf("\t- Terabytes\n");
    printf("\t- Petabytes\n");
    printf("\t- Exabytes\n");
}

// Function to display the version information
void displayVersion() {
    printf("unicon v%.1f\n", VERSION);
}
