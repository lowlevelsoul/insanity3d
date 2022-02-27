//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "xe/util/Image.h"

#include "png.h"
#include "pngstruct.h"

namespace xe {

    #define PNG_SIG_LENGTH 8
        
    //===============================================================================================================================
    static void PngReadFunc( png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead ) {
        i3d::File * file = (i3d::File*)png_ptr->io_ptr;
        file->Read( outBytes, 1, byteCountToRead );
    }

    //===============================================================================================================================
    static void PngWriteFunc( png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead ) {
        i3d::File * file = (i3d::File*)png_ptr->io_ptr;
        file->Write( outBytes, 1, byteCountToRead );
    }

    //===============================================================================================================================
    static void PngWriteFlushFunc(png_structp flushPtr) {
        
    }

    //===============================================================================================================================
    bool Image::LoadPng(const char* path, bool forceAlpha) {
        
        i3d::ScopedFile file( path, "rb" );
        if ( file.IsValid() == false ) {
            return false;
        }
        
        uint8_t pngSignature[ PNG_SIG_LENGTH ];
        file->Read( pngSignature, 1, PNG_SIG_LENGTH );

        if ((png_check_sig( pngSignature, PNG_SIG_LENGTH )) == false) {
            return false;
        }

        png_structp png_ptr = NULL;
        png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );

        if (png_ptr == NULL) {
            return false;
        }

        // get PNG image data info struct (memory is allocated by libpng)
        png_infop info_ptr = nullptr;
        info_ptr = png_create_info_struct( png_ptr );
        if (info_ptr == nullptr) {
            return false;
        }

        png_set_read_fn( png_ptr, (i3d::File*) file, PngReadFunc );

        // tell libpng we already read the signature
        png_set_sig_bytes( png_ptr, PNG_SIG_LENGTH );

        // Read info from the png
        png_read_info( png_ptr, info_ptr );

        png_uint_32 width = 0;
        png_uint_32 height = 0;
        int bitDepth = 0;
        int colorType = -1;
        png_get_IHDR( png_ptr, info_ptr, &width, &height, &bitDepth, &colorType, NULL, NULL, NULL );

        size_t pitch = png_get_rowbytes( png_ptr, info_ptr );
        int32_t bpp = (int32_t) (pitch / width);
        
        Image::FORMAT format = (bpp == 3) ? FORMAT_RGB_U8 : FORMAT_RGBA_U8;
        
        if ((forceAlpha == true) && (format != FORMAT_RGBA_U8)) {
            // We want to force rgba format for the image being loaded, and the image
            // we are loading is only rgb - so expand to rgba after we read each row.
            Initialise(width, height, FORMAT_RGBA_U8);
            
            uint8_t* tmpRow = new uint8_t[m_width * 3];
            assert(tmpRow != nullptr);
            
            for ( uint32_t h = 0; h < height; ++h ) {
                png_read_row( png_ptr, (png_bytep) tmpRow, NULL );
                ExpandRoRgba(h, tmpRow);
            }
                        
            delete []tmpRow;
        }
        else {
            // Just read the image in as normal
            Initialise(width, height, format);
            
            for ( uint32_t h = 0; h < height; ++h ) {
                png_read_row( png_ptr, (png_bytep) m_rows[h], NULL );
            }

            png_destroy_read_struct( &png_ptr, &info_ptr, NULL );
        }
        
        return true;
    }


    //===============================================================================================================================
    bool Image::SavePng( i3d::File * file) {
        
        bool formatValid = (m_format != FORMAT_RGB_U8) || (m_format != FORMAT_RGBA_U8);
        if (formatValid == false) {
            printf("Invalid formay for wroting to png");
            return false;
        }
        
        png_byte color_type;
        png_byte bit_depth;

        png_structp png_ptr;
        png_infop info_ptr;
        
        color_type = PNG_COLOR_TYPE_RGB;
        if (IsFormatRgb() == false) {
            color_type = PNG_COLOR_TYPE_RGBA;
        }
        bit_depth = 8;

        /* initialize stuff */
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        png_set_write_fn( png_ptr, &file, PngWriteFunc, PngWriteFlushFunc);

        if (!png_ptr) {
            printf("[write_png_file] png_create_write_struct failed");
            return false;
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            printf("[write_png_file] png_create_info_struct failed");
            return false;
        }

        if (setjmp(png_jmpbuf(png_ptr))) {
            printf("[write_png_file] Error during init_io");
            return false;
        }

        /* write header */
        if (setjmp(png_jmpbuf(png_ptr))) {
            printf("[write_png_file] Error during writing header");
            return false;
        }

        png_set_IHDR(png_ptr, info_ptr, GetWidth(), GetHeight(),
                     bit_depth, color_type, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

        png_write_info(png_ptr, info_ptr);


        /* write bytes */
        if (setjmp(png_jmpbuf(png_ptr))) {
            printf("[write_png_file] Error during writing bytes");
            return false;
        }

        png_write_image(png_ptr, &m_rows[0]);

        /* end write */
        if (setjmp(png_jmpbuf(png_ptr))) {
            printf("[write_png_file] Error during end of write");
            return false;
        }

        png_write_end(png_ptr, NULL);
        
        return true;
    }
}
