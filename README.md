ipquery
=======
基于[ngx\_lua](http://wiki.nginx.org/HttpLuaModule)的纯真库ip信息查询服务

原理
=======
使用lua的c api编写ip查询逻辑作为lua的扩展库，直接在nginx中直接lua代码，无需上游服务

使用
=======
* 安装nginx和ngx\_lua模块（或者直接安装[openresty](http://openresty.org/)）
* 编译ip查询模块的lua扩展：cd src; gcc -shared -fPIC -llua ip_lua_bingding.c ip.c -o ipquery.so
* 复制ipquery.so到lualib目录
* 按照conf/nginx.conf修改你的nginx配置文件
* 启动nginx
* have funs

性能
=======
机器：4核CPU，4G内存的虚拟机，4个nginx work进程
    Server Software:        ngx_openresty/1.2.7.8
    Server Hostname:        192.168.2.96
    Server Port:            8080

    Document Path:          /ip?ip=119.254.241.70
    Document Length:        26 bytes

    Concurrency Level:      1000
    Time taken for tests:   12.781 seconds
    Complete requests:      100000
    Failed requests:        0
    Write errors:           0
    Total transferred:      17800712 bytes
    HTML transferred:       2600104 bytes
    Requests per second:    7824.03 [#/sec] (mean)
    Time per request:       127.811 [ms] (mean)
    Time per request:       0.128 [ms] (mean, across all concurrent requests)
    Transfer rate:          1360.09 [Kbytes/sec] received

    Connection Times (ms)
    min  mean[+/-sd] median   max
    Connect:        0   60 463.4      1    7020
    Processing:     0    2   8.7      1     234
    Waiting:        0    2   8.7      1     234
    Total:          0   62 464.0      2    7213

great thanks to ngx\_lua and openresty projects
=======
