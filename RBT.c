#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

// Definisi warna node
#define RED 1
#define BLACK 0

struct character {
    char name[100];
    char name_gambar[100];
    int star;
    char path[100];
    char type[100];
    char description[10000];
};

// Struktur node dari Red-Black Tree
struct Node {
    struct character data;
    int color;
    int index;
    struct Node *left, *right, *parent;
};

struct Node *root = NULL;

int sumOfAscii(char *s) {
    int uniqueInt = 0;
    const int prime = 31;
    while (*s != '\0') {
        uniqueInt = (uniqueInt * prime) + *s;
        *s++;
    }
    return uniqueInt;
}

char *getStar(int n) {
	char *star = (char *) malloc ((n+1) * sizeof(char));
	int i;
	for(i = 0; i < n; i++) {
		star[i] = '*';
	}
	star[i] = '\0';
	return star;
}

// Fungsi untuk membuat node baru
struct Node* createNode(struct character data) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));

    node->data = data;
    node->color = RED; // Node baru selalu berwarna merah
    node->left = node->right = node->parent = NULL;
    return node;
}

// Fungsi untuk melakukan rotasi ke kiri
void rotateLeft(struct Node **root, struct Node *x) {
    struct Node *y = x->right;
    x->right = y->left;
    if (y->left != NULL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL) (*root) = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Fungsi untuk melakukan rotasi ke kanan
void rotateRight(struct Node **root, struct Node *y) {
    struct Node *x = y->left;
    y->left = x->right;
    if (x->right != NULL) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL) (*root) = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

// Fungsi untuk memperbaiki Red-Black Tree setelah insert
void fixInsert(struct Node *z) {
    while (z != root && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            struct Node *y = z->parent->parent->right;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(&root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateRight(&root, z->parent->parent);
            }
        } else {
            struct Node *y = z->parent->parent->left;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(&root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotateLeft(&root, z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

// Fungsi untuk memasukkan node baru ke dalam Red-Black Tree
void insert(struct character data) {
    struct Node *z = createNode(data);
    struct Node *y = NULL;
    struct Node *x = root;

    while (x != NULL) {
        y = x;
        if (sumOfAscii(z->data.name) < sumOfAscii(x->data.name)) x = x->left;
        else x = x->right;
    }

    z->parent = y;
    if (y == NULL) root = z;
    else if (sumOfAscii(z->data.name) < sumOfAscii(y->data.name)) y->left = z;
    else y->right = z;

    fixInsert(z);
}

// Fungsi untuk mencari node pengganti dalam delete
struct Node* findMin(struct Node* node) {
    while (node->left != NULL) node = node->left;
    return node;
}

// Fungsi untuk memperbaiki Red-Black Tree setelah delete
void fixDelete(struct Node *x) {
    while (x != root && (x == NULL || x->color == BLACK)) {
        if (x == x->parent->left) {
            struct Node *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateLeft(&root, x->parent);
                w = x->parent->right;
            }
            if ((w->left == NULL || w->left->color == BLACK) && (w->right == NULL || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right == NULL || w->right->color == BLACK) {
                    if (w->left != NULL) w->left->color = BLACK;
                    w->color = RED;
                    rotateRight(&root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right != NULL) w->right->color = BLACK;
                rotateLeft(&root, x->parent);
                x = root;
            }
        } else {
            struct Node *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotateRight(&root, x->parent);
                w = x->parent->left;
            }
            if ((w->left == NULL || w->left->color == BLACK) && (w->right == NULL || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left == NULL || w->left->color == BLACK) {
                    if (w->right != NULL) w->right->color = BLACK;
                    w->color = RED;
                    rotateLeft(&root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left != NULL) w->left->color = BLACK;
                rotateRight(&root, x->parent);
                x = root;
            }
        }
    }
    if (x != NULL) x->color = BLACK;
}

// Fungsi untuk menghapus node dari Red-Black Tree
void deleteNode(struct Node *z) {
    struct Node *y = z;
    struct Node *x;
    int yOriginalColor = y->color;

    if (z->left == NULL) {
        x = z->right;
        if (x) x->parent = z->parent;
        if (z->parent == NULL) root = x;
        else if (z == z->parent->left) z->parent->left = x;
        else z->parent->right = x;
    } else if (z->right == NULL) {
        x = z->left;
        if (x) x->parent = z->parent;
        if (z->parent == NULL) root = x;
        else if (z == z->parent->left) z->parent->left = x;
        else z->parent->right = x;
    } else {
        y = findMin(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x) x->parent = y;
        } else {
            if (x) x->parent = y->parent;
            y->parent->left = x;
            y->right = z->right;
            y->right->parent = y;
        }

        if (z->parent == NULL) root = y;
        else if (z == z->parent->left) z->parent->left = y;
        else z->parent->right = y;

        y->parent = z->parent;
        y->color = z->color;
        y->left = z->left;
        y->left->parent = y;
    }

    free(z);

    if (yOriginalColor == BLACK && x != NULL) {
        fixDelete(x);
    }
}

void update(struct Node *data) {
    int choice;
    char string[30];
    puts("Menu Change");
    puts("1. Picture Name");
    puts("2. Star Rating");
    puts("3. Path");
    puts("4. Combat Type");
    
    printf("What do you want to change: ");
    scanf("%d", &choice);
    
    switch(choice) {
        case 1:
        {
            printf("What do you want to change it as: ");
            scanf(" %[^\n]", string);
            
            strcpy(data->data.name_gambar, string);
            break;
        }
        case 2:
        {
            printf("What do you want to change it as: ");
            scanf(" %[^\n]", string);
            
            strcpy(data->data.path, string);
            break;
        }
        case 3:
        {
            printf("What do you want to change it as: ");
            scanf("%d", &choice);
            
            data->data.star = choice;
            break;
        }
        case 4:
        {
            printf("What do you want to change it as: ");
            scanf(" %[^\n]", string);
            
            strcpy(data->data.type, string);
            break;
        }
    }
}

// Fungsi untuk mencari node dalam tree berdasarkan nilai data
struct Node* search(struct Node *root, int data) {
    while (root != NULL && data != sumOfAscii(root->data.name)) {
        if (data < sumOfAscii(root->data.name)) root = root->left;
        else root = root->right;
    }
    return root;
}

int insertData() {
    FILE *file = fopen("Csv/honkai_star_rail.csv", "r");
    if (!file) {
        perror("Error Opening File");
        exit(EXIT_FAILURE);
    }
    
    char line[10000];
    while (fgets(line, sizeof(line), file)) {
    	
        struct character ch;
        char *token = strtok(line, ",");
        strcpy(ch.name, token);
        
        token = strtok(NULL, ",");
        strcpy(ch.name_gambar, token);
        
        token = strtok(NULL, ",");
        ch.star = atoi(token);
        
        token = strtok(NULL, ",");
        strcpy(ch.path, token);
        
        token = strtok(NULL, ",");
        strcpy(ch.type, token);

        token = strtok(NULL, "\n");
        strcpy(ch.description, token);
        
        insert(ch);
    }
    
    fclose(file);
    return 1;
}

void inorder(struct Node *root, int *count, const char *filterType) {
    if (root == NULL) return;
    
    inorder(root->left, count, filterType);
    // printf("%s %s\n", filterType, root->data.);
    // printf("%d", strcmp(root->data.type, filterType));
    
    if (!filterType || strcmp(root->data.path, filterType) == 0 || strcmp(root->data.type, filterType) == 0 || atoi(filterType) == root->data.star) {
        printf("| %-3d | %-25s | %-20s | %-20s | %-20s |\n", ++(*count), root->data.name, getStar(root->data.star), 
                                                    root->data.path, root->data.type);
        printf("--------------------------------------------------------------------------------------------------------\n");
    }
    
    inorder(root->right, count, filterType);
}

void viewCharacters(const char *filterType) {
    int count = 0;
    if (!root) {
        printf("There is no data !\n");
        return;
    }
    
    printf("========================================================================================================\n");
    printf("| %-3s | %-25s | %-20s | %-20s | %-20s |\n", "No", "Name", "Rarity", "Path", "Combat Type");
    printf("========================================================================================================\n");
    inorder(root, &count, filterType);
    printf("========================================================================================================\n");
}

// Program utama untuk menguji Red-Black Tree
//int main() {
//    int ch, choice;
//    insertData();
//    
//    while (1) {
//    	printf("B-Tree Implementation\n");
//        printf("\n1. View All\n2. Search\n3. Update\n4. Delete\n5. Exit\nEnter your choice: ");
//        scanf("%d", &choice);
//        switch (choice) {
//            case 1:
//			{
//				int count = 0;
//            	if (!root) {
//            		printf("There is no data !");
//            		break;
//				}
//
//                printf("View: \n");
//
//            	printf("========================================================================================================\n");
//            	printf("| %-3s | %-25s | %-20s | %-20s | %-20s |\n", "No", "Name", "Rarity", "Path", "Combat Type");
//            	printf("========================================================================================================\n");
//                inorder(root, &count);
//            	printf("========================================================================================================\n");
//				break;
//			}
//            case 2:
//			{
//            	char string[30];
//            	int cek = 0;
//            	
//            	printf("What do you want to search: ");
//            	scanf(" %[^\n]", string);
//            
//                struct Node *temp = search(root, sumOfAscii(string));
//
//                if(!temp) {
//                    puts("Data Not Found !");
//                    break;     
//                }
//
//                puts("Data Found!");
//				break;
//			}
//			case 3:
//			{
//				char string[30];
//				printf("What do you want to update: ");
//				scanf(" %[^\n]", string);
//				
//                struct Node *temp = search(root, sumOfAscii(string));
//
//                if(!temp) {
//                    puts("Data Not Found !");
//                    break;     
//                }
//
//                update(temp);
//
//				break;		
//			}
//			case 4: 
//			{
//				char string[30];
//            	printf("What do you want to delete: ");
//            	scanf(" %[^\n]", string);
//
//                struct Node *nodeToDelete = search(root, sumOfAscii(string));
//
//                if (nodeToDelete) {
//                    deleteNode(nodeToDelete);
//                    puts("Delete Succesfull !\n");
//                }
//                else puts("Data Not Found!\n");
//            	
//				break;
//			}
//            case 5:
//                exit(0);
//            default:
//                printf("Invalid choice\n");
//        }
//        
//        if(ch != 5) {
//        	system("pause");
//        	system("cls");
//		}
//    }
//
//    return 0;
//}
