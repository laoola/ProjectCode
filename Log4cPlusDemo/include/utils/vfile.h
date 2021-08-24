#pragma once
#include <vector>
#include <string>
#include <fstream>

namespace utils
{
    namespace vfs
    {
        bool IsLinkExist(std::string path);
        bool IsFileExist(std::string path);
        std::string PathJoin(std::string dir, std::string file);

        class IFile
        {
        protected:
            typedef enum {
                PARSE_OK = 0,
                PARSE_ERROR,
                PARSE_CONTINUE
            }ParseStatus;

        private:
            virtual ParseStatus parse_line(std::string line);
            std::string m_data;

        public:
            IFile(){}
            IFile(std::string f);
            virtual ~IFile();
            virtual bool close();
            bool open();
            bool lookup();
            std::string data();

        protected:
            std::string m_path;
            std::fstream m_file;
            std::ios_base::openmode m_open_type;
        };

        class IWFile : public IFile
        {
        public:
            virtual ~IWFile();
            virtual bool close();
            IWFile(std::string f);
            bool add_line(std::string str_line);  // 在文件末尾,添加一行内容
        };


        class FileLine : public IFile
        {
        public:
            typedef std::vector<std::string>::iterator iterator;

            FileLine(std::string file, std::size_t line_no = 1);
            std::string get();
            std::string operator[] (int idx);
            std::vector<std::string>& data();

        private:
            virtual ParseStatus parse_line(std::string line);
            std::size_t m_cur_line_no;
            std::size_t m_target_line_no;
            std::string m_line;
            std::vector<std::string> m_data;
        };
    }
}