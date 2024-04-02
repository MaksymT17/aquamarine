
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

        JSAMPLE* tempdata = new JSAMPLE[cinfo.output_width * cinfo.output_components];
        Matrix<Color24b> data(cinfo.output_width, cinfo.output_height);
        size_t rowid =0;
        while (cinfo.output_scanline < cinfo.output_height)
        {
            jpeg_read_scanlines(&cinfo, &tempdata, 1);
            for (int j = 0; j < cinfo.output_width; j++)
            {
                //printf("[%d]c:%d_%d_%d ", j, tempdata[j*3],tempdata[j*3+1], tempdata[j*3+2] );
                data(rowid, j) = {tempdata[j*cinfo.output_components],tempdata[j*cinfo.output_components+1], tempdata[j*cinfo.output_components+2]};
            }
            //printf("\n");
            rowid++;
        }
        delete tempdata;
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return data;
    }
} // namespace am::extraction
