//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#include "toolimage/ToolImage.h"
#include "jpeglib.h"
#include <setjmp.h>
    
//===============================================================================================================================

struct jerror_mgr
{
    jpeg_error_mgr base;
    jmp_buf        jmp;
};

//===============================================================================================================================
METHODDEF(void) jerror_exit(j_common_ptr jinfo)
{
    jerror_mgr* err = (jerror_mgr*)jinfo->err;
    longjmp(err->jmp, 1);
}

//===============================================================================================================================
METHODDEF(void) joutput_message(j_common_ptr)
{
}

//===============================================================================================================================
bool ToolImage::LoadJpeg(const char* path, bool forceAlpha) {
    // Read in the image to a temp buffer
    i3d::ScopedFile file(path, "rb");
    if (file.IsValid() == false ) {
        return false;
    }
    
    size_t len = file->Length();
    
    uint8_t* bin = new uint8_t[len];
    size_t amtRead = file->Read( bin, sizeof(uint8_t), len);
    if (amtRead != len) {
        return false;
    }
    
    
    jpeg_decompress_struct jinfo;
    jerror_mgr jerr;

    jinfo.err = jpeg_std_error(&jerr.base);
    jerr.base.error_exit = jerror_exit;
    jerr.base.output_message = joutput_message;
    jpeg_create_decompress(&jinfo);

    if (setjmp(jerr.jmp)) {
        delete []bin;
        return false;
    }

    jpeg_mem_src(&jinfo, bin, len);

    if (jpeg_read_header(&jinfo, TRUE) != JPEG_HEADER_OK) {
        delete []bin;
        return false;
    }

    jinfo.dct_method = JDCT_FLOAT; // change this to JDCT_ISLOW on Android/iOS

    if (!jpeg_start_decompress(&jinfo)) {
        delete []bin;
        return false;
    }

    if (jinfo.num_components != 1 && jinfo.num_components != 3) {
        delete []bin;
        return false;
    }
    
    FORMAT format = (jinfo.num_components == 3) ? FORMAT_RGB_U8 : FORMAT_RGBA_U8;
    
    if ((forceAlpha == true) && (format != FORMAT_RGBA_U8)) {
        // We want to force rgba format for the image being loaded.
        Initialise(jinfo.output_width, jinfo.output_height, FORMAT_RGBA_U8);
        
        // Create a temp image that we'll load the rgb image into
        ToolImage::Ptr tmpImg = new ToolImage;
        tmpImg->Initialise(jinfo.output_width, jinfo.output_height, FORMAT_RGB_U8);
        
        int32_t rowCount = 0;
        while (jinfo.output_scanline < jinfo.output_height)
        {
            JSAMPROW ptr = tmpImg->m_rows[rowCount];
            if (jpeg_read_scanlines(&jinfo, &ptr, 1) != 1) {
                delete []bin;
                return false;
            }
            
            ++rowCount;
        }
        
        // Convert th rgb image into rgba
        for(uint32_t h=0; h < m_height; ++h) {
            this->ExpandRoRgba(h, tmpImg->m_rows[h]);
        }
    }
    else {
    
        Initialise(jinfo.output_width, jinfo.output_height, format);
        {
            int32_t rowCount = 0;
            while (jinfo.output_scanline < jinfo.output_height)
            {
                JSAMPROW ptr = m_rows[rowCount];
                if (jpeg_read_scanlines(&jinfo, &ptr, 1) != 1) {
                    delete []bin;
                    return false;
                }
                
                ++rowCount;
            }
        }
    }

    if (!jpeg_finish_decompress(&jinfo)) {
        delete []bin;
        return false;
    }

    jpeg_destroy_decompress(&jinfo);
    delete []bin;
    
    return true;
}
