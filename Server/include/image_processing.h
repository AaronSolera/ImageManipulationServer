#ifndef _IMAGE_PROCESSING_H
#define _IMAGE_PROCESSING_H

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
 */
unsigned char * createBitmapFileHeader(int height, int width, int paddingSize);


/*
 * Function: createBitmapInfoHeader
 * ---------------------------------
 * Create the information header for the image to save
 *
 *    height: image height
 *    width: image width
 */
unsigned char * createBitmapInfoHeader(int height, int width);

#endif //IMG_PROCESSING_IMAGE_PROCESSING_H
