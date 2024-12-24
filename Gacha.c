#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>

int legendCount = 0;
int epicCount = 0;

// ANSI color codes
#define COLOR_RESET "\x1B[0m"
#define COLOR_YELLOW "\x1B[33m"
#define COLOR_PURPLE "\x1B[35m"
#define COLOR_BLUE "\x1B[34m"

typedef struct {
    char *name;
    int rarity; 
} Item;

typedef struct {
    char *name;
} Character;

// 4-star characters
Character fourStarCharacters[] = {
    {"March 7th"}, {"Dan Heng"}, {"Arlan"}, {"Asta"},
    {"Herta"}, {"Natasha"}, {"Serval"}, {"Pela"},
    {"Sampo"}, {"Hook"}, {"Sushang"}, {"Qingque"},
    {"Tingyun"}
};

// 5-star characters
Character fiveStarCharacters[] = {
    {"Yanqing"}, {"Gepard"}, {"Bronya"}, {"Welt"},
    {"Himeko"}, {"Clara"}, {"Bailu"}
};

// 3-star weapons (Light Cones)
Item threeStarLightCones[] = {
    {"Adversarial", 3}, {"Amber", 3}, {"Arrows", 3}, {"Chorus", 3},
    {"Collapsing Sky", 3}, {"Cornucopia", 3}, {"Darting Arrow", 3}, {"Data Bank", 3},
    {"Defense", 3}, {"Fine Fruit", 3}, {"Hidden Shadow", 3}, {"Loop", 3},
    {"Mediation", 3}, {"Meshing Cogs", 3}, {"Multiplication", 3}, {"Mutual Demise", 3},
    {"Passkey", 3}, {"Pioneering", 3}, {"Sagacity", 3}, {"Shattered Home", 3}, {"Void", 3}
};

// 4-star weapons (Light Cones)
Item fourStarLightCones[] = {
    {"A Secret Vow", 4}, {"Concert for Two", 4}, {"Dance! Dance! Dance!", 4},
    {"Day One of My New Life", 4}, {"Eyes of the Prey", 4}, {"Geniuses' Repose", 4},
    {"Good Night and Sleep Well", 4}, {"Indelible Promise", 4}, {"Landau's Choice", 4},
    {"Make the World Clamor", 4}, {"Memories of the Past", 4}, {"Only Silence Remains", 4},
    {"Perfect Timing", 4}, {"Planetary Rendezvous", 4}, {"Post-Op Conversation", 4},
    {"Resolution Shines As Pearls of Sweat", 4}, {"Shared Feeling", 4},
    {"Subscribe for More!", 4}, {"Swordplay", 4}, {"The Birth of the Self", 4},
    {"The Moles Welcome You", 4}, {"Trend of the Universal Market", 4},
    {"Under the Blue Sky", 4}
};

// 5-star weapons (Light Cones)
Item fiveStarLightCones[] = {
    {"But the Battle Isn't Over", 5}, {"In the Name of the World", 5},
    {"Moment of Victory", 5}, {"Night on the Milky Way", 5},
    {"Sleep Like the Dead", 5}, {"Something Irreplaceable", 5},
    {"Time Waits for No One", 5}
};

Item gacha() {
    legendCount++;
    epicCount++;
    int randomValue;

    if(epicCount == 10) {
        randomValue = 9939;
        epicCount = 0;
    }
    else if(legendCount == 90) {
        randomValue = 9999;
        legendCount = 0;    
    }
    else {
        randomValue = rand() % 10000;
    }

    if (randomValue < 9430) {
        return threeStarLightCones[rand() % (sizeof(threeStarLightCones) / sizeof(threeStarLightCones[0]))];
    } else if (randomValue < 9940) {
        if (rand() % 2 == 0) {
            Character selectedChar = fourStarCharacters[rand() % (sizeof(fourStarCharacters) / sizeof(fourStarCharacters[0]))];
            Item characterItem = {"\033[33m****\033[0m Character", 4};
            printf("- %s (%s)\n", selectedChar.name, characterItem.name);
            return characterItem;
        } else {
            // Return 4-star Light Cone
            return fourStarLightCones[rand() % (sizeof(fourStarLightCones) / sizeof(fourStarLightCones[0]))];
        }
    } else {
        // 0.6% chance, random between 5 Star Character and 5 Star Light Cone
        if (rand() % 2 == 0) {
            // Return 5-star character
            Character selectedChar = fiveStarCharacters[rand() % (sizeof(fiveStarCharacters) / sizeof(fiveStarCharacters[0]))];
            Item characterItem = {"\033[33m*****\033[0m Character", 5};
            printf("- %s (%s)\n", selectedChar.name, characterItem.name);
            return characterItem;
        } else {
            // Return 5-star Light Cone
            return fiveStarLightCones[rand() % (sizeof(fiveStarLightCones) / sizeof(fiveStarLightCones[0]))];
        }
    }
}

// int main() {
//     srand(time(NULL));
//     int choice;
//     Item pt;

//     do {
//         puts("============");
//         puts("Gacha System");
//         puts("============");
//         puts("1. 1");
//         puts("2. 10");
//         puts("3. Exit");
//         printf("How many do you want to pull?: ");
//         scanf("%d", &choice);

//         switch(choice) {
//             case 1: {
//                 printf("You Pulled: ");
//                 pt = gacha();
//                 if (strcmp(pt.name, "\033[33m****\033[0m Character") != 0 && strcmp(pt.name, "\033[33m*****\033[0m Character") != 0) {
// 					printf("- %s (\033[33m%s\033[0m Light Cone)\n", pt.name, getStar(pt.rarity));
//                 }
//                 break;
//             }   

//             case 2: {
// 				printf("You Pulled:\n");
//                 for(int i = 0; i < 10; i++) {

//                     pt = gacha();
//                     if (strcmp(pt.name, "\033[33m****\033[0m Character") != 0 && strcmp(pt.name, "\033[33m*****\033[0m Character") != 0) {
//                         printf("- %s (\033[33m%s\033[0m Light Cone)\n", pt.name, getStar(pt.rarity));
//                     }
//                 }
//                 break;
//             }

//             case 3: {
//                 exit(0);
//                 break;
//             }
//         }

//         if(choice != 3) {
//             printf("Press Any Key To Continue ...");
//             getch();
//             fflush(stdin);
//             system("cls");
//         }

//     } while(choice != 3);

//     return 0;
// }
