#include "image_processing.h"

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
