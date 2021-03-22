#include <iostream>
#include <string>
#include "../mongoose.h"

using namespace std;

void callback(struct mg_connection* c, int ev, void* ev_data, void* fn_data) {
  switch(ev) {
    case MG_EV_HTTP_MSG:   //表示当前的请求是一个http请求
      {
        struct mg_http_message* hm = (struct mg_http_message*)ev_data;
        string method;
        method.assign(hm->method.ptr, hm->method.len);
        cout << method << endl;
        string uri;
        uri.assign(hm->uri.ptr, hm->uri.len);
        cout << uri << endl;
        if(mg_http_match_uri(hm, "/hello")) {
          //自己组织简单响应
          //  if(hm->method.ptr == "GET") {
          string body = "<html><body><h1>Hello Bit</h1></body></html>";
          string header = "Content-Type: text/html\r\n";
          mg_http_reply(c, 200, header.c_str(), "%s", body.c_str());
          //   }
        }
        else if(mg_http_match_uri(hm, "/websocket")) {
          //确认升级websocket协议
          //当前连接一旦升级websocket协议, 则下次有数据到来触发的就是WS_MSG事件
          mg_ws_upgrade(c, hm, NULL);
        }
        else {
          //静态页面资源响应
          struct mg_http_serve_opts opts = {.root_dir = "./web_root"};
          mg_http_serve_dir(c, hm, &opts);
        }
      }
      break;
    case MG_EV_WS_MSG: {
      //表示当前请求是一个websocket请求
      struct mg_ws_message* wm = (struct mg_ws_message*)ev_data;
    }
      break;
    case MG_EV_CLOSE: //表示当前连接断开
      break;
    default:
      break;
  }
  return ;
}


int main() {
  //定义句柄
  struct mg_mgr mgr;
  //初始化句柄
  mg_mgr_init(&mgr);
  //创建http连接
  struct mg_connection* lst_http;
  //mg_http_listen(句柄, 监听地址, 回调函数, 传入参数)
  lst_http = mg_http_listen(&mgr, "0.0.0.0:9000", callback, NULL);
  if(lst_http == NULL) {
    return -1;
  }
  //开始监听
  while(1) {
    mg_mgr_poll(&mgr, 1000);  
  }

  return 0;
}
