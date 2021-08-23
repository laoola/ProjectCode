#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pty.h>
#include <syslog.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <poll.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <string>
#include <vector>

#include "Command.h"
#include "istring.h"

namespace utils
{
	Command::Command(const std::string &cmd_str, line_type l_type)
	{
		cmd = cmd_str;
		master_fd = -1;
		exit_code = -1;
		child_pid = 0;
		lin_type = l_type;
	}



	Command::~Command()
	{
		this->exit();
	}

	bool Command::isCmdExist(std::string cmd)
	{
		bool ret = false;

		std::vector<std::string> vecStr;
		if (!IStr(cmd).words(vecStr) || vecStr.size() == 0)
		{
			return ret;
		}

		std::string check = "which " + vecStr[0] + " 2>/dev/null";
		FILE* mainFp = popen(check.c_str(), "r");
		if (mainFp == NULL)
		{
			return ret;
		}

		//最多读取两行
		for (int i = 0; i < 2; ++i)
		{
			char line[1024] = { 0 };
			if (fgets(line, sizeof(line) - 1, mainFp) == NULL)
			{
				//不存在
				break;
			}

			//  /usr/bin/ls -> ls
			std::string result(line);
			std::size_t index = result.rfind("/");
			if (index == std::string::npos)
			{
				break;
			}
		
			if (result.compare(index + 1, vecStr[0].length(), vecStr[0]) == 0)
			{
				//找到了
				ret = true;
				break;
			}
		}

		pclose(mainFp);
		return ret;
	}

	bool Command::start(const std::string &cmd_str)
	{
		if (child_pid != 0)
		{
			//上一条命令还没结束
			return false;
		}

		if (!cmd_str.empty())
		{
			cmd = cmd_str;
		}

		child_pid = forkpty(&master_fd, NULL, NULL, NULL);
		if (child_pid < 0)
		{
			//fork失败
			cmd = "";
			child_pid = 0;
			master_fd = -1;
			syslog(LOG_ERR, "Command %s forkpty error : %s", cmd.c_str(), strerror(errno));
			return false;
		}
		else if (child_pid == 0)
		{
			//子进程 从终端执行exec
			closeIrrelevantFd(); //关闭除 0 1 2 三个文件描述符外的所有文件描述符
			char *argv[4];
			char *cmd_arg = strdup(cmd.c_str());
			//argv = (char **)malloc(sizeof(char*)*(3 + 1));
			//if (argv == NULL)
			//{
			//	::exit(-1);
			//}
			if(cmd_arg == NULL)
			{
				::exit(-1);
			}

            argv[0] = const_cast<char *>("sh");

            argv[1] = const_cast<char *>("-c");

			argv[2] = cmd_arg;

			argv[3] = NULL;

			execvp("/bin/sh", argv);  //exec执行成功 则不会返回
			free(cmd_arg);
			cmd_arg = NULL;

			::exit(-1); //如果exec执行失败 子进程带-1退出
		}
		else
		{
			//父进程 主终端
			set_noblock(master_fd);
			return true;
		}
	}

	int Command::exit()
	{
		if (child_pid != 0)
		{
			if (kill(child_pid, 0) == 0)
			{
				//当前执行的进程还未退出
				kill(-child_pid, SIGKILL);  //使用不可忽略或捕捉的信号可靠地杀死进程组的所有进程
			}
			int res_status;
			waitpid(child_pid, &res_status, 0);
			exit_code = res_status;
			child_pid = 0;
			close(master_fd);
			master_fd = -1;
            //printf("child exit code = %d\n", WEXITSTATUS(res_status));
			return res_status;
		}
		return -1;
	}


	ssize_t Command::read(char *buff, size_t cout, int time_out, bool is_immediately)
	{
		int event_num = 0, res = -1;
		struct pollfd pollfd_st[1];
		int _time_out = time_out, len_r = 0;

		pollfd_st[0].fd = master_fd;
		pollfd_st[0].events = POLLIN | POLLRDHUP; //可读 对端关闭

		char *recv_buff_ptr = buff;
		int need_recv = cout;
		int now_recv = 0;

		if (master_fd < 0)
		{
			return -1;
		}

		//判断是否立即 有数据可读 为了兼容外部的Epoll垂直触发
		if (is_immediately)
		{
			event_num = poll(pollfd_st, 1, 0);
			if (event_num == 0)
			{
				res = -EAGAIN;  //不是立即可用 但没有错误
				goto out;
			}
			else if (event_num > 0)
			{
				if (pollfd_st[0].revents&(POLLERR | POLLHUP | POLLRDHUP | POLLNVAL))
				{
					if (pollfd_st[0].revents & POLLERR)
					{
#ifdef DEBUG
						fprintf(stderr, "master fd = %d POLLERR\n", master_fd);
#endif
					}
					if (pollfd_st[0].revents & POLLHUP)
					{
#ifdef DEBUG
						fprintf(stderr, "master fd = %d POLLHUP\n", master_fd);
#endif
					}
					if (pollfd_st[0].revents & POLLNVAL)
					{
#ifdef DEBUG
						fprintf(stderr, "master fd = %d POLLRHUP\n", master_fd);
#endif
					}
					if (pollfd_st[0].revents & POLLRDHUP)
					{
#ifdef DEBUG
						fprintf(stderr, "master fd = %d POLLRHUP\n", master_fd);
#endif
					}
					res = -1;  //有错误发生
					goto out;
				}
			}
			else
			{
				res = -1;
				goto out;
			}
		}

		while (1)
		{

			timeval befor;
			int flag = 0;
			if (gettimeofday(&befor, NULL) == 0)
			{
				flag = 1;
			}

            //printf("befor wait\n");

			event_num = poll(pollfd_st, 1, _time_out);

            //printf("recv event num = %d\n",event_num);
			if (time_out != -1)
			{
				timeval after;
				if (gettimeofday(&after, NULL) == 0)
				{
					if (flag)
					{
						long m_sec_b = befor.tv_sec * 1000 + befor.tv_usec / 1000;
						long m_sec_a = after.tv_sec * 1000 + after.tv_usec / 1000;
						long diff = m_sec_a - m_sec_b;
						if (diff >= 0)
						{
							_time_out = _time_out - diff;
							if (_time_out < 0)
							{
								_time_out = 0;
							}
						}
					}
				}
			}

			if (event_num > 0)
			{
				if (pollfd_st[0].fd == master_fd)
				{
					//优先处理IN事件 防止start后程序执行非常迅速在read前已经退出 但是输出还是在缓冲区内保存着
					if (pollfd_st[0].events&POLLIN)
					{
						while ((len_r = ::read(pollfd_st[0].fd, recv_buff_ptr + now_recv, need_recv - now_recv)) > 0 && (now_recv += len_r) < need_recv);
						if (now_recv == need_recv)
						{
							res = now_recv;
							goto out;
						}
						else
						{
							if (len_r <= 0)
							{
								if (len_r == 0)
								{
									//对端挂断
#ifdef DEBUG
									fprintf(stderr, "master fd = %d read len = 0\n", master_fd);
#endif
									res = -1;
									goto out;
								}
								else
								{
									if (errno == EAGAIN || errno == EWOULDBLOCK)
									{
										//当前没有立即可读数据 继续poll
										continue;
									}
									else
									{
										//遇到其他错误
#ifdef DEBUG
										fprintf(stderr, "master fd %d read error %s \n", master_fd, strerror(errno));
#endif
										res = -1;
										goto out;
									}
								}
							}
							continue;
							//len_r>0 但是now_recv < need_recv 继续
						}
					}

					//优先级 POLLIN > POLLERR=POLLRDHUP=POLLHUP  当没有任何IN事件 检测是否有异常事件
					if (pollfd_st[0].revents&(POLLERR | POLLHUP | POLLRDHUP | POLLNVAL))
					{
						//错误 自己挂断 对方挂断 处理方式相同  套接字错误
						if (pollfd_st[0].revents & POLLERR)
						{
#ifdef DEBUG
							fprintf(stderr, "master fd = %d POLLERR\n", master_fd);
#endif
						}
						if (pollfd_st[0].revents & POLLHUP)
						{
#ifdef DEBUG
							fprintf(stderr, "master fd = %d POLLHUP\n", master_fd);
#endif
						}
						if (pollfd_st[0].revents & POLLNVAL)
						{
#ifdef DEBUG
							fprintf(stderr, "master fd = %d POLLRHUP\n", master_fd);
#endif
						}
						if (pollfd_st[0].revents & POLLRDHUP)
						{
#ifdef DEBUG
							fprintf(stderr, "master fd = %d POLLRHUP\n", master_fd);
#endif
						}
						res = -1;
						goto out;
					}
				}
			}
			else if (event_num == 0)
			{
				//超时 返回当前已经获取的字节数
				res = now_recv;
				goto out;
			}
			else
			{
				//错误
#ifdef DEBUG
				fprintf(stderr, "poll wait error %s \n", strerror(errno));
#endif
				res = -1;
				goto out;
			}
		}

	out:
		if (res < 0 && now_recv>0)
		{
			//msater发生错误 或者poll错误 或者不是立即可读 如果当前接收到的字节数大于0则返回成功收到的字节数
			//如果当前接收数小于等于0 则返回错误码 注只有当res=-1时 master才时真正的异常 当res=-EAGAIN 只是没有立即可读的数据而已
			res = now_recv;
		}
		return res;
	}

	ssize_t Command::readLine(std::string &line_buf, int time_out, bool is_immediately)
	{

		int event_num = 0, res = -1;
		struct pollfd pollfd_st[1];
		int _time_out = time_out, len_r = 0;
		char ch_buff;

		pollfd_st[0].fd = master_fd;
		pollfd_st[0].events = POLLIN | POLLRDHUP; //可读 对端关闭

		if (master_fd < 0)
		{
			return -1;
		}

		line_buf = "";
		//判断是否立即 有数据可读 为了兼容外部的Epoll垂直触发
		if (is_immediately)
		{
			event_num = poll(pollfd_st, 1, 0);
			if (event_num == 0)
			{
				res = -EAGAIN;  //不是立即可用 但没有错误
				goto out;
			}
			else if (event_num > 0)
			{
				if (pollfd_st[0].revents&(POLLERR | POLLHUP | POLLRDHUP | POLLNVAL))
				{
					if (pollfd_st[0].revents & POLLERR)
					{
#ifdef DEBUG
						fprintf(stderr, "master fd = %d POLLERR\n", master_fd);
#endif
					}
					if (pollfd_st[0].revents & POLLHUP)
					{
#ifdef DEBUG
						fprintf(stderr, "master fd = %d POLLHUP\n", master_fd);
#endif
					}
					if (pollfd_st[0].revents & POLLNVAL)
					{
#ifdef DEBUG
						fprintf(stderr, "master fd = %d POLLRHUP\n", master_fd);
#endif
					}
					if (pollfd_st[0].revents & POLLRDHUP)
					{
#ifdef DEBUG
						fprintf(stderr, "master fd = %d POLLRHUP\n", master_fd);
#endif
					}
					res = -1;  //有错误发生
					goto out;
				}
			}
			else
			{
				res = -1;
				goto out;
			}
		}

		while (1)
		{

			timeval befor;
			int flag = 0;
			if (gettimeofday(&befor, NULL) == 0)
			{
				flag = 1;
			}

            //printf("befor wait\n");

			event_num = poll(pollfd_st, 1, _time_out);

            //printf("recv event num = %d\n",event_num);
			if (time_out != -1)
			{
				timeval after;
				if (gettimeofday(&after, NULL) == 0)
				{
					if (flag)
					{
						long m_sec_b = befor.tv_sec * 1000 + befor.tv_usec / 1000;
						long m_sec_a = after.tv_sec * 1000 + after.tv_usec / 1000;
						long diff = m_sec_a - m_sec_b;
						if (diff >= 0)
						{
							_time_out = _time_out - diff;
							if (_time_out < 0)
							{
								_time_out = 0;
							}
						}
					}
				}
			}

			if (event_num > 0)
			{
				if (pollfd_st[0].fd == master_fd)
				{
					//优先处理IN事件 防止start后程序执行非常迅速在read前已经退出 但是输出还是在缓冲区内保存着
					if (pollfd_st[0].events&POLLIN)
					{
						ch_buff = '\0';
						while ((len_r = ::read(pollfd_st[0].fd, &ch_buff, 1)) > 0)
						{
							line_buf += ch_buff;
							if (ch_buff=='\n')
							{
								break;
							}
							ch_buff = '\0';
						}
						if (ch_buff == '\n')
						{
							res = line_buf.length();
							goto out;
						}
						else
						{
							if (len_r <= 0)
							{
								if (len_r == 0)
								{
									//对端挂断
#ifdef DEBUG
									fprintf(stderr, "master fd = %d read len = 0\n", master_fd);
#endif
									res = -1;
									goto out;
								}
								else
								{
									if (errno == EAGAIN || errno == EWOULDBLOCK)
									{
										//当前没有立即可读数据 继续poll
										continue;
									}
									else
									{
										//遇到其他错误
#ifdef DEBUG
										fprintf(stderr, "master fd %d read error %s \n", master_fd, strerror(errno));
#endif
										res = -1;
										goto out;
									}
								}
							}
							continue;
							//len_r>0 但是没读到换行\n 继续
						}
					}

					//优先级 POLLIN > POLLERR=POLLRDHUP=POLLHUP  当没有任何IN事件 检测是否有异常事件
					if (pollfd_st[0].revents&(POLLERR | POLLHUP | POLLRDHUP | POLLNVAL))
					{
						//错误 自己挂断 对方挂断 处理方式相同  套接字错误
						if (pollfd_st[0].revents & POLLERR)
						{
#ifdef DEBUG
							fprintf(stderr, "master fd = %d POLLERR\n", master_fd);
#endif
						}
						if (pollfd_st[0].revents & POLLHUP)
						{
#ifdef DEBUG
							fprintf(stderr, "master fd = %d POLLHUP\n", master_fd);
#endif
						}
						if (pollfd_st[0].revents & POLLNVAL)
						{
#ifdef DEBUG
							fprintf(stderr, "master fd = %d POLLRHUP\n", master_fd);
#endif
						}
						if (pollfd_st[0].revents & POLLRDHUP)
						{
#ifdef DEBUG
							fprintf(stderr, "master fd = %d POLLRHUP\n", master_fd);
#endif
						}
						res = -1;
						goto out;
					}
				}
			}
			else if (event_num == 0)
			{
				//超时 返回当前已经获取的字节数
				res = line_buf.length();
				goto out;
			}
			else
			{
				//错误
#ifdef DEBUG
				fprintf(stderr, "poll wait error %s \n", strerror(errno));
#endif
				res = -1;
				goto out;
			}
		}

	out:
		if (res < 0 && line_buf.length()>0)
		{
			//msater发生错误 或者poll错误 或者不是立即可读 如果当前接收到的字节数大于0则返回成功收到的字节数
			//如果当前接收数小于等于0 则返回错误码 注只有当res=-1时 master才时真正的异常 当res=-EAGAIN 只是没有立即可读的数据而已
			res = line_buf.length();
		}
		if (line_buf.length() > 0)
		{
			if (line_buf.length() > 1)
			{
				if (line_buf.at(line_buf.length() - 2) == '\r' && line_buf.at(line_buf.length() - 1) == '\n')
				{
					line_buf.erase(line_buf.length() - 2);
					if (lin_type == LINE_CRLF)
					{
						line_buf += "\r\n";
					}
					else if (lin_type == LINE_LF)
					{
						line_buf += "\n";
					}
				}
				else if (line_buf.at(line_buf.length() - 1) == '\n')
				{
					line_buf.erase(line_buf.length() - 1);
					if (lin_type == LINE_CRLF)
					{
						line_buf += "\r\n";
					}
					else if (lin_type == LINE_LF)
					{
						line_buf += "\n";
					}
				}
			}
			else if (line_buf.length() == 1)
			{
				if (line_buf.at(line_buf.length() - 1) == '\n')
				{
					line_buf.erase(line_buf.length() - 1);
					if (lin_type == LINE_CRLF)
					{
						line_buf += "\r\n";
					}
					else if (lin_type == LINE_LF)
					{
						line_buf += "\n";
					}
				}
			}

			res = line_buf.length();
		}
		return res;
	}

	ssize_t Command::write(const char *buff, size_t cout, int time_out, bool is_immediately)
	{

		struct pollfd pollfd_st[1];
		int event_num = 0;
		int _time_out = time_out;
		int res = -1;
		pollfd_st[0].fd = master_fd;
		pollfd_st[0].events = POLLOUT | POLLRDHUP; //可写  对端关闭
		int len_w = 0, now_send = 0, need_send = cout;

		if (master_fd < 0)
		{
			return -1;
		}

		//判断是否立即 有数据可写 为了兼容外部的Epoll垂直触发
		if (is_immediately)
		{
			event_num = poll(pollfd_st, 1, 0);
			if (event_num == 0)
			{
				res = -EAGAIN;  //不是立即可写 但没有错误
				goto out;
			}
			else if (event_num > 0)
			{
				if (pollfd_st[0].revents&(POLLERR | POLLHUP | POLLRDHUP | POLLNVAL))
				{
					if (pollfd_st[0].revents & POLLERR)
					{
#ifdef DEBUG
						fprintf(stderr, "master fd = %d POLLERR\n", master_fd);
#endif
					}
					if (pollfd_st[0].revents & POLLHUP)
					{
#ifdef DEBUG
						fprintf(stderr, "master fd = %d POLLHUP\n", master_fd);
#endif
					}
					if (pollfd_st[0].revents & POLLNVAL)
					{
#ifdef DEBUG
						fprintf(stderr, "master fd = %d POLLRHUP\n", master_fd);
#endif
					}
					if (pollfd_st[0].revents & POLLRDHUP)
					{
#ifdef DEBUG
						fprintf(stderr, "master fd = %d POLLRHUP\n", master_fd);
#endif
					}
					res = -1;  //有错误发生
					goto out;
				}
			}
			else
			{
				res = -1;
				goto out;
			}
			//当前信道 立即可写
		}

		while (1)
		{
			timeval befor;
			int flag = 0;
			if (gettimeofday(&befor, NULL) == 0)
			{
				flag = 1;
			}
			event_num = poll(pollfd_st, 1, _time_out); //因为文件描述符只有一个 所以使用更轻量的poll

			if (time_out != -1)
			{
				timeval after;
				if (gettimeofday(&after, NULL) == 0)
				{
					if (flag)
					{
						long m_sec_b = befor.tv_sec * 1000 + befor.tv_usec / 1000;
						long m_sec_a = after.tv_sec * 1000 + after.tv_usec / 1000;
						long diff = m_sec_a - m_sec_b;
						if (diff >= 0)
						{
							_time_out = _time_out - diff;
							if (_time_out < 0)
							{
								_time_out = 0;
							}
						}
					}
				}
			}

			if (event_num > 0)
			{
				if (pollfd_st[0].fd == master_fd)
				{
					//POLLOUT需要处理  优先级 POLLERR=POLLRDHUP=POLLHUP  > POLLOUT
					if (pollfd_st[0].revents&(POLLERR | POLLHUP | POLLRDHUP | POLLNVAL))
					{
						//错误 自己挂断 对方挂断 处理方式相同  套接字错误
						if (pollfd_st[0].revents & POLLERR)
						{
#ifdef DEBUG
							fprintf(stderr, "master fd = %d POLLERR\n", master_fd);
#endif
						}
						if (pollfd_st[0].revents & POLLHUP)
						{
#ifdef DEBUG
							fprintf(stderr, "master fd = %d POLLHUP\n", master_fd);
#endif
						}
						if (pollfd_st[0].revents & POLLNVAL)
						{
#ifdef DEBUG
							fprintf(stderr, "master fd = %d POLLRHUP\n", master_fd);
#endif
						}
						if (pollfd_st[0].revents & POLLRDHUP)
						{
#ifdef DEBUG
							fprintf(stderr, "master fd = %d POLLRHUP\n", master_fd);
#endif
						}
						res = -1;
						goto out;
					}
					if (pollfd_st[0].revents&POLLOUT)
					{
						while ((len_w = ::write(pollfd_st[0].fd, buff + now_send, need_send - now_send)) > 0 && (now_send += len_w) < need_send);
						if (now_send == need_send)
						{
							//写入需要写入的长度
							res = now_send;
							goto out;
						}
						else
						{
							if (len_w <= 0)
							{
								if (len_w == 0)
								{
									//缓冲区已满 继续等待可写
									continue;
								}
								else
								{
									if (errno == EAGAIN || errno == EWOULDBLOCK)
									{
										//当前缓冲区已满 继续poll
										continue;
									}
									else
									{
										//遇到其他错误
#ifdef DEBUG
										fprintf(stderr, "master fd %d read error %s \n", master_fd, strerror(errno));
#endif
										res = -1;
										goto out;
									}
								}
							}
						}
					}
				}
			}
			else if (event_num == 0)
			{
				//超时
				res = 0;
				goto out;
			}
			else
			{
				//发送错误
				res = -1;
				goto out;
			}

		}
	out:
		if (now_send > 0)
		{
			//当超时或者发生错误之前已经写入了部分数据则返回成功写入的字节数
			res = now_send;
		}
		return res;
	}

	ssize_t Command::writeLine(const std::string &line_cmd, int time_out, bool is_immediately)
	{
		std::string line_buff;
		line_buff = line_cmd;
		//如果行尾没有\n 则添加\n
		if (line_buff.length() > 0)
		{
			if (line_buff.at(line_buff.length() - 1) != '\n')
			{
				line_buff += "\n";
			}
		}
		else
		{
			line_buff += "\n";
		}
		return this->write(line_buff.c_str(), line_buff.length(), time_out, is_immediately);
	}

	void Command::closeIrrelevantFd()
	{
		DIR *dir;
		struct dirent *dir_i;
		char fd_path[2048], f_path[2048], link_path[2048];
		int len;
		struct stat st;
		snprintf(fd_path, sizeof(fd_path), "/proc/%d/fd", (int)getpid());
		dir = opendir(fd_path);
		if (dir == NULL)
		{
			syslog(LOG_ERR, "Opendir /proc/self/fd Error : %s ! ", strerror(errno));
		}
		while ((dir_i = readdir(dir)) != NULL)
		{
			if (strcmp(dir_i->d_name, ".") == 0  || 
				strcmp(dir_i->d_name, "..") == 0 ||
				strcmp(dir_i->d_name, "0") == 0  ||
				strcmp(dir_i->d_name, "1") == 0  ||
				strcmp(dir_i->d_name, "2") == 0    )
			{
				//跳过 . .. 0 1 2
				continue;
			}
			snprintf(f_path, sizeof(f_path), "%s/%s", fd_path, dir_i->d_name);
			if (lstat(f_path, &st) == 0)
			{
				if (S_ISLNK(st.st_mode))
				{
					len = readlink(f_path, link_path, sizeof(link_path));
					if (len > 0)
					{
						link_path[len] = '\0';
						if (strcmp(link_path, fd_path) == 0)
						{
							//跳过opendir的目录项
							continue;
						}
					}
				}
			}
			close(atoi(dir_i->d_name));
		}
		closedir(dir);
	}


	int Command::set_noblock(int fd)
	{
		int flags;
		flags = fcntl(fd, F_GETFL);
		flags |= O_NONBLOCK;
		return fcntl(fd, F_SETFL, flags);
	}
} //end utils ns

