//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "i3d/core/Sys.h"
#include "i3d/core/FileSystem.h"
#include "i3d/render/Texture.h"
#include "i3d/render/TextureImageLoader.h"

#ifdef XE_ENABLE_PNG
#   include "png.h"
#   include "pngstruct.h"
#endif

#include <setjmp.h>

#define PNG_SIG_LENGTH 8
#define FORCE_RGBA


namespace i3d { namespace texture {
    
#if 0
    
#ifdef XE_ENABLE_PNG
    static void PngReadFunc( png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead ) {
        handle_t* file = (handle_t*)png_ptr->io_ptr;
        xe::fs::FileRead( *file, outBytes, 1, byteCountToRead );
    }
#endif

    //======================================================================================================================
    TextureImageLoader::TextureImageLoader() {
    }

    //======================================================================================================================
    TextureImageLoader::~TextureImageLoader() {

    }

    //======================================================================================================================
    void TextureImageLoader::ExpandRowToRGBA(int32_t h) {
#if XE_ENABLE_PNG
        uint8_t* dst = m_rows[h];
        uint8_t* src = &m_tempRow[0];
        
        for(int32_t w=0; w < m_width; ++w) {
            dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2];
            dst[3] = 1;
            
            dst += 4;
            src += 3;
        }
    #endif
    }

    //======================================================================================================================
    bool TextureImageLoader::Load( handle_t file, const char* path, std::string& errorMsg) {
        
        std::string ext;
        bool gotExt = fs->GetExtension(ext, path);
        if (gotExt == false) {
            errorMsg = sys->Vformat("File for image at %s does not have an extension", path);
            return false;
        }

        bool loadRes = false;
        const char* extCStr = ext.c_str();
        
        if (strcmp(extCStr, "png") == 0) {
            loadRes = LoadPng( file, errorMsg);
        }
        else {
            errorMsg = sys->Vformat("Unsupported image file type \"%s\"", ext.c_str());
        }
        
        return loadRes;
    }

    //======================================================================================================================
    bool TextureImageLoader::LoadToTexture( handle_t tex, handle_t file, const char* path, std::string& errorMsg) {
        std::lock_guard<std::mutex> lock(m_lock);
        
        bool loadOk = Load(file, path, errorMsg);
        if (loadOk == false) {
            return false;
        }
        
        if ((m_bpp != 3) && (m_bpp != 4)) {
            errorMsg = sys->Vformat("Texture bytes-per-pixel is not a valid value. Expected 3 or 4 but got %i", m_bpp);
            return false;
        }
        
        texture::FORMAT fmt = (m_bpp == 3) ? texture::FORMAT_RGB_U8 : texture::FORMAT_RGBA_U8;
        
        texture::Create( tex, fmt, m_width, m_height, 0);
        texture::Write( tex, &m_bytes[0], 0);
        
        return true;
    }

    //======================================================================================================================
    bool TextureImageLoader::Setup(int32_t width, int32_t height, int32_t bpp, std::string& errorMsg) {
        m_width = width;
        m_height = height;
        m_bpp = bpp;
        m_pitch = width * bpp;
        
        size_t size = height * m_pitch;
        
        m_bytes.resize(size);
        m_rows.resize(height);
        m_tempRow.resize(m_pitch);
        
        uint8_t * currRow = &m_bytes[0];
        for(int32_t h=0; h < m_height; ++h) {
            m_rows[h] = currRow;
            currRow += m_pitch;
        }
        
        return true;
    }

    //======================================================================================================================
    bool TextureImageLoader::LoadPng( handle_t file, std::string& errorMsg) {
     
#ifdef XE_ENABLE_PNG

        uint8_t pngSignature[ PNG_SIG_LENGTH ];
        fs::FileRead(file, pngSignature, 1, PNG_SIG_LENGTH );

        if ((png_check_sig( pngSignature, PNG_SIG_LENGTH )) == false) {
            errorMsg = "Invalid PNG file";
            return false;
        }

        png_structp png_ptr = NULL;
        png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );

        if (png_ptr == NULL) {
            errorMsg = "Error creating png struct";
            return false;
        }

        // get PNG image data info struct (memory is allocated by libpng)
        png_infop info_ptr = nullptr;
        info_ptr = png_create_info_struct( png_ptr );
        if (info_ptr == nullptr) {
            errorMsg = "Error creating png info struct";
            return false;
        }

        png_set_read_fn( png_ptr, &file, PngReadFunc );

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
        int32_t dstBpp = bpp;
        
    #if defined(FORCE_RGBA)
        dstBpp = 4;
    #endif
        
        bool startOk = Setup(width, height, dstBpp, errorMsg);
        if (startOk == false) {
            return false;
        }

        for ( uint32_t h = 0; h < height; ++h ) {
    #if defined(FORCE_RGBA)
            if (bpp == 3) {
                png_read_row( png_ptr, (png_bytep) &m_tempRow[0], NULL );
                ExpandRowToRGBA(h);
            }
            else {
                png_read_row( png_ptr, (png_bytep)m_rows[h], NULL );
            }
    #else
            png_read_row( png_ptr, (png_bytep)m_rows[h], NULL );
    #endif
        }

        png_destroy_read_struct( &png_ptr, &info_ptr, NULL );
        
#endif
        
        return true;
    }

    //======================================================================================================================
    bool TextureImageLoader::LoadJpg( handle_t file, std::string& errorMsg) {
    #if 1
        errorMsg = "Jpeg images are not supported";
        return false;
    #else
        // Read in the image to a temp buffer
        
        size_t len = file->Length();
        ScopedTempResourceBuffer tmpBuffer(len);
        uint8_t* bin = (uint8_t*) tmpBuffer.GetPointer();
        size_t amtRead = file->Read(bin, sizeof(uint8_t), len);
        if (amtRead != len) {
            goto bail;
        }
        
        jpeg_decompress_struct jinfo;
        jerror_mgr jerr;

        jinfo.err = jpeg_std_error(&jerr.base);
        jerr.base.error_exit = jerror_exit;
        jerr.base.output_message = joutput_message;
        jpeg_create_decompress(&jinfo);

        if (setjmp(jerr.jmp)) {
            goto bail;
        }

        jpeg_mem_src(&jinfo, bin, len);

        if (jpeg_read_header(&jinfo, TRUE) != JPEG_HEADER_OK) {
            goto bail;
        }

        jinfo.dct_method = JDCT_FLOAT; // change this to JDCT_ISLOW on Android/iOS

        if (!jpeg_start_decompress(&jinfo)) {
            goto bail;
        }

        if (jinfo.num_components != 1 && jinfo.num_components != 3) {
            goto bail;
        }
        
    #if defined(FORCE_RGBA)
        // Note - we always expand jpegs since they don't have an alpha channel
        Setup(jinfo.output_width, jinfo.output_height, 4, errorMsg);
        {
            int32_t rowCount = 0;
            while (jinfo.output_scanline < jinfo.output_height)
            {
                JSAMPROW ptr = m_tempRow;
                if (jpeg_read_scanlines(&jinfo, &ptr, 1) != 1) {
                    goto bail;
                }
                
                ExpandRowToRGBA(rowCount);
                ++rowCount;
            }
        }
    #else
        Setup(jinfo.output_width, jinfo.output_height, jinfo.num_components, errorMsg);
        {
            int32_t rowCount = 0;
            while (jinfo.output_scanline < jinfo.output_height)
            {
                JSAMPROW ptr = m_rows[rowCount];
                if (jpeg_read_scanlines(&jinfo, &ptr, 1) != 1) {
                    goto bail;
                }
                
                ++rowCount;
            }
        }
    #endif

        if (!jpeg_finish_decompress(&jinfo)) {
            goto bail;
        }

        jpeg_destroy_decompress(&jinfo);
        return true;

    bail:
        errorMsg = "Error decompressing jpeg image";
        jpeg_destroy_decompress(&jinfo);
        return false;
    #endif
    }
    
#endif
}}
