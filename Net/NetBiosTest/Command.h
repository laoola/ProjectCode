#ifndef _COMMAND_EXEC_H_
#define _COMMAND_EXEC_H_

#include <stdio.h>
#include <unistd.h>
#include <pty.h>
#include <map>
#include <dlfcn.h>
#include <assert.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <pty.h>
#include <iostream>

using namespace std;

namespace utils
{
	class Command
	{
	public:

		enum line_type
		{
			//LINE_IGNOE = 0,  //read line 忽略(不读出)"/r/n"  这种情况不能准确知道是否获取了完整的一行，暂时不使用
			LINE_CRLF = 1,   //read line 行尾为"/r/n" 默认方式
			LINE_LF = 2      //read line 行尾为"/n"
		};

		//************************************
		// Method:    Command
		// FullName:  utils::Command::Command
		// Access:    private 
		// Returns:   
		// Qualifier:
		// Parameter: const std::string cmd_str 执行的命令行 可以在start时再指定
		// Parameter: line_type l_type readLine的换行方式 LINE_CRLF(默认\r\n) LINE_LF(\n)
		//************************************
		Command(const std::string &cmd_str = "", line_type l_type = LINE_CRLF);
		~Command();


		//************************************
		// Method:    isCmdExist
		// FullName:  utils::Command::isCmdExist
		// Access:    public 
		// Returns:   bool true存在, false不存在
		// Qualifier: 检查命令是否存在
		// Parameter: std::string cmd 如ls -al ，检查ls是否存在
		//************************************
		static bool isCmdExist(std::string cmd);


		//************************************
		// Method:    start
		// FullName:  utils::Command::start
		// Access:    public 
		// Returns:   bool forkpty成功返回true  forkpty失败返回false 注！不是exec的成功与否
		// Qualifier: 执行cmd命令行
		// Parameter: const std::string cmd_str 如果非空则覆盖构造函数中给定的命令行
		//************************************
		bool start(const std::string &cmd_str = "");


		//************************************
		// Method:    exit
		// FullName:  utils::Command::exit
		// Access:    public 
		// Returns:   int -1失败  其他情况返回cmd程序退出状态
		// Qualifier: 如果cmd执行成功则停止命令行程序 并返回cmd的退出状态 可以使用WEXITSTATUS()对该值进行计算获取程序返回值
		//************************************
		int exit();  //结束命令行程序并返回退出码 


		//************************************
		// Method:    read
		// FullName:  utils::Command::read
		// Access:    public 
		// Returns:   ssize_t 读取到的字节数 -EAGAIN不是立即可读  -1发生错误一般是执行的程序退出 >=0成功读取的字节数
		// Qualifier: 在设置的超时时间内ms读取指定的字节到buff中
		// Parameter: char * buff
		// Parameter: size_t cout
		// Parameter: int time_out  超时时间ms
		// Parameter: bool is_immediately 是否检测立即可用 如果不是立即可用返回 -EAGAIN  -11
		//            当在指定超时时间内没有读取到要求的字节数则返回现已经读取的字节数>=0
		//            只有当该函数返回-1时才代表执行的程序退出！-EAGAIN(-11)只表示没有数据立即可读并不是发生错误
		//************************************
		ssize_t read(char *buff, size_t cout, int time_out = -1, bool is_immediately = false);


		//************************************
		// Method:    readLine
		// FullName:  utils::Command::readLine
		// Access:    public 
		// Returns:   ssize_t
		// Qualifier: ssize_t 读取到的字节数包含换行符（/r/n或者/n 具体哪一种换行方式由构造函数指定） -EAGAIN不是立即可读  -1发生错误一般是执行的程序退出 >=0成功读取的字节数
		// Parameter: std::string & line_buf
		// Parameter: int time_out 超时时间ms
		// Parameter: bool is_immediately 是否检测立即可用 如果不是立即可用返回 -EAGAIN  -11
		//            当在指定超时时间内没有读取到要求的字节数则返回现已经读取的字节数>=0
		//            只有当该函数返回-1时才代表执行的程序退出！-EAGAIN(-11)只表示没有数据立即可读并不是发生错误
		//            当返回一个不完整的行时line_buff最后一个字符不为\n
		//************************************
		ssize_t readLine(std::string &line_buf, int time_out = -1, bool is_immediately = false);


		//************************************
		// Method:    write
		// FullName:  utils::Command::write
		// Access:    public 
		// Returns:   ssize_t
		// Qualifier:
		// Parameter: const char * buff
		// Parameter: size_t cout
		// Parameter: int time_out
		// Parameter: bool is_immediately
		//************************************
		ssize_t write(const char *buff, size_t cout, int time_out = -1, bool is_immediately = false);


		//************************************
		// Method:    writeLine
		// FullName:  utils::Command::writeLine
		// Access:    public 
		// Returns:   ssize_t
		// Qualifier:
		// Parameter: const std::string & line_cmd
		// Parameter: int time_out
		// Parameter: bool is_immediately
		//************************************
		ssize_t writeLine(const std::string &line_cmd, int time_out = -1, bool is_immediately = false);

	protected:
		int master_fd;        //主设备文件描述符 方便子类实现更复杂的交互式命令行类
		int set_noblock(int fd);
	private:
		std::string cmd;
		int exit_code;
		
		pid_t child_pid;
		line_type lin_type;

		//************************************
		// Method:    closeIrrelevantFd
		// FullName:  utils::Command::closeIrrelevantFd
		// Access:    private 
		// Returns:   void
		// Qualifier: 关闭 除了 标准输出、标准输入、标准错误 外的已打开的所有文件描述符
		//************************************
		void closeIrrelevantFd();

		Command(const Command &);                         //禁止复制
		Command &operator=(const Command &);              //禁止赋值
	};
} //end utils ns
#endif
