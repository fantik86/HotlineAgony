#include "utils.h"

animation_sequence_t packAnimationSequence(const std::string dirpath) {
    animation_sequence_t packed_textures;
    std::filesystem::path full_path = std::filesystem::path(GetApplicationDirectory()) / dirpath;
    std::vector<std::filesystem::directory_entry> files;

    // Сбор всех файлов из директории
    for (const auto& file : std::filesystem::directory_iterator(full_path)) {
        if (file.is_regular_file()) {
            std::string extension = file.path().extension().string();

            // Фильтруем по расширениям (например, изображения)
            if (extension == ".png" || extension == ".jpg" || extension == ".bmp") {
                files.push_back(file);
            }
        }
    }

    // Сортировка по числовому индексу в имени файла (например, 1.png, 2.png)
    std::sort(files.begin(), files.end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
        std::string filename_a = a.path().stem().string(); // Без расширения
        std::string filename_b = b.path().stem().string(); 

        int index_a = std::stoi(filename_a);
        int index_b = std::stoi(filename_b);

        return index_a < index_b;
    });

    // Загрузка текстур в вектор
    for (const auto& file : files) {
        Texture2D texture = LoadTexture(file.path().string().c_str());
        SetTextureFilter(texture, TEXTURE_FILTER_POINT); // Настройка фильтра для текстур
        packed_textures.push_back(texture);
    }

    return packed_textures;
}