//
//  s21_sub.c
//  test
//
//  Created by mcbk on 18.12.2022.
//

#include <stdio.h>

#include "s21_decimal.h"
#include "supp_func.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  s21_big_decimal bv1 = convert_dec_to_big(value_1),
                  bv2 = convert_dec_to_big(value_2);
  s21_big_decimal res;
  memset(&res, 0, 28);
  make_equal_exp(&bv1, &bv2);
  //    print_bits_big_decimal(&bv1);
  //    print_bits_big_decimal(&bv2);
  minus_equal_exp(bv1, bv2, &res);
  //   print_bits_big_decimal(&res);
  err = convert_big_to_dec(res, result, get_exp(bv1.bits[6]), 1);
  return err;
}
