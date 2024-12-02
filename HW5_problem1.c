/**
 * HW5_problem1.c - Source file for ECE 361 Homework 5 Problem 1
 *
 * @file:		HW5_problem1.c
 * @author:		Keenan Mondragon (kmondra2@pdx.edu)
 * @date:		12/01/2024
 * @version:	1.0
 *
 * This file tests my AHT20_BST source and header files by simulating the retrieval of
 * sensor readings, storing them in a binary search tree, and accessing them at user request.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "iom361_r2.h"
#include "AHT20_BST.h"
#include <time.h>


int main () {


    // greet the user and display the working directory for the application
    printf("ECE 361 - Mock Temperature and Humidity readings for Oregon 2023 - Keenan Mondragon (kmondra2@pdx.edu)\n");
    char *buf = getcwd(NULL, 0);    // allocates a buffer large enough to hold the path

    if (buf == NULL) {
        perror("getcwd");
        printf("Could not display the path\n");
    }
    else {
        printf("Current working directory: %s", buf);
        free(buf);
    }

    // get user input
    int month, day, year, days;
    printf("\nPlease enter the begin date of your query (MM DD YYYY), followed by the number of days to query: ");
    scanf("%d" "%d" "%d" "%d",&month, &day, &year, &days);

    int daysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (day > daysPerMonth[month - 1]) {
        printf("ERROR (main): The date you entered is invalid.");
        return 1;
    }
    if (year > 2024 || year < 2000) {
        printf("ERROR (main): The year you entered is invalid, please enter a year between 2000 and 2024.");
        return 1;
    }

    // convert user input into time_t
    struct tm date = {0};
    date.tm_year = year-1900;
    date.tm_mon = month-1;
    date.tm_mday = day;

    time_t result = mktime(&date);

    //populate BST with AHT20 readings beginning on the specified date, for the specified number of days
    BSTptr_t mainTree = populateBST(result, days);

    //display all sensor readings in order
    printf("\nThe sensor readings for the month of interest are: \n");
    inorderDisplay(mainTree->root);
    printf("\n");


    //get user input to find specific AHT20 readings
    char input;
    printf("Begin: Search for AHT20 readings for a specific data. Press 'd' to exit.\n");

    while (1) {
        //clear the buffer
        while ((input = getchar()) != '\n' && input != EOF);

        //gather input
        printf("Please enter the search date in the format (MM DD YYYY): ");
        if (scanf("%d %d %d", &month, &day, &year) != 3) {
            printf("Thanks for running Keenan's HW#5 application.\n");
            return 0;
        }

        struct tm date = {0};
        date.tm_year = year - 1900;
        date.tm_mon = month - 1;
        date.tm_mday = day;

        time_t result = mktime(&date);
        if (result == -1) {
            printf("Failed to convert the given date. Please try again.\n");
            continue;
        }

        // Search the BST, display the results
        searchBST(mainTree, result);
    }


}