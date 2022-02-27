//======================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2021, 2022 James Steele. All Rights Reserved.
//======================================================================================================================

#ifndef __I3D_TEXTUREIMAGELOADER_H__
#define __I3D_TEXTUREIMAGELOADER_H__

#include <mutex>
#include "i3d/core/Types.h"
#include "i3d/core/FileSystem.h"
//#include "i3d/mem/Mem_local.h"

namespace i3d { namespace texture {

    /// Utility class for handling the loading of non-block compressed images into memory. This is a singleton resource
    /// and is thread locked during usage to be thread safe. With this in mind,  it is possible that this can become a perf
    /// bottleneck when dealing with loads.
    class TextureImageLoader {
    public:
        static const int32_t MAX_WIDTH = 4096;
        static const int32_t MAX_HEIGHT = 4096;
        static const int32_t MAX_BPP = 4;
        static const int32_t MAX_BYTES_SIZE = MAX_WIDTH * MAX_HEIGHT * MAX_BPP;
        
        typedef std::vector<uint8_t> byte_vector_t;
        typedef std::vector<uint8_t*> byte_ptr_vector_t;
        
        std::mutex              m_lock;
        int32_t                 m_width;
        int32_t                 m_height;
        int32_t                 m_bpp;
        int32_t                 m_pitch;
        byte_vector_t           m_bytes;
        byte_ptr_vector_t       m_rows;
        byte_vector_t           m_tempRow;
        
        TextureImageLoader();
        
        ~TextureImageLoader();
        
        /// Loads an image into memory
        ///@param file The opened file that we are loading the image from
        ///@param path C-string that specifies the path of the image to load
        ///@param errorMsg Reference to the ti::String to hold any messages related to loading errors
        ///@returns True on success, false on error with the error information being stored in errorMsg
        bool Load( handle_t file, const char* path, std::string& errorMsg);
        
        /// One-shot call for loading an image file directly into a texture. The texture is assumed to be uninitialized.
        ///@param texture Pointer to the texture to load the image into. The texture is assumed to be uninitialised.
        ///@param path C-string that specifies the path of the image to load
        ///@param errorMsg Reference to the ti::String to hold any messages related to loading errors
        ///@returns True on success, false on error with the error information being stored in errorMsg
        bool LoadToTexture( handle_t texture, handle_t file, const char* path, std::string& errorMsg);
        
        /// Locks the instance for the thread that is using it
        void Lock() { m_lock.lock(); }
        
        /// Unlocks the instance for threrading
        void Unlock() { m_lock.unlock(); }

    protected:
        /// Initialises the internal image state
        ///@param width Width of the image in pixels
        ///@param height Height of the image in pixels
        ///@param bpp Colour deoth of the image in bytes-per-pixel
        ///@param errorMsg Reference to the ex:;String that holds any error information
        ///@returns True on success, false on error with the error information being stored in errorMsg
        bool Setup(int32_t width, int32_t height, int32_t bpp, std::string& errorMsg);
        
        /// Performs loading of a png image
        ///@param file Pointer to ther opened file that we load the png from
        ///@param errorMsg Reference to the ti::String to hold any messages realted to loading errors
        ///@returns True on success, false on error with the error information being stored in errorMsg
        bool LoadPng( handle_t file, std::string& errorMsg);
        
        /// Performs loading of a jpeg image
        ///@param file Pointer to the opened file that we load the jpg from
        ///@param errorMsg Reference to the ti::String to hold any messages realted to loading errors
        ///@returns Trueon success, false on error with the error information being stored in errorMsg
        bool LoadJpg( handle_t file, std::string& errorMsg);
        
        /// Expands a RGB row help in m_tempRow to RBA in m_bytes at the row specified by h
        ///@param h The destination row index of the expanded 32-bit image row
        void ExpandRowToRGBA(int32_t h);
    };
}}

#endif
