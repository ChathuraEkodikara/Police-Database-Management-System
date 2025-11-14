#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct CrimeRecord {
    char nic[13];
    char policeStation[50];
    char date[20];
    char name[50];
    char crime[100];
    int age;
};

void clearScreen() {
    printf("\033[H\033[J"); // clear the screen
}

void loadingAnimation() {
    printf("Processing");
    fflush(stdout);
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        sleep(1); // Wait for 1 second
    }
    printf("\n");
}

void inputData() {
    int numRecords;
    clearScreen();
    printf("Police Database Management System\n");
    printf("=================================\n");
    printf("Enter the number of crime records to input: ");
    if (scanf("%d", &numRecords) != 1) {
        printf("Invalid input. Please enter a valid number.\n");
        fflush(stdin); // Clear input buffer
        return;
    }

    struct CrimeRecord *records = (struct CrimeRecord *)malloc(numRecords * sizeof(struct CrimeRecord));

    for (int i = 0; i < numRecords; i++) {
        clearScreen();
        printf("Input Crime Records\n");
        printf("====================\n");
        printf("Enter details for record %d:\n", i + 1);

        // Input and validate NIC number
        printf("NIC Number (12 digits): ");
        scanf("%s", records[i].nic);

        if (strlen(records[i].nic) != 12) {
            printf("Invalid NIC number. NIC number should be 12 digits.\n");
            free(records);
            return;
        }

        // Input and validate age
        printf("Age (2 digits): ");
        if (scanf("%d", &records[i].age) != 1) {
            printf("Invalid input. Please enter a valid age as a number.\n");
            free(records);
            fflush(stdin);
            return;
        }

        // Input police station name
        printf("Police Station Name: ");
        scanf("%s", records[i].policeStation);

        // Input date
        printf("Date (MM/DD/YYYY): ");
        scanf("%s", records[i].date);

        // Check if NIC number already exists
        FILE *checkFile = fopen("crime_records.txt", "r");
        int nicExists = 0;

        if (checkFile != NULL) {
            struct CrimeRecord checkRecord;
            while (fread(&checkRecord, sizeof(struct CrimeRecord), 1, checkFile) == 1) {
                if (strcmp(checkRecord.nic, records[i].nic) == 0) { // Used strcmp for string comparison
                    printf("NIC Number already exists in the records.\n");
                    printf("Is this the same person? (y/n): ");
                    char choice;
                    scanf(" %c", &choice); // Read the choice to consume the newline character
                    if (choice == 'y' || choice == 'Y') {
                        nicExists = 1;
                        break;
                    }
                }
            }
            fclose(checkFile);
        }

        if (nicExists) {
            printf("Enter additional crime details for this person:\n");
            printf("Crime: ");
            scanf("%s", records[i].crime);
        } else {
            printf("Name: ");
            scanf("%s", records[i].name);
            printf("Crime: ");
            scanf("%s", records[i].crime);
        }

        clearScreen();
        printf("Saving Crime Records");
        loadingAnimation();
    }

    FILE *file = fopen("crime_records.txt", "a"); // Open the file in cmd mode
    if (file != NULL) {
        fwrite(records, sizeof(struct CrimeRecord), numRecords, file);
        fclose(file);
        clearScreen();
        printf("Crime records saved successfully.\n");
    } else {
        printf("Error: Unable to open the file for writing.\n");
    }

    free(records);
    sleep(2); // Wait for 2 seconds
}

void generateReport() {
    char searchNIC[13];
    clearScreen();
    printf("Police Department Management System\n");
    printf("=================================\n");
    printf("Enter NIC Number to generate a report: ");
    scanf("%s", searchNIC);

    FILE *inputFile = fopen("crime_records.txt", "r");
    FILE *outputFile = fopen("crime_report.txt", "w");

    if (inputFile == NULL || outputFile == NULL) {
        clearScreen();
        printf("Error: Unable to open files for reading/writing.\n");
        return;
    }

    fprintf(outputFile, "Crime Report for NIC: %s\n\n", searchNIC);
    fprintf(outputFile, "---------------------------------------------\n");

    struct CrimeRecord record;
    int found = 0;

    while (fread(&record, sizeof(struct CrimeRecord), 1, inputFile) == 1) {
        if (strcmp(record.nic, searchNIC) == 0) {
            fprintf(outputFile, "NIC Number: %s\n", record.nic);
            fprintf(outputFile, "Police Station Name: %s\n", record.policeStation);
            fprintf(outputFile, "Date: %s\n", record.date);
            fprintf(outputFile, "Name: %s\n", record.name);
            fprintf(outputFile, "Age: %d\n", record.age);
            fprintf(outputFile, "Crime: %s\n", record.crime);
            fprintf(outputFile, "---------------------------------------------\n");
            found = 1;
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    clearScreen();
    if (found) {
        printf("Report generated successfully. Check 'crime_report.txt'.\n");
    } else {
        printf("No records found for the provided NIC number.\n");
    }
    sleep(2); //  Wait for 2 seconds
}


void analyzeCrimes() {
    clearScreen();
    printf("Police Department Management System\n");
    printf("=================================\n");
    printf("Analyze Crimes\n");
    printf("========================\n");

    int choice;
    printf("Select an analysis criteria:\n");
    printf("1. Analyze crimes by age group\n");
    printf("2. Analyze crimes by police station\n");
    printf("3. Analyze crimes by crime type\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    FILE *file = fopen("crime_records.txt", "r");
    if (file == NULL) {
        clearScreen();
        printf("Error: Unable to open the file for reading.\n");
        return;
    }

    int ageGroupCount[5] = {0};
    int stationCount[5] = {0};
    int crimeTypeCount[5] = {0};
    int totalCrimes = 0;

    // Loop through the records and perform analysis
    struct CrimeRecord record;
    while (fread(&record, sizeof(struct CrimeRecord), 1, file) == 1) {
        totalCrimes++;

        // Analyze by age group
        if (record.age >= 0 && record.age <= 19) {
            ageGroupCount[0]++;
        } else if (record.age >= 20 && record.age <= 39) {
            ageGroupCount[1]++;
        } else if (record.age >= 40 && record.age <= 59) {
            ageGroupCount[2]++;
        } else if (record.age >= 60 && record.age <= 79) {
            ageGroupCount[3]++;
        } else {
            ageGroupCount[4]++;
        }

        // Analyze by police station
        if (strcmp(record.policeStation, "Colombo") == 0) {
            stationCount[0]++;
        } else if (strcmp(record.policeStation, "Kelaniya") == 0) {
            stationCount[1]++;
        } else if (strcmp(record.policeStation, "Kandy") == 0) {
            stationCount[2]++;
        } else if (strcmp(record.policeStation, "Galle") == 0) {
            stationCount[3]++;
        } else {
            stationCount[4]++;
        }

        // Analyze by crime type
        if (strcmp(record.crime, "Murder") == 0) {
            crimeTypeCount[0]++;
        } else if (strcmp(record.crime, "Assault") == 0) {
            crimeTypeCount[1]++;
        } else if (strcmp(record.crime, "Rape") == 0) {
            crimeTypeCount[2]++;
        } else if (strcmp(record.crime, "Robbery") == 0) {
            crimeTypeCount[3]++;
        } else {
            crimeTypeCount[4]++;
        }
    }

    fclose(file);

    // Display the analysis results based on user choice
    clearScreen();
    printf("Analysis Results\n");
    printf("================\n");

    if (choice == 1) {
        // Analyze by age group
        printf("Analysis by Age Group\n");
        printf("-----------------------\n");
        printf("Age Group\tNumber of Crimes\tPercentage\n");
        printf("----------------------------------------\n");
        for (int i = 0; i < 5; i++) {
            double percentage = (double)ageGroupCount[i] / totalCrimes * 100.0;
            printf("%d-%d\t\t%d\t\t%.2f%%\n", i * 20, i * 20 + 19, ageGroupCount[i], percentage);
        }
    } else if (choice == 2) {
        // Analyze by police station
        printf("Analysis by Police Station\n");
        printf("---------------------------\n");
        printf("Police Station\tNumber of Crimes\tPercentage\n");
        printf("----------------------------------------\n");
        char stationNames[5][50] = {"Colombo ", "Kelaniya", "Kandy   ", "Galle   ", "Other   "};
        for (int i = 0; i < 5; i++) {
            double percentage = (double)stationCount[i] / totalCrimes * 100.0;
            printf("%s\t\t%d\t\t%.2f%%\n", stationNames[i], stationCount[i], percentage);
        }
    } else if (choice == 3) {
        // Analyze by crime type
        printf("Analysis by Crime Type\n");
        printf("------------------------\n");
        printf("Crime Type\tNumber of Crimes\tPercentage\n");
        printf("----------------------------------------\n");
        char crimeTypes[5][100] = {"Murder ", "Assault", "Rape   ", "Robbery", "Other  "};
        for (int i = 0; i < 5; i++) {
            double percentage = (double)crimeTypeCount[i] / totalCrimes * 100.0;
            printf("%s\t\t%d\t\t%.2f%%\n", crimeTypes[i], crimeTypeCount[i], percentage);
        }
    } else {
        printf("Invalid choice. Please try again.\n");
    }

    printf("\nPress Enter to continue...");
    while (getchar() != '\n'); // Clear input buffer
    getchar(); // Wait for Enter key press
}

int main() {
    int choice;

    do {
        clearScreen();
        printf("\t\t\t\t\tWelcome to the Police Database Management System\n");
        printf("\t\t\t\t\t================================================\n");
        printf("\n\t\t\t\t\t\t1. Input Crime Data\n");
        printf("\t\t\t\t\t\t2. Generate Police Report\n");
        printf("\t\t\t\t\t\t3. Analyze Crimes\n");
        printf("\t\t\t\t\t\t4. Exit\n");
        printf("\n\t\t\t\t\tEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                inputData();
                break;
            case 2:
                generateReport();
                break;
            case 3:
                analyzeCrimes();
                break;
            case 4:
                clearScreen();
                printf("Exiting the program.\n");
                break;
            default:
                clearScreen();
                printf("Invalid choice. Please try again.\n");
                break;
        }

        if (choice != 4) {
            printf("\nPress Enter to continue...");
            while (getchar() != '\n'); // Clear input buffer
            getchar(); // Wait for Enter key press
        }
    } while (choice != 4);

    return 0;
}
