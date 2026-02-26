#include "utils/zip_util.h"

bool ExtractZipTo(const std::string& zipPath, const std::string& outputDir)
{
    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_reader_init_file(&zip, zipPath.c_str(), 0)) {
        std::cerr << "Failed to open zip: " << zipPath << "\n";
        return false;
    }

    fs::create_directories(outputDir);
    mz_uint fileCount = mz_zip_reader_get_num_files(&zip);
    for (mz_uint i = 0; i < fileCount; i++) {
        mz_zip_archive_file_stat stat;
        if (!mz_zip_reader_file_stat(&zip, i, &stat))
            continue;

        std::string outPath = outputDir + "/" + stat.m_filename;

        if (mz_zip_reader_is_file_a_directory(&zip, i)) {
            fs::create_directories(outPath);
            continue;
        }

        fs::create_directories(fs::path(outPath).parent_path());

        if (!mz_zip_reader_extract_to_file(&zip, i, outPath.c_str(), 0)) {
            std::cerr << "Failed extracting " << stat.m_filename << "\n";
        }
    }

    mz_zip_reader_end(&zip);
    return true;
}
