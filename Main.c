#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <unistd.h> 

#include "Song.c"
#include "Gacha.c"
#include "RBT.c"
#include "Light_Cone_RBT.c"
#include "Relic RBT.c"
#include "Planar Ornaments RBT.c"
#include "TextArt.c"
int start = 0;

int getConsoleWidth();
int getConsoleHeight();
void cursorPositionCenter();
void printWithAnimation();
void flickeringText();
void clearLine();

void menuUpdate();
void menuView();
void menuInsert();
void menuDelete();
void menuExit();
void menuGacha();

void subMenuViewCharacter();
void subMenuViewType();
void subMenuViewPath();
void subMenuViewStar();
void subMenuViewBack();
void subMenuView();

void displayMenuPath();
void navigateMenuPath();

void displayMenuType();
void navigateMenuType();

void displayMenuStar();
void navigateMenuStar();

void displayMenuUpdate();
void navigateUpdate()
;
void displayMenuDetails();
void navigateDetails();

void displayMenuGacha();
void navigateMenuGacha();

const char *menuGachas[] = {
	"One Pull",
	"Ten Pull",
	"Back"
};

const char *menuDetails[] = {
	"Image",
	"Light Cone",
	"Relic",
	"Planar Ornament",
	"Back"
};

const char *menuViewUpdate[] = {
	"Name",
	"Picture Name",
	"Star Rating",
	"Path",
	"Combat Type",
	"Description",
	"Back"
};

const char *menuPath[] = {
	"Destruction",
	"The Hunt",
	"Erudition",
	"Harmony",
	"Nihility",
	"Preservation",
	"Abundance",
	"Back"
};

const char *menuType[] = {
	"Physical",
	"Fire",
	"Ice",
	"Lightning",
	"Wind",
	"Quantum",
	"Imaginary",
	"Back"
};

const char *menuStar[] = {
	"4",
	"5",
	"Back"
};

const char *typeColor[][2] = {
	{"Lightning", "35"},
	{"Physical", "37"},
	{"Fire", "31"},
	{"Imaginary", "33"},
	{"Wind", "32"},
	{"Quantum", "35"},
	{"Ice", "34"},
};

int validatePath(char *s) {
	for(int i = 0; i < 7; i++) {
		if(strcmp(s, menuPath[i]) == 0){
			return 0;
		}
	}
	return 1;
}

int validateType(char *s) {
	for(int i = 0; i < 7; i++) {
		if(strcmp(s, menuType[i]) == 0){
			return 0;
		}
	}
	return 1;
}

const char *getFileExtension(const char *fileName) {
    const char *dot = strrchr(fileName, '.');

    if (!dot || dot == fileName) {
        return "";
    }

    return dot + 1;
}

const char* getColorCode(const char *type) {
    for (int i = 0; i < sizeof(typeColor) / sizeof(typeColor[0]); i++) {
        if (strcmp(typeColor[i][0], type) == 0) {
            return typeColor[i][1];
        }
    }
	
	return "37";
}

void printCard(struct Node *data) {
    puts("====================================================================");
    printf("|%36s%30s|\n", data->data.name, "");
    puts("====================================================================");
    printf("| %-64s |\n", "");
    printf("| %-15s : \033[33m%-46s\033[0m |\n", "Rarity", getStar(data->data.star));
    printf("| %-15s : %-46s |\n", "Path", data->data.path);
    printf("| %-15s : \033[%sm%-46s\033[0m |\n", "Combat Type", getColorCode(data->data.type), data->data.type);
    printf("| %-15s : %-46s |\n", "Description", "");

    int width = 63; // Adjust as needed for your terminal or card layout
    int start = 0;
    char *description = malloc(strlen(data->data.description) + 1);
    strcpy(description, data->data.description);

    int len = strlen(description);
    while (start < len) {
        int lineLength = width;
        if (start + lineLength > len) {
            lineLength = len - start;
        } else {
            while (lineLength > 0 && description[start + lineLength - 1] != ' ') {
                lineLength--;
            }
        }

        char buffer[64] = {0};
        strncpy(buffer, description + start, lineLength);
        printf("|   %-63s|\n", buffer);

        start += lineLength;
        while (start < len && description[start] == ' ') {
            start++;
        }
    }

    while (start < len) {
        printf("|   %-63s|\n", "");
        start += width;
    }

    puts("====================================================================");

    free(description);
}

// Function to get the console width dynamically
int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width;

    // Get console screen buffer info
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    } else {
        width = 80; // Default width if unable to get information
    }

    return width;
}

// Function to get the console height dynamically
int getConsoleHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int height;

    // Get console screen buffer info
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        height = 25; // Default height if unable to get information
    }

    return height;
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void showCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);		
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void cursorPositionCenter(int lineNumber, int textWidth){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    int consoleWidth = csbi.dwSize.X;

    // Calculate the center position
    COORD coord = { (consoleWidth - textWidth) / 2, lineNumber }; 
    SetConsoleCursorPosition(hConsole, coord);
}

void clearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = {0, 0}; 
	SetConsoleCursorPosition(hConsole, coordScreen);
}

void printWithAnimation(const char *text, int delay) {
    while (*text) {
        putchar(*text++);
        fflush(stdout); // Ensure the output is displayed immediately
        Sleep(delay);   // Delay in milliseconds
    }
}

void flickeringText(int x, int y, char* text) {
    hideCursor();
    int textLength = strlen(text);
    
    do {
        for (int i = 0; i < 3; i++) { // Change 3 to the desired number of flickers
            cursorPositionCenter(x, y);
            printf("%s", text);
            Sleep(1000); // Adjust the sleep time for faster/slower flickering
            cursorPositionCenter(x, y);
            for (int j = 0; j < textLength; j++) {
                putchar(' '); // Print spaces to clear the text
            }
            Sleep(1000);
        }
        cursorPositionCenter(x, y);
        printf("%s", text); // Final display of text
        fflush(stdout); // Ensure the final display is shown
        
        // Check if the user pressed a valid key (W, S, A, D, w, s, a, d) to exit the loop
        
    } while (getchar() != '\n'); // Keep looping until the user presses Enter
    hideCursor();
}

void clearLine() {
    printf("\033[2K\r"); // ANSI escape sequence to clear the line
    fflush(stdout); // Flush the output to ensure it's immediately displayed
}

void printTitle() {
	int consoleWidth = getConsoleWidth();
	int consoleHeight = getConsoleHeight();
    int padding = (consoleWidth - 50) / 2; // 50 is the width of the title
    int padding2 = (consoleHeight - 15) / 2; // 15 title height
	
	for(int i = 0; i < padding2; i++)
        printf("\n");
    for (int i = 0; i < padding; i++)
        putchar(' ');
	printf(" ██╗  ██╗ ██████╗ ███╗   ██╗██╗  ██╗ █████╗ ██╗\n");
	for (int i = 0; i < padding; i++)
        putchar(' ');					
	printf(" ██║  ██║██╔═══██╗████╗  ██║██║ ██╔╝██╔══██╗██║\n");
	for (int i = 0; i < padding; i++)
        putchar(' ');	
	printf(" ███████║██║   ██║██╔██╗ ██║█████╔╝ ███████║██║\n");
	for (int i = 0; i < padding; i++)
        putchar(' ');
	printf(" ██╔══██║██║   ██║██║╚██╗██║██╔═██╗ ██╔══██║██║\n");
	for (int i = 0; i < padding; i++)
        putchar(' ');	
	printf(" ██║  ██║╚██████╔╝██║ ╚████║██║  ██╗██║  ██║██║\n");
	for (int i = 0; i < padding; i++)
        putchar(' ');
	printf(" ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝\n");
	for (int i = 0; i < padding; i++)
        putchar(' ');
	
	printf("\n");
	
	for (int i = 8; i < padding; i++)
        putchar(' ');
	printf(" ███████╗████████╗ █████╗ ██████╗     ██████╗  █████╗ ██╗██╗\n");
	for (int i = 8; i < padding; i++)
        putchar(' ');
	printf(" ██╔════╝╚══██╔══╝██╔══██╗██╔══██╗    ██╔══██╗██╔══██╗██║██║\n");
	for (int i = 8; i < padding; i++)
        putchar(' ');
	printf(" ███████╗   ██║   ███████║██████╔╝    ██████╔╝███████║██║██║\n");
	for (int i = 8; i < padding; i++)
        putchar(' ');
	printf(" ╚════██║   ██║   ██╔══██║██╔══██╗    ██╔══██╗██╔══██║██║██║\n");
	for (int i = 8; i < padding; i++)
        putchar(' ');
	printf(" ███████║   ██║   ██║  ██║██║  ██║    ██║  ██║██║  ██║██║███████╗\n");
	for (int i = 8; i < padding; i++)
        putchar(' ');
	printf(" ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝    ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚══════╝\n");
	for (int i = 8; i < padding; i++)
        putchar(' ');
}

void menu() {
	char choice;
	char validation;
	const char *message = "----PLEASE PRESS ENTER TO CONTINUE----\n";
	int len = strlen(message);
	const char* subMenus[] = {
    "\t\t   \033[0m->[View]",
    "\t\t   \033[0m->[Gacha]",
    "\t\t   \033[0m->[Update]",
    "\t\t   \033[0m->[Insert]",
    "\t\t   \033[0m->[Delete]",
    "\t\t   \033[0m->[EXIT]",
	};
	
	int numSubMenus = sizeof(subMenus) / sizeof(subMenus[0]);
	
	do {
		system("cls");
	
		if(!start) {
			printTitle();
			cursorPositionCenter(32,len);
			printWithAnimation(message, 60);
			
			validation =  getchar();
		   
			while(validation != '\n'){
				validation = getchar();
				
			}
			cursorPositionCenter(32,len);
			clearLine();
			cursorPositionCenter(32,len);
			printWithAnimation("----[Use W A S D to Navigate]----", 60);
			start = 1;
		} 
		else {
			printTitle();	
			cursorPositionCenter(32,46);
			printf("----[Use W A S D to Navigate]----");
		}
		
		for (int i = 0; i < numSubMenus; i++) {
			cursorPositionCenter(34 + i , 48);
	    	printf("%s\n", subMenus[i]);
		}
		
		choice = getch();
		while (choice != 'W' && choice != 'w' && choice != 'S' && choice != 's' && choice != 'A' && choice != 'a' && choice != 'D' && choice != 'd'){
			choice = getch();
		}

	    switch(choice) {
			case 'W':
			case 'w':
			case 'S':
			case 's':
			case 'A':
			case 'a':
			case 'D':
			case 'd':
			{
				menuView();
				break;
			}
		}
		
	} while(1);
}

int main() {
    system("chcp 65001");
    system("cls");
	hideCursor();
	insertData();
	insertLC();
	insertR();
	insertP();
	sound();
	menu();

    return 0;
}

void menuExit() {
	char choice;
	clearScreen();	
	
	const char* subMenus[] = {
    "\t\t   \033[0m->[View]",
    "\t\t   \033[0m->[Gacha]",
    "\t\t   \033[0m->[Update]",
    "\t\t   \033[0m->[Insert]",
    "\t\t   \033[0m->[Delete]",
    "\t\t   \033[0;34m->[EXIT]\033[0m",
    };
    	
    int numSubMenus = sizeof(subMenus) / sizeof(subMenus[0]);
		
    for (int i = 0; i < numSubMenus; i++) {
    	cursorPositionCenter(34 + i ,48);
        printf("%s\n", subMenus[i]);
    }
    
	
	choice = getch();
	while (choice != 'W' && choice != 'w' && choice != 'S' && choice != 's' && choice != '\n' && choice != '\r'){
		choice = getch();
	}

	if(choice == '\n' || choice == '\r'){
		printf("Exiting the Program\n");	
		exit(0);
	}

	if (choice == 'W' || choice == 'w') menuDelete();
	menuView();
}

void menuDelete() {
	char choice;

	clearScreen();
	
	const char* subMenus[] = {
    "\t\t   \033[0m->[View]",
    "\t\t   \033[0m->[Gacha]",
    "\t\t   \033[0m->[Update]",
    "\t\t   \033[0m->[Insert]",
    "\t\t   \033[0;34m->[Delete]",
    "\t\t   \033[0m->[EXIT]",
    };
    	
    int numSubMenus = sizeof(subMenus) / sizeof(subMenus[0]);
		
    for (int i = 0; i < numSubMenus; i++) {
    	cursorPositionCenter(34 + i ,48);
        printf("%s\n", subMenus[i]);
    }
	
	choice = getch();
	while (choice != 'W' && choice != 'w' && choice != 'W' && choice != 's' && choice != '\n' && choice != '\r'){
		choice = getch();
	}

	if(choice == '\n' || choice == '\r'){
		system("cls");
		char string[100];
		viewCharacters(NULL);

		showCursor();
		printf("What character do you want to \033[1;31mDelete\033[0m: ");
		scanf(" %[^\n]", string);
		hideCursor();

		struct Node *nodeToDelete = search(root, sumOfAscii(string));
		if(nodeToDelete) {
			deleteNode(nodeToDelete);
			puts("Delete Succesfull !\n");
		}
		else puts ("Data Not Found!\n");
		system("pause");
		return;
	}

	if (choice == 'W' || choice == 'w') menuInsert();
	else menuExit();
}

void menuInsert() {
	char choice;
	clearScreen();	
	
	const char* subMenus[] = {
    "\t\t   \033[0m->[View]",
    "\t\t   \033[0m->[Gacha]",
    "\t\t   \033[0m->[Update]",
    "\t\t   \033[0;34m->[Insert]",
    "\t\t   \033[0m->[Delete]",
    "\t\t   \033[0m->[EXIT]",
    };
    	
    int numSubMenus = sizeof(subMenus) / sizeof(subMenus[0]);
		
    for (int i = 0; i < numSubMenus; i++) {
    	cursorPositionCenter(34 + i ,48);
        printf("%s\n", subMenus[i]);
    }
    	
	choice = getch();
	while (choice != 'W' && choice != 'w' && choice != 'S' && choice != 's' && choice != '\n' && choice != '\r'){
		choice = getch();
	}

	if(choice == '\n' || choice == '\r'){
		system("cls");
		struct character ch;
		int count;
		showCursor();
		puts("--==Insert New Character==--");
		do {
			printf("Name[1-100 characters]: ");
			scanf(" %[^\n]", ch.name);
		} while (strlen(ch.name) < 1 || strlen(ch.name) > 100); 

		do {
			printf("Picture Name[1-100 characters][.bmp]: ");
			scanf(" %[^\n]", ch.name_gambar);
		} while (strlen(ch.name_gambar) < 1 || strlen(ch.name_gambar) > 100 || strcmp(getFileExtension(ch.name_gambar), "bmp") != 0);

		do {
			printf("Star Rating[1-5]: ");
			count = scanf("%d", &ch.star);
			fflush(stdin);
		} while(ch.star < 1 || ch.star > 5 || !count);

		do {
			printf("Path[Destruction|The Hunt|Erudition|Harmony|Nihility|Preservation|Abundance]: ");
			scanf(" %[^\n]", ch.path);
		} while(validatePath(ch.path));

		do {
			printf("Type[Physical|Fire|Ice|Lightning|Wind|Quantum|Imaginary]: ");
			scanf(" %[^\n]", ch.type);
		} while(validateType(ch.type));

		do {
			printf("Description[1-10.000 characters]: ");
			scanf(" %[^\n]", ch.description);
		} while (strlen(ch.description) < 1 || strlen(ch.description) > 10000);

		insert(ch);
		puts("Insert Succesfull!\n");
		hideCursor();
		system("pause");
		return;
	}

	if (choice == 'W' || choice == 'w') menuUpdate();
	else menuDelete();
}

void menuUpdate() {
	char choice;
	clearScreen();	
	
	
	const char* subMenus[] = {
    "\t\t   \033[0m->[View]",
    "\t\t   \033[0m->[Gacha]",
    "\t\t   \033[0;34m->[Update]",
    "\t\t   \033[0m->[Insert]",
    "\t\t   \033[0m->[Delete]",
    "\t\t   \033[0m->[EXIT]",
    };
    	
    int numSubMenus = sizeof(subMenus) / sizeof(subMenus[0]);
		
    for (int i = 0; i < numSubMenus; i++) {
    	cursorPositionCenter(34 + i ,48);
        printf("%s\n", subMenus[i]);
    }
    	
	choice = getch();
	while (choice != 'W' && choice != 'w' && choice != 'S' && choice != 's' && choice != '\n' && choice != '\r'){
		choice = getch();
	}

	if(choice == '\n' || choice == '\r'){
		system("cls");
		char string[100];
		viewCharacters(NULL);

		showCursor();
		printf("What character do you want to \033[0;33mUpdate\033[0m: ");
		scanf(" %[^\n]", string);
		hideCursor();

		struct Node *nodeToUpdate = search(root, sumOfAscii(string));
		if(nodeToUpdate) {
			system("cls");
			navigateUpdate(nodeToUpdate);
			puts("Update Succesfull !\n");
		}
		else puts ("Data Not Found!\n");
		system("pause");
		return;
	}

	if (choice == 'W' || choice == 'w') menuGacha();
	else menuInsert();
}

void subMenuViewPath() {
	int c;
	clearScreen();

	printf("--==View Menu==--\n");	
	printf("[1] View All\n");
	printf("[2] View Character\n");
	printf("\033[0;34m[3] View By Path\033[0m\n");
	printf("[4] View By Star\n");
	printf("[5] View By Type\n");
	printf("[0] Back\n");
	
	c = getch();
	while (c != 'W' && c != 'w' && c != 'S' && c != 's' && c != '\n' && c != '\r'){
		c = getch();
	}

    switch(c) {
    	case '\n':
    	case '\r':
		{
			system("cls");
			navigateMenuPath();
			break;
		}
		case 'W':
		case 'w':
		{
			subMenuViewCharacter();
			break;
		}
		case 'S':
		case 's':
		{
			subMenuViewStar();
			break;
		}
	}
}

void subMenuViewBack() {
	int c;
	clearScreen();

	printf("--==View Menu==--\n");	
	printf("[1] View All\n");
	printf("[2] View Character\n");
	printf("[3] View By Path\n");
	printf("[4] View By Star\n");
	printf("[5] View By Type\n");
	printf("\033[0;34m[0] Back\033[0m\n");
	
	c = getch();
	while (c != 'W' && c != 'w' && c != 'S' && c != 's' && c != '\n' && c != '\r'){
		c = getch();
	}

    switch(c) {
    	case '\n':
    	case '\r':
		{
			break;
		}
		case 'W':
		case 'w':
		{
			subMenuViewType();
			break;
		}
		case 'S':
		case 's':
		{
			subMenuView();
			break;
		}
	}
}

void subMenuViewStar() {
	int c;
	clearScreen();

	printf("--==View Menu==--\n");	
	printf("[1] View All\n");
	printf("[2] View Character\n");
	printf("[3] View By Path\n");
	printf("\033[0;34m[4] View By Star\033[0m\n");
	printf("[5] View By Type\n");
	printf("[0] Back\n");
	
	c = getch();
	while (c != 'W' && c != 'w' && c != 'S' && c != 's' && c != '\n' && c != '\r'){
		c = getch();
	}

    switch(c) {
    	case '\n':
    	case '\r':
		{
			system("cls");
			navigateMenuStar();
			break;
		}
		case 'W':
		case 'w':
		{
			subMenuViewPath();
			break;
		}
		case 'S':
		case 's':
		{
			subMenuViewType();
			break;
		}
	}
}

void subMenuViewType() {
	int c;
	clearScreen();

	printf("--==View Menu==--\n");	
	printf("[1] View All\n");
	printf("[2] View Character\n");
	printf("[3] View By Path\n");
	printf("[4] View By Star\n");
	printf("\033[0;34m[5] View By Type\033[0m\n");
	printf("[0] Back\n");
	
	c = getch();
	while (c != 'W' && c != 'w' && c != 'S' && c != 's' && c != '\n' && c != '\r'){
		c = getch();
	}

    switch(c) {
    	case '\n':
    	case '\r':
		{
			system("cls");
			navigateMenuType();
			break;
		}
		case 'W':
		case 'w':
		{
			subMenuViewStar();
			break;
		}
		case 'S':
		case 's':
		{
			subMenuViewBack();
			break;
		}
	}
}

void subMenuViewCharacter() {
	int c;
	clearScreen();

	printf("--==View Menu==--\n");	
	printf("[1] View All\n");
	printf("\033[0;34m[2] View Character\033[0m\n");
	printf("[3] View By Path\n");
	printf("[4] View By Star\n");
	printf("[5] View By Type\n");
	printf("[0] Back\n");
	
	c = getch();
	while (c != 'W' && c != 'w' && c != 'S' && c != 's' && c != '\n' && c != '\r'){
		c = getch();
	}

    switch(c) {
    	case '\n':
    	case '\r':
		{
			char string[100];
			viewCharacters(NULL);

			showCursor();
			printf("What character do you want to search for: ");
			scanf(" %[^\n]", string);
			hideCursor();

	    	struct Node *temp = search(root, sumOfAscii(string));
			if(!temp) {
				puts("Data Not Found !");
			}
			else {
				system("cls");
				navigateDetails(temp);
			}
			break;
		}
		case 'W':
		case 'w':
		{
			subMenuView();
			break;
		}
		case 'S':
		case 's':
		{
			subMenuViewPath();
			break;
		}
	}
}

void subMenuView() {
	int c;
	clearScreen();

	printf("--==View Menu==--\n");	
	printf("\033[0;34m[1] View All\033[0m\n");
	printf("[2] View Character\n");
	printf("[3] View By Path\n");
	printf("[4] View By Star\n");
	printf("[5] View By Type\n");
	printf("[0] Back\n");
	
	c = getch();
	while (c != 'W' && c != 'w' && c != 'S' && c != 's' && c != '\n' && c != '\r'){
		c = getch();
	}

    switch(c) {
    	case '\n':
    	case '\r':
		{
			viewCharacters(NULL);
			system("pause");
			break;
		}
		case 'W':
		case 'w':
		{
			subMenuViewBack();
			break;
		}
		case 'S':
		case 's':
		{
			subMenuViewCharacter();
			break;
		}
	}
}

void menuView() {
	char choice;
	clearScreen();
	
	const char* subMenus[] = {
    "\t\t   \033[0;34m->[View]",
    "\t\t   \033[0m->[Gacha]",
    "\t\t   \033[0m->[Update]",
    "\t\t   \033[0m->[Insert]",
    "\t\t   \033[0m->[Delete]",
    "\t\t   \033[0m->[EXIT]",
    };
    	
    int numSubMenus = sizeof(subMenus) / sizeof(subMenus[0]);
	
	for (int i = 0; i < numSubMenus; i++) {
    	cursorPositionCenter(34 + i ,48);
        printf("%s\n", subMenus[i]);
    }
		
	choice = getch();
	while (choice != 'W' && choice != 'w' && choice != 'S' && choice != 's' && choice != '\n' && choice != '\r'){
		choice = getch();
	}
	
	if(choice == '\n' || choice == '\r'){

		clearScreen();
		system("cls");
		int c;

		printf("--==View Menu==--\n");	
		printf("[1] View All\n");
		printf("[2] View Character\n");
		printf("[3] View By Path\n");
		printf("[4] View By Star\n");
		printf("[5] View By Type\n");
		printf("[0] Back\n");
		
		c = getch();
		while (c != 'W' && c != 'w' && c != 'S' && c != 's' && c != 'A' && c != 'a' && c != 'D' && c != 'd'){
			c = getch();
		}
	
	    switch(c) {
			case 'W':
			case 'w':
			case 'S':
			case 's':
			case 'A':
			case 'a':
			case 'D':
			case 'd':
			{
				subMenuView();
				menu();
			}
		}
	}
	
	if (choice == 'W' || choice == 'w') menuExit();
	else menuGacha();
}

void menuGacha() {
	char choice;
	clearScreen();	
	
	const char* subMenus[] = {
    "\t\t   \033[0m->[View]",
    "\t\t   \033[0;34m->[Gacha]",
    "\t\t   \033[0m->[Update]",
    "\t\t   \033[0m->[Insert]",
    "\t\t   \033[0m->[Delete]",
    "\t\t   \033[0m->[EXIT]",
    };
    	
    int numSubMenus = sizeof(subMenus) / sizeof(subMenus[0]);
		
    for (int i = 0; i < numSubMenus; i++) {
    	cursorPositionCenter(34 + i ,48);
        printf("%s\n", subMenus[i]);
    }
    	
	choice = getch();
	while (choice != 'W' && choice != 'w' && choice != 'S' && choice != 's' && choice != '\n' && choice != '\r'){
		choice = getch();
	}

	if(choice == '\n' || choice == '\r'){
		system("cls");
		navigateMenuGacha();
		return;
	}

	if (choice == 'W' || choice == 'w') menuView();
	else menuUpdate();
}

void displayMenuPath(int selected) {
	clearScreen();
    printf("--==View By Path==--\n");
    for (int i = 0; i < 8; i++) {
        if (i == selected) {
            printf("\033[0;34m[%d] %s\033[0m\n", i + 1, menuPath[i]);
        } else {
            printf("[%d] %s\n", i + 1, menuPath[i]);
        }
    }
}

void navigateMenuPath() {
    int selected = 0;
    int c;

    displayMenuPath(selected);
    while (1) {
        c = getch();
        if (c == 'W' || c == 'w') {
            selected = (selected == 0) ? 7 : selected - 1;
        } 
		
		else if (c == 'S' || c == 's') {
            selected = (selected == 7) ? 0 : selected + 1;
        } 
		
		else if (c == '\n' || c == '\r') {
            if (selected == 7) return;
			else {
				viewCharacters(menuPath[selected]);
				system("pause");
				return;
			}
        }
        displayMenuPath(selected);
    }
}

void displayMenuType(int selected) {
	clearScreen();
    printf("--==View By Type==--\n");
    for (int i = 0; i < 8; i++) {
        if (i == selected) {
            printf("\033[0;34m[%d] %s\033[0m\n", i + 1, menuType[i]);
        } else {
            printf("[%d] %s\n", i + 1, menuType[i]);
        }
    }
}

void navigateMenuType() {
    int selected = 0;
    int c;

    displayMenuType(selected);
    while (1) {
        c = getch();
        if (c == 'W' || c == 'w') {
            selected = (selected == 0) ? 7 : selected - 1;
        } 
		
		else if (c == 'S' || c == 's') {
            selected = (selected == 7) ? 0 : selected + 1;
        } 
		
		else if (c == '\n' || c == '\r') {
            if (selected == 7) return;
			else {
				viewCharacters(menuType[selected]);
				system("pause");
				return;
			}
        }
        displayMenuType(selected);
    }
}

void displayMenuStar(int selected) {
	clearScreen();
    printf("--==View By Star==--\n");
    for (int i = 0; i < 3; i++) {
        if (i == selected) {
            printf("\033[0;34m[%d] %s\033[0m\n", i + 1, menuStar[i]);
        } else {
            printf("[%d] %s\n", i + 1, menuStar[i]);
        }
    }
}

void navigateMenuStar() {
    int selected = 0;
    int c;

    displayMenuStar(selected);
    while (1) {
        c = getch();
        if (c == 'W' || c == 'w') {
            selected = (selected == 0) ? 3 : selected - 1;
        } 
		
		else if (c == 'S' || c == 's') {
            selected = (selected == 3) ? 0 : selected + 1;
        } 
		
		else if (c == '\n' || c == '\r') {
            if (selected == 3) return;
			else {
				viewCharacters(menuStar[selected]);
				system("pause");
				return;
			}
        }
        displayMenuStar(selected);
    }
}

void navigateUpdate(struct Node *data){
    int selected = 0;
    int c;

    displayMenuUpdate(selected, data);
    while (1) {
        c = getch();
        if (c == 'W' || c == 'w') {
            selected = (selected == 0) ? 7 : selected - 1;
        } 
		
		else if (c == 'S' || c == 's') {
            selected = (selected == 7) ? 0 : selected + 1;
        } 
		
		else if (c == '\n' || c == '\r') {
            if (selected == 6) return;
			else {
				int n, count;
				char string[10000];
				showCursor();
				if (strcmp(menuViewUpdate[selected], "Name") == 0) {
					
					do {
						printf("What do you want to change it as[1-100 characters]: ");
						scanf(" %[^\n]", string);
					} while (strlen(string) < 1 || strlen(string) > 100); 

					struct character temp = data->data;
					deleteNode(data);
					strcpy(temp.name, string);
					insert(temp);
				}
				else if (strcmp(menuViewUpdate[selected], "Picture Name") == 0) {
					do {
						printf("What do you want to change it as[1-100 characters][.bmp]: ");
						scanf(" %[^\n]", string);
					} while (strlen(string) < 1 || strlen(string) > 100 || strcmp(getFileExtension(string), "bmp") != 0); 
					
					strcpy(data->data.name_gambar, string);
				}
				else if (strcmp(menuViewUpdate[selected], "Star Rating") == 0) {

					do {
						printf("What do you want to change it as[1-5]: ");
						count = scanf("%d", &n);
						fflush(stdin);
					} while(n < 1 || n > 5 || !count); 
					
					data->data.star = n;
				}
				else if (strcmp(menuViewUpdate[selected], "Path") == 0) {
					do{
						printf("What do you want to change it as[Destruction|The Hunt|Erudition|Harmony|Nihility|Preservation|Abundance]: ");
						scanf(" %[^\n]", string);
					} while(validatePath(string));
					
					strcpy(data->data.path, string);
				}
				else if (strcmp(menuViewUpdate[selected], "Combat Type") == 0) {

					do {
						printf("What do you want to change it as[Physical|Fire|Ice|Lightning|Wind|Quantum|Imaginary]: ");
						scanf(" %[^\n]", string);
					} while(validateType(string));
					
					strcpy(data->data.type, string);
				}
				else {
					do {
						printf("What do you want to change it as: ");
						scanf(" %[^\n]", string);
					} while(strlen(string) < 1 || strlen(string) > 10000);
					
					strcpy(data->data.description, string);
				}
				hideCursor();
				return;
			}
        }
        displayMenuUpdate(selected, data);
    }
}

void displayMenuUpdate(int selected, struct Node *data) {
	clearScreen();
    printf("--==Update Menu %s==--\n", data->data.name);
    for (int i = 0; i < 7; i++) {
        if (i == selected) {
            printf("\033[0;34m[%d] %s\033[0m\n", i + 1, menuViewUpdate[i]);
        } else {
            printf("[%d] %s\n", i + 1, menuViewUpdate[i]);
        }
    }
}

void navigateDetails(struct Node *data){
    int selected = 0;
    int c;

    displayMenuDetails(selected, data);
    while (1) {
        c = getch();
        if (c == 'W' || c == 'w') {
            selected = (selected == 0) ? 4 : selected - 1;
        } 
		
		else if (c == 'S' || c == 's') {
            selected = (selected == 4) ? 0 : selected + 1;
        } 
		
		else if (c == '\n' || c == '\r') {
            if (selected == 4) return;
			else {
				switch (selected) {
					case 0: 
					{
						system("cls");
						picture(data->data.name_gambar);
						printf("\n");
						system("pause");
						break;
					}

					case 1: 
					{
						for (int i = 0; i < 10; i++) printf("\n");
						int count = 0;
						int header = 0;
						char string[100];
						if (!lroot) {
							puts("There is no data !");
						}
						else {
							inorderLC(lroot, &count, data->data.name, &header);

							if(!header) puts("This character is not recommended to be build!\n");
							else printf("==========================================================================================================================================================================================================\n");
						}

						if(header) {
							showCursor();
							printf("\n Name of the Light Cone you want to see[0 for back]: ");
							scanf(" %[^\n]", string);
							hideCursor();
							if(strcmp(string, "0") != 0) {
								struct LC_Node *temp = searchLC(lroot, sumOfAscii(string));
								if(!temp) puts("There is no data!");
								else {
									system("cls");
									picture(temp->ldata.LC_img);
								}
							}
						}
						system("pause");
						break;
					}

					case 2: 
					{
						for (int i = 0; i < 10; i++) printf("\n");
						int count = 0;
						int header = 0;
						char string[100];
						if (!rroot) {
							printf("There is no data !\n");
						}
						else {
							inorderR(rroot, &count, data->data.name, &header);
							if (!header) puts("This characters is not recommended to be build!\n");
							else printf("==========================================================================================================================================================================================================================================\n");
						}

						if(header) {
							showCursor();
							printf("\n Name of the relic you want to see[0 for back]: ");
							scanf(" %[^\n]", string);
							hideCursor();
							if(strcmp(string, "0") != 0) {
								struct R_Node *temp = searchR(rroot, sumOfAscii(string));
								if(!temp) puts("There is no data!");
								else {
									system("cls");
									picture(temp->rdata.set_img);
								}
							}
						}

						system("pause");
						break;
					}

					case 3: 
					{
						for (int i = 0; i < 10; i++) printf("\n");
						int count = 0;
						int header = 0;
						char string[100];
						if (!proot) {
							printf("There is no data !\n");
						}
						else {
							inorderP(proot, &count, data->data.name, &header);

							if(!header) puts("This characters is not recommended to be build!");
							else printf("=======================================================================================================================================================================================================================================\n");
						}

						if(header) {
							showCursor();
							printf("\n Name of the Planar Ornaments you want to see[0 for back]: ");
							scanf(" %[^\n]", string);
							hideCursor();
							if(strcmp(string, "0") != 0) {
								struct P_Node *temp = searchP(proot, sumOfAscii(string));
								if(!temp) puts("There is no data!");
								else {
									system("cls");
									picture(temp->pdata.p_img);
								}
							}
						}
						system("pause");
						break;
					}
				}
				return;
			}
        }
        displayMenuDetails(selected, data);
    }
}

void displayMenuDetails(int selected, struct Node *data) {
	clearScreen();
	printCard(data);
	clearScreen();
	printf("\n\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t --==Menu Details==--\n");
    for (int i = 0; i < 5; i++) {
        if (i == selected) {
            printf("\t\t\t\t\t\t\t\t\t\t\t \033[0;34m[%d] %s\033[0m\n", i + 1, menuDetails[i]);
        } else {
            printf("\t\t\t\t\t\t\t\t\t\t\t [%d] %s\n", i + 1, menuDetails[i]);
        }
    }
}


void displayMenuGacha(int selected) {
	clearScreen();
    printf("--==Menu Gacha==--\n");
    for (int i = 0; i < 3; i++) {
        if (i == selected) {
            printf("\033[0;34m[%d] %s\033[0m\n", i + 1, menuGachas[i]);
        } else {
            printf("[%d] %s\n", i + 1, menuGachas[i]);
        }
    }
}

void navigateMenuGacha() {
    int selected = 0;
    int c;

    displayMenuGacha(selected);
    while (1) {
        c = getch();
        if (c == 'W' || c == 'w') {
            selected = (selected == 0) ? 2 : selected - 1;
        } 
		
		else if (c == 'S' || c == 's') {
            selected = (selected == 2) ? 0 : selected + 1;
        } 
		
		else if (c == '\n' || c == '\r') {
            if (selected == 2) return;
			else {
				srand(time(NULL));
				int choice;
				Item pt;

				switch(selected) {
					case 0: 
					{
						printf("You Pulled:\n");
						pt = gacha();
						if (strcmp(pt.name, "\033[33m****\033[0m Character") != 0 && strcmp(pt.name, "\033[33m*****\033[0m Character") != 0) {
							printf("- %s (\033[33m%s\033[0m Light Cone)\n", pt.name, getStar(pt.rarity));
						}
						break;
					}
					case 1: 
					{
						printf("You Pulled:\n");
						for(int i = 0; i < 10; i++) {

							pt = gacha();
							if (strcmp(pt.name, "\033[33m****\033[0m Character") != 0 && strcmp(pt.name, "\033[33m*****\033[0m Character") != 0) {
								printf("- %s (\033[33m%s\033[0m Light Cone)\n", pt.name, getStar(pt.rarity));
							}
						}
						break;
					}
				}
				system("pause");
				system("cls");
			}
        }
        displayMenuGacha(selected);
    }
}