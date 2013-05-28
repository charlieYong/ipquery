ipquery
=======
基于[ngx\_lua](http://wiki.nginx.org/HttpLuaModule)的纯真库ip信息查询服务

原理
=======
使用lua的c api编写ip查询逻辑作为lua的扩展库，直接在nginx中直接lua代码，无需上游服务

使用
=======
1, 安装nginx和ngx\_lua模块（或者直接安装[openresty](http://openresty.org/)）
2, 编译ip查询模块的lua扩展：cd src; gcc -shared -fPIC -llua ip_lua_bingding.c ip.c -o ipquery.so
3, 复制ipquery.so到lualib目录
4, 按照conf/nginx.conf修改你的nginx配置文件
5, 启动nginx
6, have funs

great thanks to ngx\_lua and openresty projects

