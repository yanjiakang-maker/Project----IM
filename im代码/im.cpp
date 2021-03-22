#include "im.hpp"

void sql_test() {
  im::TableUser user;
  //user.Insert("lisi", "111111");
  //user.UpdatePasswd("zhangsan", "111111");
  //cout << user.VerifyUser("lisi", "111111") << endl;
  //cout << user.Exists("lisi") << endl;
  Json::Value val;
  user.SelectAll(&val);
  Json::StyledWriter writer;
  //cout << writer.write(val) << endl;

  user.Delete("zhangsan");
}


int main() {
  im::IM im_server;
  im_server.Init();
  im_server.Run();

  return 0;
}
