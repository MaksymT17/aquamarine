
#include "JpgExtractor.h"
#include "common/exceptions/FileAccessException.hpp"
#include "common/exceptions/AllocationException.hpp"
#include <future>
#include <vector>
#include <jpeglib.h>

namespace am::extraction
{
    using namespace common::types;

    Matrix<Color24b> JpgExtractor::readFile(const std::string &fileName)
    {
        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;

        FILE *infile = fopen(fileName.c_str(), "rb");
        if (!infile)
        {
            std::string errMsg = "File '" + fileName + "' could not be found!";
            throw common::exceptions::FileAccessException(errMsg);
        }

        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);
        jpeg_stdio_src(&cinfo, infile);
        jpeg_read_header(&cinfo, TRUE);
        jpeg_start_decompress(&cinfo);

        JSAMPARRAY buffer;
        buffer = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * cinfo.output_height);
        for (int i = 0; i < cinfo.output_height; i++)
        {
            buffer[i] = (JSAMPROW)malloc(sizeof(JSAMPLE) * cinfo.output_width * cinfo.output_components);
        }
        while (cinfo.output_scanline < cinfo.output_height)
        {
            jpeg_read_scanlines(&cinfo, buffer + cinfo.output_scanline, cinfo.output_height - cinfo.output_scanline);
        }
        Matrix<Color24b> data(cinfo.output_width, cinfo.output_height);
        for (int i = 0; i < cinfo.output_height; i++)
        {
            for (int j = 0; j < cinfo.output_width; j++)
            {
                Color24b c24 = reinterpret_cast<Color24b &>(buffer[i][j]);
                // printf("%d %d %d\n", c24.r, c24.g, c24.b);
                data(i, j) = c24;
            }
        }
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);

        return data;
    }
} // namespace am::extraction
