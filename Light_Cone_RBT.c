#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// Definisi Warna Node
#define RED 1
#define BLACK 0

// Isi Data dari Struct
struct lightCone {
    char LC_name[100];
    char LC_path[100];
    char LC_img[100];
    int LC_rarity;
    char sChar[100];
    char CharRecom[1000];
};

// Struktur node dari Red-Black Tree
struct LC_Node {
    struct lightCone ldata;
    int lcolor;
    int lindex;
    struct LC_Node *lleft, *lright, *lparent;
};

// Inisialisasi root Struct Nnode
struct LC_Node *lroot = NULL;

// Fungsi untuk membuat node baru
struct LC_Node *createLCNode(struct lightCone ldata) {
    struct LC_Node *node = (struct LC_Node *)malloc(sizeof(struct LC_Node));
    node->ldata = ldata;
    node->lcolor = RED; // Node baru selalu berwarna merah
    node->lleft = node->lright = node->lparent = NULL;
    return node;
}

// Fungsi untuk melakukan rotasi ke kiri
void rotatelLeft(struct LC_Node **lroot, struct LC_Node *x) {
    struct LC_Node *y = x->lright;
    x->lright = y->lleft;
    if (y->lleft != NULL) y->lleft->lparent = x;
    y->lparent = x->lparent;
    if (x->lparent == NULL) (*lroot) = y;
    else if (x == x->lparent->lleft) x->lparent->lleft = y;
    else x->lparent->lright = y;
    y->lleft = x;
    x->lparent = y;
}

// Fungsi untuk melakukan rotasi ke kanan
void rotatelRight(struct LC_Node **lroot, struct LC_Node *y) {
    struct LC_Node *x = y->lleft;
    y->lleft = x->lright;
    if (x->lright != NULL) x->lright->lparent = y;
    x->lparent = y->lparent;
    if (y->lparent == NULL) (*lroot) = x;
    else if (y == y->lparent->lleft) y->lparent->lleft = x;
    else y->lparent->lright = x;
    x->lright = y;
    y->lparent = x;
}

// Fungsi untuk memperbaiki Red-Black Tree setelah insert
void fixInsertLC(struct LC_Node *z) {
    while (z != lroot && z->lparent->lcolor == RED) {
        if (z->lparent == z->lparent->lparent->lleft) {
            struct LC_Node *y = z->lparent->lparent->lright;
            if (y != NULL && y->lcolor == RED) {
                z->lparent->lcolor = BLACK;
                y->lcolor = BLACK;
                z->lparent->lparent->lcolor = RED;
                z = z->lparent->lparent;
            } else {
                if (z == z->lparent->lright) {
                    z = z->lparent;
                    rotatelLeft(&lroot, z);
                }
                z->lparent->lcolor = BLACK;
                z->lparent->lparent->lcolor = RED;
                rotatelRight(&lroot, z->lparent->lparent);
            }
        } else {
            struct LC_Node *y = z->lparent->lparent->lleft;
            if (y != NULL && y->lcolor == RED) {
                z->lparent->lcolor = BLACK;
                y->lcolor = BLACK;
                z->lparent->lparent->lcolor = RED;
                z = z->lparent->lparent;
            } else {
                if (z == z->lparent->lleft) {
                    z = z->lparent;
                    rotatelRight(&lroot, z);
                }
                z->lparent->lcolor = BLACK;
                z->lparent->lparent->lcolor = RED;
                rotatelLeft(&lroot, z->lparent->lparent);
            }
        }
    }
    lroot->lcolor = BLACK;
}

// Fungsi untuk memasukkan node baru ke dalam Red-Black Tree
void insertdataLC(struct lightCone ldata) {
    struct LC_Node *z = createLCNode(ldata);
    struct LC_Node *y = NULL;
    struct LC_Node *x = lroot;

    while (x != NULL) {
        y = x;
        if (sumOfAscii(z->ldata.LC_name) < sumOfAscii(x->ldata.LC_name)) x = x->lleft;
        else x = x->lright;
    }

    z->lparent = y;
    if (y == NULL) lroot = z;
    else if (sumOfAscii(z->ldata.LC_name) < sumOfAscii(y->ldata.LC_name)) y->lleft = z;
    else y->lright = z;

    fixInsertLC(z);
}

// Fungsi untuk melakukan in-order traversal dan mencetak elemen-elemen dalam tree
void inorderLC(struct LC_Node *lroot, int *count, const char *filter, int *headerPrinted) {
    if (lroot == NULL) return;
    
    inorderLC(lroot->lleft, count, filter, headerPrinted);
    if (strstr(lroot->ldata.CharRecom, filter)) {
        if (!(*headerPrinted)) {
            printf("==========================================================================================================================================================================================================\n");
            printf("| %-3s | %-40s | %-15s | %-10s | %-30s | %-85s |\n", "No", "Light Cone Name", "Path", "Rarity", "Suitable Character", "Character Recommendations");
            printf("==========================================================================================================================================================================================================\n");
            *headerPrinted = 1;
        }
        printf("| %-3d | %-40s | %-15s | %-10s | %-30s | %-85s |\n", ++(*count), lroot->ldata.LC_name, lroot->ldata.LC_path, 
                                                                getStar(lroot->ldata.LC_rarity), lroot->ldata.sChar, lroot->ldata.CharRecom);
        printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        fflush(stdout); // Ensure buffer is flushed after each print statement
    }
    inorderLC(lroot->lright, count, filter, headerPrinted);
}

// Fungsi untuk mencari node dalam tree berdasarkan nilai data
struct LC_Node *searchLC(struct LC_Node *lroot, int ldata) {
    while (lroot != NULL && ldata != sumOfAscii(lroot->ldata.LC_name)) {
        if (ldata < sumOfAscii(lroot->ldata.LC_name)) lroot = lroot->lleft;
        else lroot = lroot->lright;
    }
    return lroot;
}

int insertLC() {
    FILE *file = fopen("Csv/lightcone_details.txt", "r");
    if (!file) {
        perror("Error Opening File");
        exit(EXIT_FAILURE);
    }

    char line[500];
    while (fgets(line, sizeof(line), file)) {
        struct lightCone LC;
        char *token = strtok(line, ",");
        strcpy(LC.LC_name, token);

        token = strtok(NULL, ",");
        strcpy(LC.LC_path, token);

        token = strtok(NULL, ",");
        strcpy(LC.LC_img, token);

        token = strtok(NULL, ",");
        LC.LC_rarity = atoi(token);

        token = strtok(NULL, ",");
        strcpy(LC.sChar, token);

        token = strtok(NULL, "\n");
        strcpy(LC.CharRecom, token);

        insertdataLC(LC);
    }

    fclose(file);
    return 1;
}

// int main() {
//     int ch, choice;
//     insertLC();

//     while (1) {
//         printf("HONKAI STAR RAIL GUIDE TO LIGHT CONE\n");
//         printf("\n1. View All\n2. Search\n3. Exit\nEnter your choice: ");
//         scanf("%d", &choice);
//         switch (choice) {
//             case 1: {
//                 int count = 0;
//                 if (!lroot) {
//                     printf("There is no data !");
//                     break;
//                 }

//                 printf("View: \n");

//                 printf("==========================================================================================================================================================================================================\n");
//                 printf("| %-3s | %-40s | %-15s | %-10s | %-30s | %-85s |\n", "No", "Light Cone Name", "Path", "Rarity", "Suitable Character", "Character Recommendations");
//                 printf("==========================================================================================================================================================================================================\n");
//                 inorderLC(lroot, &count);
//                 printf("==========================================================================================================================================================================================================\n");
//                 break;
//             }
//             case 2: {
//                 char LC_name[100];
//                 printf("Search Light Cone: ");
//                 fflush(stdin);
//                 gets(LC_name);
//                 int ascii = sumOfAscii(LC_name);
//                 struct LC_Node *LC = search(lroot, ascii);
//                 if (LC) {
//                     printf("| %-40s | %-15s | %-10s | %-30s | %-85s |\n", LC->ldata.LC_name, LC->ldata.LC_path, getStar(LC->ldata.LC_rarity), LC->ldata.sChar, LC->ldata.CharRecom);
//                 } else {
//                     printf("Light Cone not found.\n");
//                 }
//                 break;
//             }
//             case 3: {
//                 exit(EXIT_SUCCESS);
//                 break;
//             }
//             default: {
//                 printf("Invalid choice!\n");
//                 break;
//             }
//         }
//         printf("Press any key to continue...\n");
//         getch();
//         system("cls");
//     }
//     return 0;
// }
