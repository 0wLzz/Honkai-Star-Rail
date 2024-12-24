#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>


//Definisi Warna Node
#define RED 1
#define BLACK 0

//Isi Data dari Struct
struct planarOrnament { //data untuk Planar Ornament
	char p_name[100];
	char p_img[100];
	char p_bonus[100];
	int SUL; //required Simulated Universe Level
	char pChar[100];
};

// Struktur node dari Red-Black Tree
struct P_Node{ //Node untuk Planar Ornaments
	struct planarOrnament pdata;
	int pcolor;
	int pindex;
	struct P_Node *pleft, *pright, *pparent;
};

//Inisialisasi root Struct Nnode
struct P_Node *proot = NULL;

// Fungsi untuk membuat node baru
struct P_Node* createPNode(struct planarOrnament pdata) {
    struct P_Node* pnode = (struct P_Node*)malloc(sizeof(struct P_Node));

    pnode->pdata = pdata;
    pnode->pcolor = RED; // Node baru selalu berwarna merah
    pnode->pleft = pnode->pright = pnode->pparent = NULL;
    return pnode;
}

// Fungsi untuk melakukan rotasi ke kiri
void rotatepLeft(struct P_Node **proot, struct P_Node *x) {
    struct P_Node *y = x->pright;
    x->pright = y->pleft;
    if (y->pleft != NULL) y->pleft->pparent = x;
    y->pparent = x->pparent;
    if (x->pparent == NULL) (*proot) = y;
    else if (x == x->pparent->pleft) x->pparent->pleft = y;
    else x->pparent->pright = y;
    y->pleft = x;
    x->pparent = y;
}

// Fungsi untuk melakukan rotasi ke kanan
void rotatepRight(struct P_Node **proot, struct P_Node *y) {
    struct P_Node *x = y->pleft;
    y->pleft = x->pright;
    if (x->pright != NULL) x->pright->pparent = y;
    x->pparent = y->pparent;
    if (y->pparent == NULL) (*proot) = x;
    else if (y == y->pparent->pleft) y->pparent->pleft = x;
    else y->pparent->pright = x;
    x->pright = y;
    y->pparent = x;
}

// Fungsi untuk memperbaiki Red-Black Tree setelah insert
void fixInsertP(struct P_Node *z) {
    while (z != proot && z->pparent->pcolor == RED) {
        if (z->pparent == z->pparent->pparent->pleft) {
            struct P_Node *y = z->pparent->pparent->pright;
            if (y != NULL && y->pcolor == RED) {
                z->pparent->pcolor = BLACK;
                y->pcolor = BLACK;
                z->pparent->pparent->pcolor = RED;
                z = z->pparent->pparent;
            } else {
                if (z == z->pparent->pright) {
                    z = z->pparent;
                    rotatepLeft(&proot, z);
                }
                z->pparent->pcolor = BLACK;
                z->pparent->pparent->pcolor = RED;
                rotatepRight(&proot, z->pparent->pparent);
            }
        } else {
            struct P_Node *y = z->pparent->pparent->pleft;
            if (y != NULL && y->pcolor == RED) {
                z->pparent->pcolor = BLACK;
                y->pcolor = BLACK;
                z->pparent->pparent->pcolor = RED;
                z = z->pparent->pparent;
            } else {
                if (z == z->pparent->pleft) {
                    z = z->pparent;
                    rotatepRight(&proot, z);
                }
                z->pparent->pcolor = BLACK;
                z->pparent->pparent->pcolor = RED;
                rotatepLeft(&proot, z->pparent->pparent);
            }
        }
    }
    proot->pcolor = BLACK;
}



// Fungsi untuk memasukkan node baru ke dalam Red-Black Tree
void insertdataP(struct planarOrnament pdata) {
    struct P_Node *z = createPNode(pdata);
    struct P_Node *y = NULL;
    struct P_Node *x = proot;

    while (x != NULL) {
        y = x;
        if (sumOfAscii(z->pdata.p_name) < sumOfAscii(x->pdata.p_name)) x = x->pleft;
        else x = x->pright;
    }

    z->pparent = y;
    if (y == NULL) proot = z;
    else if (sumOfAscii(z->pdata.p_name) < sumOfAscii(y->pdata.p_name)) y->pleft = z;
    else y->pright = z;

    fixInsertP(z);
}



// Fungsi untuk melakukan in-order traversal dan mencetak elemen-elemen dalam tree
void inorderP(struct P_Node *proot, int *count, const char* filter, int *headerPrinted) {
    if (proot == NULL) return;

    inorderP(proot->pleft, count, filter, headerPrinted);
    if (strstr(proot->pdata.pChar, filter)) {
        if (!(*headerPrinted)) {
            printf("=======================================================================================================================================================================================================================================\n");
            printf("| %-3s | %-45s | %-55s | %-12s | %-100s |\n", "No", "Name", "Bonus", "Path", "Suitable Character");
            printf("=======================================================================================================================================================================================================================================\n");
            *headerPrinted = 1;
        }
        printf("| %-3d | %-45s | %-55s | %-12d | %-100s |\n", ++(*count), proot->pdata.p_name, proot->pdata.p_bonus, 
                                                            proot->pdata.SUL,  proot->pdata.pChar);
        printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        fflush(stdout); // Ensure buffer is flushed after each print statement
    }
    inorderP(proot->pright, count, filter, headerPrinted);  
}


// Fungsi untuk mencari node dalam tree berdasarkan nilai data
struct P_Node* searchP(struct P_Node *proot, int pdata) {
    while (proot != NULL && pdata != sumOfAscii(proot->pdata.p_name)) {
        if (pdata < sumOfAscii(proot->pdata.p_name)) proot = proot->pleft;
        else proot = proot->pright;
    }
    return proot;
}
int insertP() {
    FILE *file = fopen("Csv/planar_detail.txt", "r");
    if (!file) {
        perror("Error Opening File");
        exit(EXIT_FAILURE);
    }
    
    char line[300];
    while (fgets(line, sizeof(line), file)) {
    	
        struct planarOrnament p;
        char *token = strtok(line, ",");
        strcpy(p.p_name, token);
        
        token = strtok(NULL, ",");
        strcpy(p.p_img, token);
        
        token = strtok(NULL, ",");
        strcpy(p.p_bonus, token);
        
        token = strtok(NULL, ",");
        p.SUL = atoi(token);
        
        token = strtok(NULL, "\n");
        strcpy(p.pChar, token);
        
        insertdataP(p);
    }
    
    fclose(file);
    return 1;
}



// int main() {
//     int ch, choice;
//     insertP();
    
//     while (1) {
//     	printf("HONKAI STAR RAIL GUIDE TO PLANAR ORNAMENTS\n");
//         printf("\n1. View All\n2. Search\n3. Exit\nEnter your choice: ");
//         scanf("%d", &choice);
//         switch (choice) {
//             case 1:
// 			{
// 				int count = 0;
//             	if (!proot) {
//             		printf("There is no data !");
//             		break;
// 				}

//                 printf("View: \n");

//             	printf("=======================================================================================================================================================================================================================================\n");
//             	printf("| %-3s | %-45s | %-55s | %-12s | %-100s |\n", "No", "Pair Name", "Bonus", "World Level", "Best For");
//             	printf("=======================================================================================================================================================================================================================================\n");
//                 inorderP(proot, &count);
//             	printf("=======================================================================================================================================================================================================================================\n");
// 				break;
// 			}
//             case 2:
// 			{
//             	char string[30];
//             	int cek = 0;
            	
//             	printf("What do you want to search: ");
//             	scanf(" %[^\n]", string);
            
//                 struct P_Node *temp = search(proot, sumOfAscii(string));

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

