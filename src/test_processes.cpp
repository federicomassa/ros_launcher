#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include <boost/regex.hpp>

void send_command(const std::string& dev_name, const std::string& cmd, float sleep_seconds = 1)
{
  const char* dev_name_c = dev_name.c_str();
  const char* cmd_c = cmd.c_str();
  
  int fd = open(dev_name_c,O_RDWR);
 
  if(fd == -1) {
    perror("open DEVICE");
    exit(1);
  }
  
  for (int i = 0; cmd_c[i]; i++) {
    ioctl (fd, TIOCSTI, cmd_c+i);
  }  
  
  const char *nl = "\n";
  
  ioctl (fd, TIOCSTI, nl);
  close(fd);
  
  std::this_thread::sleep_for(std::chrono::milliseconds(int(sleep_seconds*1000)));
}

int main()
{
  system("gnome-terminal -e 'bash -c \"tty > ~/outputfile; bash\"'");
  sleep(3);
  
  std::ifstream dev_file;
  
  std::string home_dir = "";
  
  if (getenv("HOME"))
    home_dir += getenv("HOME");
  
  std::cout << "Home: " << home_dir << std::endl;
  dev_file.open((home_dir + "/outputfile").c_str());
  
  if (!dev_file.is_open())
  {
   std::cout << "Error: device file not open" << std::endl; 
   exit(1);
  }
  
  std::string dev_name;
  std::getline(dev_file, dev_name);

  remove((home_dir + "/outputfile").c_str());
  
  std::cout << dev_name << std::endl;
  
  send_command(dev_name, std::string() + "cd " + home_dir + "/Roborace/tools", 0.1);
  send_command(dev_name, "source devel/setup.bash", 0.2);
  send_command(dev_name, "roslaunch replay_tools test.launch", 4);  
  system("ps aux | grep roslaunch > ~/roslaunch_pid.txt");
  
  std::ifstream launch_pid((home_dir + "/roslaunch_pid.txt").c_str());
  
  boost::regex roslaunch_expr{"\\w+\\s+(\\d+)\\s+[\\w\\s\\d\\.\\/]+(pts\\/\\d+)[\\w\\s\\d\\.\\/\\+\\:]+roslaunch[\\w\\s\\d\\.\\-\\/]+"};
//  boost::regex roslaunch_expr{"[\\w\\s\\d\\./]+(\\d)+[\\w\\s\\d\\./]+(pts/\\d+)[\\w\\s\\d\\./\\+\\:]+roslaunch[\\w\\s\\d\\.\\-/]+"};
  boost::smatch match;
  bool found = false;
  
  while (!launch_pid.eof())
  {
    std::string str;
    std::getline(launch_pid, str);
      
    std::cout << "Matching... " << str << std::endl;
    
    if (boost::regex_match(str, match, roslaunch_expr))
    {
      if (match.size() == 3)
      {
	std::string matched_dev_name = "/dev/" + match[2];
	std::cout << "Matched dev name: " << matched_dev_name << std::endl;
	
	if (matched_dev_name == dev_name)
	{
	  std::cout << "Match 1: " << match[1] << std::endl;
	  std::cout << "Match 2: " << match[2] << std::endl;  
	  found = true;
	  break;
	}
      }
      
      else
      {
	std::cout << "WTF!!\n";
	for (int i = 0; i < match.size(); i++)
	{
	 std::cout << match[i] << std::endl; 
	}
	
      }
    }
    
  
  }

  if (found)
  {
    std::cout << "Killing pid: " << match[1] << " in 10 seconds...\n";
    sleep(10);

    system((std::string("kill -2 ") + match[1]).c_str());
  }
  
  
  
  return 0;
}