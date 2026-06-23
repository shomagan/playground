#include "keyboard.hpp"

#ifdef _WIN32
#include <conio.h>

std::string get_ch()
{
   if (_kbhit()) {
      char c = static_cast<char>(_getch());
      return std::string(1, c);
   }
   return "";
}

#else
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

std::string get_ch()
{
   int fd = STDIN_FILENO;
   struct termios old_term, new_term;
   tcgetattr(fd, &old_term);
   new_term = old_term;
   new_term.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO));
   tcsetattr(fd, TCSANOW, &new_term);

   fd_set readfds;
   FD_ZERO(&readfds);
   FD_SET(fd, &readfds);
   struct timeval tv{0, 0};

   std::string result;
   if (select(fd + 1, &readfds, nullptr, nullptr, &tv) > 0) {
      char c;
      if (read(fd, &c, 1) == 1)
         result = std::string(1, c);
   }
   tcsetattr(fd, TCSADRAIN, &old_term);
   return result;
}

#endif
