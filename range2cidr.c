/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: FengGu <flygoast@126.com>                                    |
  +----------------------------------------------------------------------+
*/

/* $Id: range2cidr.c 321634 2012-01-01 13:15:04Z felipe $ */


#include <php.h>

#define PHP_RANGE2CIDR_EXTNAME "range2cidr"
#define PHP_RANGE2CIDR_VERSION "0.0.1"

PHP_FUNCTION(range2cidr);

zend_function_entry range2cidr_functions[] = {
    PHP_FE(range2cidr, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry range2cidr_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_RANGE2CIDR_EXTNAME,
    range2cidr_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_RANGE2CIDR_VERSION,
    STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(range2cidr)

PHP_FUNCTION(range2cidr) {
    zval *from, *to, *function, *ret, **params[1], *ret_arr;
    long l_from, l_to, i, j, s, e, mask, tmp, tmp_mask;
    char buf[20];

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, 
                              "zz", &from, &to) == FAILURE)
    {
        return;
    }

    if (!((Z_TYPE_P(from) == IS_STRING && Z_TYPE_P(to) == IS_STRING)
          || (Z_TYPE_P(from) == IS_LONG && Z_TYPE_P(to) == IS_LONG)))
    {
        RETURN_FALSE;
    }

    if (Z_TYPE_P(from) == IS_STRING) {
        MAKE_STD_ZVAL(function);
        ZVAL_STRING(function, "ip2long", 1);
    
        params[0] = &from;
    
        if (call_user_function_ex(CG(function_table), NULL, function, &ret,
                                  1, params, 0, NULL TSRMLS_CC) == FAILURE)
        {
            RETURN_FALSE;
    
        } else {
    
            if (Z_TYPE_P(ret) == IS_BOOL && Z_BVAL_P(ret) == 0) {
                RETURN_FALSE;
            }
    
            l_from = Z_LVAL_P(ret);
            zval_ptr_dtor(&ret);
        }
    
        params[0] = &to;
        if (call_user_function_ex(CG(function_table), NULL, function, &ret,
                                  1, params, 0, NULL TSRMLS_CC) == FAILURE)
        {
            RETURN_FALSE;
    
        } else {
    
            if (Z_TYPE_P(ret) == IS_BOOL && Z_BVAL_P(ret) == 0) {
                RETURN_FALSE;
            }
    
            l_to = Z_LVAL_P(ret);
            zval_ptr_dtor(&ret);
        }

    } else {
        l_from = Z_LVAL_P(from);
        l_to = Z_LVAL_P(to);
    }

    MAKE_STD_ZVAL(ret_arr);
    array_init(ret_arr);

    s = l_from;

    while (s <= l_to) {
        if (s == l_to) {
            snprintf(buf, sizeof(buf), "%d.%d.%d.%d/%d",
                     (s >> 24) & 0xff, (s >> 16) & 0xff, 
                     (s >> 8) & 0xff, s & 0xff, 32);
            add_next_index_string(ret_arr, buf, 1);
            break;
        }

        for (i = 0, mask = 0, tmp = s; (tmp & 1) == 0; i++) {
            mask = (mask << 1) | 1;
            tmp >>= 1;
        }

        if (i == 0) {
            snprintf(buf, sizeof(buf), "%d.%d.%d.%d/%d",
                     (s >> 24) & 0xff, (s >> 16) & 0xff, 
                     (s >> 8) & 0xff, s & 0xff, 32);
            add_next_index_string(ret_arr, buf, 1);
            s++;

        } else {

            tmp_mask = mask;
            for (j = i; j > 0; j--) {
                e = s | tmp_mask;

                if (e <= l_to) {
                    snprintf(buf, sizeof(buf), "%d.%d.%d.%d/%d",
                             (s >> 24) & 0xff, (s >> 16) & 0xff, 
                             (s >> 8) & 0xff, s & 0xff, 32 - j);
                    add_next_index_string(ret_arr, buf, 1);
                    s = e + 1;
                    break;
                }

                tmp_mask >>= 1;
            }
        }
    }
    *return_value = *ret_arr;
}
