ipquery
=======
基于[ngx\_lua](http://wiki.nginx.org/HttpLuaModule)的纯真库ip信息查询服务

原理
=======
使用lua的c api编写ip查询逻辑作为lua的扩展库，直接在nginx中执行lua代码，无需上游服务

使用
=======
* 安装nginx和ngx\_lua模块（或者直接安装[openresty](http://openresty.org/)）
* 编译ip查询模块的lua扩展：cd src; gcc -Wall -O2 -shared -fPIC -llua ip_lua_bingding.c ip.c -o ipquery.so
* 复制ipquery.so到lualib目录
* 按照conf/nginx.conf修改你的nginx配置文件
* 启动nginx
* have funs

性能
=======
机器：4核CPU，4G内存的虚拟机，4个nginx worker进程

    Server Software:        ngx_openresty/1.2.7.8
    Server Hostname:        192.168.2.96
    Server Port:            8080

    Document Path:          /ip?ip=119.254.241.70
    Document Length:        26 bytes

    Concurrency Level:      100
    Time taken for tests:   1.343 seconds
    Complete requests:      20000
    Failed requests:        0
    Write errors:           0
    Total transferred:      3560000 bytes
    HTML transferred:       520000 bytes
    Requests per second:    14896.15 [#/sec] (mean)
    Time per request:       6.713 [ms] (mean)
    Time per request:       0.067 [ms] (mean, across all concurrent requests)
    Transfer rate:          2589.37 [Kbytes/sec] received

    Connection Times (ms)
    min  mean[+/-sd] median   max
    Connect:        0    2   1.1      2       7
    Processing:     1    4   1.4      4       8
    Waiting:        1    3   1.5      3       8
    Total:          3    7   1.5      6      13

    Percentage of the requests served within a certain time (ms)
    50%      6
    66%      7
    75%      7
    80%      7
    90%      9
    95%     10
    98%     11
    99%     11
    100%    13 (longest request)

great thanks to ngx\_lua and openresty projects
