ipquery
=======

基于纯真库的ip信息查询服务

python扩展：
gcc -shared -lpython2.7 -fPIC  -I /usr/include/python2.7  ip.c ip_python_binding.c -o ipquery.so
