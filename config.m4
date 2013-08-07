PHP_ARG_ENABLE(range2cidr, whether to enable RANGE2CIDR extension, [  --enable-range2cidr   Enable RANGE2CIDR extension])
if test "$PHP_RANGE2CIDR" != "no"; then
    PHP_NEW_EXTENSION(range2cidr, range2cidr.c, $ext_shared)
fi
