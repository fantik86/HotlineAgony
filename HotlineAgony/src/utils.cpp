#include "utils.h"
#include <algorithm>

animation_sequence_t packAnimationSequence(const std::string dirpath) {
    animation_sequence_t packed_textures;
    std::filesystem::path full_path = std::filesystem::path(GetApplicationDirectory()) / dirpath;
    std::vector<std::filesystem::directory_entry> files;

    for (const auto& file : std::filesystem::directory_iterator(full_path)) {
        if (file.is_regular_file()) {
            std::string extension = file.path().extension().string();

            if (extension == ".png" || extension == ".jpg" || extension == ".bmp") {
                files.push_back(file);
            }
        }
    }

    std::sort(files.begin(), files.end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
        std::string filename_a = a.path().stem().string();
        std::string filename_b = b.path().stem().string(); 

        int index_a = std::stoi(filename_a);
        int index_b = std::stoi(filename_b);

        return index_a < index_b;
    });

    for (const auto& file : files) {
        Texture2D texture = LoadTexture(file.path().string().c_str());
        SetTextureFilter(texture, TEXTURE_FILTER_POINT);
        packed_textures.push_back(texture);
    }

    return packed_textures;
}

