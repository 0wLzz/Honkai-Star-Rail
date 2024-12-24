#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Define the structures for the headers and RGB pixel data
struct BITMAP_header {
    char name[2];
    unsigned int size;
    unsigned int reserved;
    unsigned int image_offset;
};

struct DIB_header {
    unsigned int header_size;
    int width;
    int height;
    unsigned short int colorplanes;
    unsigned short int bitsperpixel;
    unsigned int compression;
    unsigned int image_size;
};

struct RGB {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

struct Image {
    int height;
    int width;
    struct RGB **rgb;
};

// Function to read the image data from the file
struct Image readImage(FILE *fp, int height, int width, int row_size) {
    struct Image pic;
    pic.height = abs(height);
    pic.width = width;

    pic.rgb = (struct RGB **)malloc(pic.height * sizeof(struct RGB *));
    if (pic.rgb == NULL) {
        perror("Unable to allocate memory for image rows");
        exit(1);
    }

    for (int i = 0; i < pic.height; i++) {
        pic.rgb[i] = (struct RGB *)malloc(width * sizeof(struct RGB));
        if (pic.rgb[i] == NULL) {
            perror("Unable to allocate memory for image row");
            exit(1);
        }
    }

    if (height > 0) {
        for (int i = pic.height - 1; i >= 0; i--) {
            if (fread(pic.rgb[i], sizeof(struct RGB), width, fp) != width) {
                perror("Error reading pixel data");
                exit(1);
            }
            fseek(fp, row_size - (width * sizeof(struct RGB)), SEEK_CUR);
        }
    } else {
        for (int i = 0; i < pic.height; i++) {
            if (fread(pic.rgb[i], sizeof(struct RGB), width, fp) != width) {
                perror("Error reading pixel data");
                exit(1);
            }
            fseek(fp, row_size - (width * sizeof(struct RGB)), SEEK_CUR);
        }
    }

    return pic;
}

// Function to free the allocated memory for the image
void freeImage(struct Image pic) {
    for (int i = 0; i < pic.height; i++) {
        free(pic.rgb[i]);
    }
    free(pic.rgb);
}

// Function to convert RGB to grayscale
unsigned char grayscale(struct RGB rgb) {
    return (unsigned char)((0.3 * rgb.red) + (0.6 * rgb.green) + (0.1 * rgb.blue));
}

// Function to resize the image
struct Image resizeImage(struct Image img, int new_width, int new_height) {
    struct Image resized;
    resized.height = new_height;
    resized.width = new_width;

    resized.rgb = (struct RGB **)malloc(new_height * sizeof(struct RGB *));
    if (resized.rgb == NULL) {
        perror("Unable to allocate memory for resized image rows");
        exit(1);
    }

    for (int i = 0; i < new_height; i++) {
        resized.rgb[i] = (struct RGB *)malloc(new_width * sizeof(struct RGB));
        if (resized.rgb[i] == NULL) {
            perror("Unable to allocate memory for resized image row");
            exit(1);
        }
    }

    int x_ratio = (int)((img.width << 16) / new_width) + 1;
    int y_ratio = (int)((img.height << 16) / new_height) + 1;
    int x2, y2;

    for (int i = 0; i < new_height; i++) {
        for (int j = 0; j < new_width; j++) {
            x2 = ((j * x_ratio) >> 16);
            y2 = ((i * y_ratio) >> 16);
            resized.rgb[i][j] = img.rgb[y2][x2];
        }
    }

    return resized;
}

// Function to convert an image to text and print it
void imageToText(struct Image img) {
    unsigned char gs;
    char textpixel[] = {'@', '#', '%', '0', 'a', '-', '.', ' '};
    
    FILE *file = fopen("Output.txt", "w");

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            gs = grayscale(img.rgb[i][j]);
            fprintf(file, "%c", textpixel[7 - gs / 32]);
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
}

int picture(char *s) {
	char route[100] = "Asset/";
	strcat(route, s);
	
    FILE *file = fopen(route, "rb");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

	puts("\033[1;33mDISCLAIMER\033[0m");
	printf("To see all of the image, please zoom out the terminal\n\n");
	
    printf("Loading");
    fflush(stdout);

    // Animation loop
    for (int i = 0; i < 10; i++) {
        printf(".");
        fflush(stdout);
        sleep(1);
    }
    printf("\n");
    

    struct BITMAP_header header;
    struct DIB_header dibheader;

    fread(header.name, sizeof(char), 2, file);
    if (header.name[0] != 'B' || header.name[1] != 'M') {
        perror("Not a BM file!");
        fclose(file);
        return 1;
    }

    fread(&header.size, sizeof(unsigned int), 1, file);
    fread(&header.reserved, sizeof(unsigned int), 1, file);
    fread(&header.image_offset, sizeof(unsigned int), 1, file);

    fread(&dibheader, sizeof(struct DIB_header), 1, file);

    // Calculate the row size with padding
    int bytes_per_pixel = dibheader.bitsperpixel / 8;
    int row_size = ((dibheader.width * bytes_per_pixel + 3) / 4) * 4;

    fseek(file, header.image_offset, SEEK_SET);
    struct Image image = readImage(file, dibheader.height, dibheader.width, row_size);

    // Resize the image to smaller dimensions
    int new_width = image.width / 2;  // Adjust as needed
    int new_height = image.height / 2; // Adjust as needed
    struct Image resized_image = resizeImage(image, new_width, new_height);

    imageToText(resized_image);
    freeImage(image);
    freeImage(resized_image);
    fclose(file);
    
    printf("Loading complete. \nPrinting file content...\n");
    sleep(3);
    system("cls");
	
	FILE *outputFile = fopen("Output.txt", "r");
    if (outputFile == NULL) {
        perror("Error opening output file");
        return 1;
    }

    fseek(outputFile, 0, SEEK_END);
    long fileSize = ftell(outputFile);
    fseek(outputFile, 0, SEEK_SET);

    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(outputFile);
        return 1;
    }
    
    fread(buffer, 1, fileSize, outputFile);
    buffer[fileSize] = '\0';

    fclose(outputFile);

    printf("%s", buffer);
    free(buffer);
}
