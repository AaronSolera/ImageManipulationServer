#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define COLOR_CHANNELS 3 //RED,GREEN,BLUE
#define R_CHANNEL 2
#define G_CHANNEL 1
#define B_CHANNEL 0
#define WINDOW_FILTER_SIZE 3
#define fileHeaderSize 14
#define infoHeaderSize 40

/*
 * Function: allocateMemorySpaceForImage
 * --------------------------------------
 * Returns a triple pointer to the allocated memory (3D array)
 *
 *    ptr_height: block memory height
 *    ptr_width: block memory width
 *
 *    returns: first memory direction of the block
 */
unsigned char *** allocateMemorySpaceForImage(const int * ptr_height, const  int * ptr_width);


/*
 * Function: deallocateMemorySpaceOfImage
 * --------------------------------------
 * Free the memory allocated for an image
 *
 *    ptr_height: block memory height
 *    ptr_width: block memory width
 *    img: block memory initial direction
 */
void deallocateMemorySpaceOfImage(const int * ptr_height, const  int * ptr_width, unsigned char *** img);


/*
 * Function: stringToImage
 * ------------------------
 * Returns a triple pointer to the allocated memory (3D) for the image
 * Converts a string array that contain the information of image pixels into a 3D matrix,
 * where each color channel (RGB) is a matrix
 *
 *    ptr_height: memory direction of the image height
 *    ptr_width: memory direction of the image width
 *    str_pixels: pixels of the image in (R,G,B,..,R,G,B) string format
 *
 *    returns: first memory direction of the image
 */
unsigned char *** stringToImage(const int * ptr_height, const int * ptr_width, char * str_pixels);


/*
 * Function: convertImageToGrayscale
 * -----------------------------------
 * Convert a color image into a grayscale image
 *
 *    ptr_height: memory direction of the image height
 *    ptr_width: memory direction of the image width
 *    img: memory direction of the image pixels
 */
void convertImageToGrayscale(const int * ptr_height, const int * ptr_width, unsigned char *** img);


//----------------------------------------------------------------------------------------------------------------------
// FUNCTIONS FOR IMAGE CLASSIFICATION
//----------------------------------------------------------------------------------------------------------------------

/*
 * Function: classifyImageByColor
 * -------------------------------
 * Classify an image according to his color in one of the following categories: RED, GREEN, BLUE
 * Modify color_type value according to the corresponding classification of the image:
 *                                 0 -> RED, 1 -> GREEN, 2 -> BLUE
 *
 *    ptr_height: memory direction of the image height
 *    ptr_width: memory direction of the image width
 *    img: memory direction of the image pixels
 *    ptr_color_type: memory direction of the image color type
 */
void classifyImageByColor(const int * ptr_height, const int * ptr_width, unsigned char *** img, int * ptr_color_type);


/*
 * Function: incrementColorCounter
 * --------------------------------
 * Increment the respective color counter according to the value of the RGB color channels
 *
 *    ptr_R_value: memory direction of the red channel pixel
 *    ptr_G_value: memory direction of the green channel pixel
 *    ptr_B_value: memory direction of the blue channel pixel
 *    ptr_R_counter: memory direction of the red color counter
 *    ptr_G_counter: memory direction of the green color counter
 *    ptr_B_Counter: memory direction of the blue color counter
 */
void incrementColorCounter(const unsigned char * ptr_R_value,
                           const unsigned char * ptr_G_value,
                           const unsigned char * ptr_B_value,
                           int * ptr_R_counter,
                           int * ptr_G_counter,
                           int * ptr_B_Counter);


/*
 * Function: classifyImage
 * ------------------------
 * Set the value of the color_type according to the value of the RGB counter
 *                     0 -> RED, 1 -> GREEN, 2 -> BLUE
 *
 *    ptr_R_counter: memory direction of the red color counter
 *    ptr_G_counter: memory direction of the green color counter
 *    ptr_B_counter: memory direction of the blue color counter
 *    ptr_color_type: memory direction of the image color type
 */
void classifyImage(const int * ptr_R_counter,
                   const int * ptr_G_counter,
                   const int * ptr_B_counter,
                   int * ptr_color_type);

//----------------------------------------------------------------------------------------------------------------------
// FUNCTIONS FOR FILTER IMAGES
//----------------------------------------------------------------------------------------------------------------------

/*
 * Function: meanFilter
 * ---------------------
 * Apply a mean filter to the image
 *
 *    ptr_height: memory direction of the image height
 *    ptr_width: memory direction of the image width
 *    ptr_input_img: memory direction of the image
 */
void meanFilter(const int * ptr_height, const int * ptr_width,
                unsigned char *** input_img, unsigned char *** output_img);

/*
 * Function: meanFilter
 * ---------------------
 * Apply a median filter to the image
 *
 *    ptr_height: memory direction of the image height
 *    ptr_width: memory direction of the image width
 *    ptr_input_img: memory direction of the image
 */
void medianFilter(const int * ptr_height, const int * ptr_width,
                  unsigned char *** input_img, unsigned char *** output_img);


/*
 * Function: calculateMedian
 * --------------------------
 * Returns the median value of the elements of the array
 *
 *    arr: memory direction of the array
 *    length: length of the array
 */
int calculateMedian(unsigned char * arr, int length);


//----------------------------------------------------------------------------------------------------------------------
// FUNCTIONS FOR SAVE BITMAP IMAGE
//----------------------------------------------------------------------------------------------------------------------

/*
 * Function: generateBitmapImage
 * ------------------------------
 * Save the image in a .bmp format and generate the respective information of the image
 *
 *    image: memory direction of the image pixels
 *    height: image height
 *    width: image width
 *    imageFileName: name of the image
 *
 *  This code is a modification obtained at the website: 
 *			https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
 *  Creator: Minhas Kamal, Dec 13, 2017.
 */
void generateBitmapImage(int height, int width, unsigned char *** image, char * imageFileName);


/*
 * Function: createBitmapFileHeader
 * ---------------------------------
 * Create the file header for the image to save
 *
 *    height: image height
 *    width: image width
 *    paddingSize:
 *
 *  This code is a modification obtained at the website: 
 *			https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
 *  Creator: Minhas Kamal, Dec 13, 2017.
 */
unsigned char * createBitmapFileHeader(int height, int width, int paddingSize);


/*
 * Function: createBitmapInfoHeader
 * ---------------------------------
 * Create the information header for the image to save
 *
 *    height: image height
 *    width: image width
 *
 *  This code is a modification obtained at the website: 
 *			https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
 *  Creator: Minhas Kamal, Dec 13, 2017.
 */
unsigned char * createBitmapInfoHeader(int height, int width);



//----------------------------------------------------------------------------------------------------------------------
// FUNCTIONS FOR HANDLE THE IMAGE
//----------------------------------------------------------------------------------------------------------------------
unsigned char *** stringToImage(const int * height, const int * width, char * str_pixels)
{
    unsigned char *** ptr_image = allocateMemorySpaceForImage(height, width);

    char sep[2] = ",";

    char * token = strtok(str_pixels, sep);

    for (int i = *height - 1; i >= 0 ; i--)
        for (int j = *width - 1; j >= 0 ; j--)
            for (int k = COLOR_CHANNELS - 1; k >= 0; k--)
            {
                ptr_image[i][j][k] = (unsigned char)atoi(token);
                token = strtok(NULL, sep);
            }

    return ptr_image;
}

void convertImageToGrayscale(const int * ptr_height, const int * ptr_width, unsigned char *** img)
{
    for (int i = 0; i < *ptr_height; i++)
        for (int j = 0; j < *ptr_width; j++)
        {
            int grayscale_value = (int)((float)(0.30 * img[i][j][R_CHANNEL]) +
                                        (float)(0.59 * img[i][j][G_CHANNEL]) +
                                        (float)(0.11 * img[i][j][B_CHANNEL]));

            //Set the grayscale value in RGB channels
            img[i][j][R_CHANNEL] = grayscale_value;
            img[i][j][G_CHANNEL] = grayscale_value;
            img[i][j][B_CHANNEL] = grayscale_value;
        }
}

unsigned char *** allocateMemorySpaceForImage(const int * ptr_height, const  int * ptr_width)
{
    unsigned char *** img = (unsigned char ***)calloc(*ptr_height, sizeof(unsigned char **));

    if (img == NULL) {
        fprintf(stderr, "Out of memory");
        exit(0);
    }

    for (int i = 0; i < *ptr_height; i++)
    {
        img[i] = (unsigned char **)calloc(*ptr_width, sizeof(unsigned char*));

        if (img[i] == NULL)
        {
            fprintf(stderr, "Out of memory");
            exit(0);
        }

        for (int j = 0; j < *ptr_width; j++)
        {
            img[i][j] = (unsigned char *)calloc(COLOR_CHANNELS, sizeof(unsigned char));

            if (img[i][j] == NULL)
            {
                fprintf(stderr, "Out of memory");
                exit(0);
            }
        }
    }
    return img;
}

void deallocateMemorySpaceOfImage(const int * ptr_height, const  int * ptr_width, unsigned char *** img)
{
    for (int i = 0; i < *ptr_height; i++)
    {
        for (int j = 0; j < *ptr_width; j++)
            free(img[i][j]);

        free(img[i]);
    }
    free(img);
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTIONS FOR IMAGE CLASSIFICATION
//----------------------------------------------------------------------------------------------------------------------
void classifyImageByColor(const int * ptr_height,
                          const int * ptr_width,
                          unsigned char *** rgb_img,
                          int * ptr_color_type)
{
    int R_counter = 0, G_counter = 0, B_counter = 0;

    for (int i = 0; i < *ptr_height; i++)
        for (int j = 0; j < *ptr_width; j++)
        {
            incrementColorCounter(&rgb_img[i][j][R_CHANNEL],
                                  &rgb_img[i][j][G_CHANNEL],
                                  &rgb_img[i][j][B_CHANNEL],
                                  &R_counter, &G_counter, &B_counter);
        }

    classifyImage(&R_counter, &G_counter, &B_counter, ptr_color_type);
}


void incrementColorCounter(const unsigned char * ptr_R_value,
                           const unsigned char * ptr_G_value,
                           const unsigned char * ptr_B_value,
                           int * ptr_R_counter, int * ptr_G_counter, int * ptr_B_Counter)
{
    //RED value predominates or RED by default if (R = G = B)
    if (*ptr_R_value >= *ptr_G_value && *ptr_R_value >= *ptr_B_value)
        (*ptr_R_counter)++;
    //GREEN value predominates or GREEN by default if (R < G = B)
    else if (*ptr_G_value > *ptr_R_value && *ptr_G_value >= *ptr_B_value)
        (*ptr_G_counter)++;
    //BLUE value predominates
    else
        (*ptr_B_Counter)++;
}


void classifyImage(const int * ptr_R_counter,
                   const int * ptr_G_counter,
                   const int * ptr_B_counter,
                   int * ptr_color_type)
{
    //RED image
    if (*ptr_R_counter >= *ptr_G_counter && *ptr_R_counter >= *ptr_B_counter)
        *ptr_color_type = 0;
    //GREEN image
    else if (*ptr_G_counter > *ptr_R_counter && *ptr_G_counter >= *ptr_B_counter)
        *ptr_color_type = 1;
    //BLUE image
    else
        *ptr_color_type = 2;
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTIONS FOR IMAGE FILTERING
//----------------------------------------------------------------------------------------------------------------------
void meanFilter(const int * ptr_height, const int * ptr_width,
                unsigned char *** input_img, unsigned char *** output_img)
{
    int scaled_filter_size = WINDOW_FILTER_SIZE / 2;
    int neighborhood_size = WINDOW_FILTER_SIZE * WINDOW_FILTER_SIZE;
    double sum;

    //Go throughout the pixels of the image
    for (int i = 0; i < *ptr_height; i++)
        for (int j = 0; j < *ptr_width; j++)
        {
            if ((i >= scaled_filter_size && i <= *ptr_height - scaled_filter_size - 1) &&
                (j >= scaled_filter_size && j <= *ptr_width - scaled_filter_size - 1))
            {
                sum = 0;

                //Go throughout the neighborhood of the current pixel
                for (int x = i - scaled_filter_size, nx = 0; nx < WINDOW_FILTER_SIZE; x++, nx++)
                    for (int y = j - scaled_filter_size, ny = 0; ny < WINDOW_FILTER_SIZE; y++, ny++)
                        //Add the pixel to the current neighborhood
                        sum += input_img[x][y][R_CHANNEL];

                int mean = (int)round(sum / neighborhood_size);

                //Set the median value of the neighborhood of the current pixel in RGB channels
                output_img[i][j][R_CHANNEL] = mean;
                output_img[i][j][G_CHANNEL] = mean;
                output_img[i][j][B_CHANNEL] = mean;
            }
            //Keep border pixels value unchanged
            else
            {
                output_img[i][j][R_CHANNEL] = input_img[i][j][R_CHANNEL];
                output_img[i][j][G_CHANNEL] = input_img[i][j][R_CHANNEL];
                output_img[i][j][B_CHANNEL] = input_img[i][j][R_CHANNEL];
            }
        }
}


void medianFilter(const int * ptr_height, const int * ptr_width,
                  unsigned char *** input_img, unsigned char *** output_img)
{
    int scaled_filter_size = WINDOW_FILTER_SIZE / 2;
    int neighborhood_size = WINDOW_FILTER_SIZE * WINDOW_FILTER_SIZE;
    unsigned char * neighborhood = calloc(neighborhood_size, sizeof(unsigned char));

    //Go throughout the pixels of the image
    for (int i = 0; i < *ptr_height; i++)
        for (int j = 0; j < *ptr_width; j++)
        {
            if ((i >= scaled_filter_size && i <= *ptr_height - scaled_filter_size - 1) &&
                (j >= scaled_filter_size && j <= *ptr_width - scaled_filter_size - 1))
            {
                //Go throughout the neighborhood of the current pixel
                for (int x = i - scaled_filter_size, nx = 0; nx < WINDOW_FILTER_SIZE; x++, nx++)
                    for (int y = j - scaled_filter_size, ny = 0; ny < WINDOW_FILTER_SIZE; y++, ny++)
                        //Add the pixel to the current neighborhood
                        *(neighborhood + nx * WINDOW_FILTER_SIZE + ny) = input_img[x][y][R_CHANNEL];

                int median = calculateMedian(neighborhood, neighborhood_size);

                //Set the median value of the neighborhood of the current pixel in RGB channels
                output_img[i][j][R_CHANNEL] = median;
                output_img[i][j][G_CHANNEL] = median;
                output_img[i][j][B_CHANNEL] = median;
            }
            //Keep border pixels value unchanged
            else
            {
                output_img[i][j][R_CHANNEL] = input_img[i][j][R_CHANNEL];
                output_img[i][j][G_CHANNEL] = input_img[i][j][R_CHANNEL];
                output_img[i][j][B_CHANNEL] = input_img[i][j][R_CHANNEL];
            }
        }

    free(neighborhood);
}


int calculateMedian(unsigned char * arr, int length)
{
    //Sort the elements of the array
    for (int i = 0; i < length; i++)
        for (int j = i + 1; j < length; j++)
        {
            if (*(arr + j) < *(arr + i))
            {
                int temp = *(arr + i);
                *(arr + i) = *(arr + j);
                *(arr + j) = temp;
            }
        }

    return *(arr + (length / 2));
}

//----------------------------------------------------------------------------------------------------------------------
// FUNCTIONS FOR SAVE BITMAP IMAGE
//----------------------------------------------------------------------------------------------------------------------
void generateBitmapImage(int height, int width, unsigned char *** image, char * imageFileName)
{
    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (width * COLOR_CHANNELS) % 4) % 4;

    unsigned char* fileHeader = createBitmapFileHeader(height, width, paddingSize);
    unsigned char* infoHeader = createBitmapInfoHeader(height, width);

    FILE* imageFile = fopen(imageFileName, "wb");

    fwrite(fileHeader, 1, fileHeaderSize, imageFile);
    fwrite(infoHeader, 1, infoHeaderSize, imageFile);

    //------------------------------------------------------------------------------------------------------------------
    unsigned char img_temp[height][width][COLOR_CHANNELS];
    for (int k = 0; k < 3; k++)
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                img_temp[i][j][k] = image[i][j][k];
    unsigned char * img = (unsigned char *)img_temp;
    //------------------------------------------------------------------------------------------------------------------

    for(int i = 0; i < height; i++)
    {
        fwrite(img+(i * width * COLOR_CHANNELS), COLOR_CHANNELS, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char * createBitmapFileHeader(int height, int width, int paddingSize)
{
    int fileSize = fileHeaderSize + infoHeaderSize + (COLOR_CHANNELS * width + paddingSize) * height;

    static unsigned char fileHeader[] = {
            0,0,     /// signature
            0,0,0,0, /// image file size in bytes
            0,0,0,0, /// reserved
            0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize    );
    fileHeader[ 3] = (unsigned char)(fileSize>> 8);
    fileHeader[ 4] = (unsigned char)(fileSize>>16);
    fileHeader[ 5] = (unsigned char)(fileSize>>24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);

    return fileHeader;
}

unsigned char * createBitmapInfoHeader(int height, int width)
{
    static unsigned char infoHeader[] = {
            0,0,0,0, /// header size
            0,0,0,0, /// image width
            0,0,0,0, /// image height
            0,0,     /// number of color planes
            0,0,     /// bits per pixel
            0,0,0,0, /// compression
            0,0,0,0, /// image size
            0,0,0,0, /// horizontal resolution
            0,0,0,0, /// vertical resolution
            0,0,0,0, /// colors in color table
            0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(infoHeaderSize);
    infoHeader[ 4] = (unsigned char)(width    );
    infoHeader[ 5] = (unsigned char)(width>> 8);
    infoHeader[ 6] = (unsigned char)(width>>16);
    infoHeader[ 7] = (unsigned char)(width>>24);
    infoHeader[ 8] = (unsigned char)(height    );
    infoHeader[ 9] = (unsigned char)(height>> 8);
    infoHeader[10] = (unsigned char)(height>>16);
    infoHeader[11] = (unsigned char)(height>>24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(COLOR_CHANNELS * 8);

    return infoHeader;
}
