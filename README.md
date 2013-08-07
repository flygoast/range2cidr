RANGE2CIDR
==========                                                                                                             

A PHP extension providing a function `range2cidr` used to convert IP range to CIDR.


Installation
------------

Run listed commands inside the directory
    
    * phpize
    * ./configure
    * make
    * sudo make install
    * php -d extension=range2cidr.so -r 'var_dump(range2cidr("123.123.123.123", "123.123.123.125"));'
