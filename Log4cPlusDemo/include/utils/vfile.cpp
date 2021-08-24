#include "vfile.h"
#include "common.h"
#include <sys/stat.h>
#include <sys/types.h>


namespace utils
{
    namespace vfs
    {
        /*
        ** 判断符号链接是否存在
        */
       bool IsLinkExist(std::string path)
        {
            struct stat st;
            if (0 == lstat(path.c_str(), &st) && S_ISLNK(st.st_mode))
            {
                return true;
            }
            return false;
        }

        /*
        ** 判断文件是否存在
        */
        bool IsFileExist(std::string path)
        {
            struct stat st;
            if (0 == lstat(path.c_str(), &st) && S_ISREG(st.st_mode))
            {
                return true;
            }
            return false;
        }

        /*
        ** 文件路径拼接
        ** 成功返回拼接后路径，失败返回file参数
        */
        std::string PathJoin(std::string dir, std::string file)
        {
            if (dir.empty() || file.empty())
            {
                return file;
            }

            std::string path(dir);
            if (path.at(path.length()-1) != '/')
            {
                path.append("/");
            }
            path.append(file);

            return path;
        }

        /*
        ** 文件操作基类
        */
        IFile::IFile(std::string f) : m_path(f), m_open_type(std::ios::in)
        {
            open();
        }

        IFile::~IFile()
        {
            close();
        }

        bool IFile::close()
        {
            if (m_file.is_open())
            {
                m_file.close();
            }
            return !m_file.is_open();
        }

        bool IFile::open()
        {
            if (m_open_type & std::ios::out)
                m_open_type = std::ios::out;

            if (m_file.is_open())
                m_file.close();

            m_file.open(m_path.c_str(), m_open_type);
            return m_file.is_open();
        }

        bool IFile::lookup()
        {
            if (!m_file.is_open())
                return false;

            ParseStatus ret;
            std::string line;
            m_file.seekp(0, std::ios::beg);
            while (std::getline(m_file, line))
            {
                if((ret = parse_line(line)) != PARSE_CONTINUE)
                {
                    break;
                }
            }
            
            return ret != PARSE_ERROR;
        }

        std::string IFile::data()
        {
            return m_data;
        }

        IFile::ParseStatus IFile::parse_line(std::string line)
        {
            m_data += line;
            return PARSE_CONTINUE;
        }

        IWFile::IWFile(std::string f) : IFile(f)
        {
            m_open_type|=std::ios::out;
        }

        IWFile::~IWFile()
        {
            close();
        }

        bool IWFile::close()
        {
            if(m_file.is_open())
            {
                m_file.flush();
                m_file.close();
            }
            return !m_file.is_open();
        }

        bool IWFile::add_line(std::string str_line)
        {
            if(open())
            {
                m_file.seekp(0,std::ios::end);
                m_file<<str_line<<std::endl;
                return true;
            }
            return false;
        }


        /*
        ** 读取文件的一行，并分割成字符串
        ** 默认读第一行
        */
        FileLine::FileLine(std::string file, std::size_t line_no)
            : IFile(file), m_cur_line_no(0), m_target_line_no(line_no)
        {
            lookup();
        }

        std::string FileLine::get()
        {
            return m_line;
        }

        std::vector<std::string>& FileLine::data()
        {
            if (m_data.empty())
            {
                IStr(m_line).words(m_data);
            }
            return m_data;
        }

        std::string FileLine::operator[] (int idx)
        {
            data();
            std::size_t pos = (idx < 0) ? (m_data.size()-idx) : idx;
            if (pos < m_data.size())
            {
                return m_data[pos];
            }
            return std::string();
        }

        FileLine::ParseStatus FileLine::parse_line(std::string line)
        {
            if (++m_cur_line_no == m_target_line_no)
            {
                m_line = line;
                return m_line.empty() ? PARSE_ERROR : PARSE_OK;
            }
            return PARSE_CONTINUE;
        }
    }
}
