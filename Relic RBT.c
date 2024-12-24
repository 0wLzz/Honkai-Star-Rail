#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>


//Definisi Warna Node
#define RED 1
#define BLACK 0

//Isi Data dari Struct
struct relic { //data untuk Relic
    char set_name[100];
    char set_img[100];
    char rp_bonus[100];    
    char DoS[200];
    char rbestChar[100];
};



// Struktur node dari Red-Black Tree
struct R_Node { //node untuk Relic
    struct relic rdata; 
    int rcolor;
    int rindex;
    struct R_Node *rleft, *rright, *rparent;
};



//Inisialisasi root Struct Nnode
struct R_Node *rroot = NULL;



// Fungsi untuk membuat node baru
struct R_Node* createRNode(struct relic rdata) {
    struct R_Node* node = (struct R_Node*)malloc(sizeof(struct R_Node));

    node->rdata = rdata;
    node->rcolor = RED; // Node baru selalu berwarna merah
    node->rleft = node->rright = node->rparent = NULL;
    return node;
}



// Fungsi untuk melakukan in-order traversal dan mencetak elemen-elemen dalam tree
void inorderR(struct R_Node *rroot, int *count, const char *filter, int *headerPrinted) {
    if (rroot == NULL) return;

    inorderR(rroot->rleft, count, filter, headerPrinted);
    if (strstr(rroot->rdata.rbestChar, filter)) {
        if (!(*headerPrinted)) {
            printf("==========================================================================================================================================================================================================================================\n");
            printf("| %-3s | %-50s | %-50s | %-50s | %-65s |\n", "No", "Name", "Bonus", "Path", "Suitable Character");
            printf("==========================================================================================================================================================================================================================================\n");
            *headerPrinted = 1;
        }
        printf("| %-3d | %-50s | %-50s | %-50s | %-65s |\n", ++(*count), rroot->rdata.set_name, rroot->rdata.rp_bonus, 
                                                            rroot->rdata.DoS,  rroot->rdata.rbestChar);
        printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        fflush(stdout); // Ensure buffer is flushed after each print statement
    }
    inorderR(rroot->rright, count, filter, headerPrinted);
}



// Fungsi untuk mencari node dalam tree berdasarkan nilai data
struct R_Node* searchR(struct R_Node *rroot, int rdata) {
    while (rroot != NULL && rdata != sumOfAscii(rroot->rdata.set_name)) {
        if (rdata < sumOfAscii(rroot->rdata.set_name)) rroot = rroot->rleft;
        else rroot = rroot->rright;
    }
    return rroot;
}

// Fungsi untuk melakukan rotasi ke kiri
void rotateRLeft(struct R_Node **rroot, struct R_Node *x) {
    struct R_Node *y = x->rright;
    x->rright = y->rleft;
    if (y->rleft != NULL) y->rleft->rparent = x;
    y->rparent = x->rparent;
    if (x->rparent == NULL) (*rroot) = y;
    else if (x == x->rparent->rleft) x->rparent->rleft = y;
    else x->rparent->rright = y;
    y->rleft = x;
    x->rparent = y;
}



// Fungsi untuk melakukan rotasi ke kanan
void rotateRRight(struct R_Node **rroot, struct R_Node *y) {
    struct R_Node *x = y->rleft;
    y->rleft = x->rright;
    if (x->rright != NULL) x->rright->rparent = y;
    x->rparent = y->rparent;
    if (y->rparent == NULL) (*rroot) = x;
    else if (y == y->rparent->rleft) y->rparent->rleft = x;
    else y->rparent->rright = x;
    x->rright = y;
    y->rparent = x;
}



// Fungsi untuk memperbaiki Red-Black Tree setelah insert
void fixInsertR(struct R_Node *z) {
    while (z != rroot && z->rparent->rcolor == RED) {
        if (z->rparent == z->rparent->rparent->rleft) {
            struct R_Node *y = z->rparent->rparent->rright;
            if (y != NULL && y->rcolor == RED) {
                z->rparent->rcolor = BLACK;
                y->rcolor = BLACK;
                z->rparent->rparent->rcolor = RED;
                z = z->rparent->rparent;
            } else {
                if (z == z->rparent->rright) {
                    z = z->rparent;
                    rotateRLeft(&rroot, z);
                }
                z->rparent->rcolor = BLACK;
                z->rparent->rparent->rcolor = RED;
                rotateRRight(&rroot, z->rparent->rparent);
            }
        } else {
            struct R_Node *y = z->rparent->rparent->rleft;
            if (y != NULL && y->rcolor == RED) {
                z->rparent->rcolor = BLACK;
                y->rcolor = BLACK;
                z->rparent->rparent->rcolor = RED;
                z = z->rparent->rparent;
            } else {
                if (z == z->rparent->rleft) {
                    z = z->rparent;
                    rotateRRight(&rroot, z);
                }
                z->rparent->rcolor = BLACK;
                z->rparent->rparent->rcolor = RED;
                rotateRLeft(&rroot, z->rparent->rparent);
            }
        }
    }
    rroot->rcolor = BLACK;
}

// Fungsi untuk memasukkan node baru ke dalam Red-Black Tree
void insertdataR(struct relic rdata) {
    struct R_Node *z = createRNode(rdata);
    struct R_Node *y = NULL;
    struct R_Node *x = rroot;

    while (x != NULL) {
        y = x;
        if (sumOfAscii(z->rdata.set_name) < sumOfAscii(x->rdata.set_name)) x = x->rleft;
        else x = x->rright;
    }

    z->rparent = y;
    if (y == NULL) rroot = z;
    else if (sumOfAscii(z->rdata.set_name) < sumOfAscii(y->rdata.set_name)) y->rleft = z;
    else y->rright = z;

    fixInsertR(z);
}


int insertR() {
    FILE *file = fopen("Csv/relic_details.txt", "r");
    if (!file) {			
        perror("Error Opening File");
        exit(EXIT_FAILURE);
    }
    
    char line[400];
    while (fgets(line, sizeof(line), file)) {
    	
        struct relic rl;
        char *token = strtok(line, ",");
        strcpy(rl.set_name, token);
        
        token = strtok(NULL, ",");
        strcpy(rl.set_img, token);
        
        token = strtok(NULL, ",");
        strcpy(rl.rp_bonus, token);
        
        token = strtok(NULL, ",");
        strcpy(rl.DoS, token);
        
        token = strtok(NULL, "\n");
        strcpy(rl.rbestChar, token);
        
        insertdataR(rl);
    }
    
    fclose(file);
    return 1;
}


// int main() {
//     int ch, choice;
//     insertR();
    
//     while (1) {
//     	printf("HONKAI STAR RAIL GUIDE TO RELIC SET\n");
//         printf("\n1. View All\n2. Search\n3. Exit\nEnter your choice: ");
//         scanf("%d", &choice);
//         switch (choice) {
//             case 1:
// 			{
// 				int count = 0;
//             	if (!rroot) {
//             		printf("There is no data !");
//             		break;
// 				}

//                 printf("View: \n");

//             	printf("========================================================================================================\n");
//             	printf("| %-3s | %-25s | %-20s | %-20s | %-20s |\n", "No", "Name", "Rarity", "Path", "Combat Type");
//             	printf("========================================================================================================\n");
//                 inorderR(rroot, &count);
//             	printf("========================================================================================================\n");
// 				break;
// 			}
//             case 2:
// 			{
//             	char string[30];
//             	int cek = 0;
            	
//             	printf("What do you want to search: ");
//             	scanf(" %[^\n]", string);
            
//                 struct R_Node *temp = search(rroot, sumOfAscii(string));

//                 if(!temp) {
//                     puts("Data Not Found !");
//                     break;     
//                 }

//                 puts("Data Found!");
// 				break;
// 			}
// 			case 3:
// 				exit(0);
//             default:
//                 printf("Invalid choice\n");
//         }
        
//         if(ch != 3) {
//         	printf("Press Anything to Continue. . .");
//         	getch();
//         	fflush(stdout);
//         	system("cls");
// 		}
//     }

//     return 0;
// }
