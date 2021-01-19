// © 2021 Joseph Cameron - All Rights Reserved

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <streambuf>
#include <vector>

/// \brief article datamodel
///
/// constructed instance represents the usable data of a single article
/// correctness of data on disk is tested at ctor time, throwing if incorrect.
///
class article final
{
private:
    /// \brief name of the file containing the name article's description
    static const std::string c_description_file_name; 

public:
    std::string name;
    //repo_url //if repourl file !exist then generate
    std::string description;

    article(const std::filesystem::path &aPath)
    : repository_name(aPath.filename())
    , description([&aPath]()
    {
        auto desc_path = aPath / c_description_file_name;

        if (!std::filesystem::exists(desc_path)) throw std::runtime_error(
            std::string("description file not present for article: ") 
                + aPath.filename().string());

        if (!std::filesystem::is_regular_file(desc_path)) throw std::runtime_error(
            std::string("description file must be a regular file: ")
                + aPath.filename().string());

        std::ifstream t(desc_path.string());

        return std::string((std::istreambuf_iterator<char>(t)), 
            std::istreambuf_iterator<char>());
    }())
    {}
};

const std::string article::c_description_file_name = "description"; 

///////////// unsorted
using article_collection_type = std::vector<article>;

const std::string c_path_to_articles("/home/joe/Temp/articles");

int main(int count, char **args)
{
    article_collection_type articles;

    for (const auto &entry : std::filesystem::directory_iterator(c_path_to_articles))
        if (std::filesystem::is_directory(entry))
            articles.push_back(entry.path());
    
    std::cout << "======= HEADER =======\n";

    for (const auto &article : articles)
    {
        std::cout << article.description << "\n";
    }
    
    std::cout << "======= FOOTER =======\n";

    return EXIT_SUCCESS;
}

